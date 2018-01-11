function void Score_CalcMapPoints(void)
{
    if (MapStart_FullHealPoints > 0) { return; }

    int i;

    int monTypes    = ACS_NamedExecuteWithResult("Score_MonsterTypeCount");
    int testmonTID  = UniqueTID();
    int totalMons   = 0;
    int totalPoints = 0;

    for (i = 0; i < monTypes; i++)
    {
        str name  = ACS_NamedExecuteWithResult("Score_MonsterAtIndex", i);
        int value = ACS_NamedExecuteWithResult("Score_RewardAtIndex",  i);
        
        if (value < 0)
        {
            SpawnForced(name, 0,0,0, testmonTID);
            value = GetActorProperty(testmonTID, APROP_SpawnHealth);
            Thing_Remove(testmonTID);
        }
    
        int count = ThingCountName(name, 0);
        //Log(s:"\cd[", s:name, s:"] \cf(", d:value, s:")\c- Count: ", d:count);

        totalMons   += count;
        totalPoints += count * value;
    }
    
    int averagePoints = 0;
    if (totalMons > 0) { averagePoints  = totalPoints / totalMons; }
    
    // Controls for low and high monster counts
    int fullHealMult = middle(P_FULLHEAL_MIN, FixedMul(totalMons, P_FULLHEAL_PERCENT), P_FULLHEAL_MAX);
    int fullHealPoints = averagePoints * fullHealMult;
    
    MapStart_FullHealPoints = max(5000, ((fullHealPoints + 2500) / 5000) * 5000);
    
    //Log(s:"\ca", d:totalMons, s:"   \cf", d:totalPoints, s:"   \cd", d:MapStart_FullHealPoints);

    // Adjust everyone's base score to match the percentage from the last map if it's non-zero
    for (i = 0; i < PLAYERMAX; i++)
    {
        if (!PlayerInGame(i)) { continue; }
        int percent = Score_GetScorePercent(i);

        // We add 1 because FixedMul loves rounding down, and we don't actually want that!
        Score_SetScore(i, FixedMul(MapStart_FullHealPoints, percent + 1));
    }
}


// This reads dakka_keepscore.
function void Score_Reset(int pln)
{
    int resetMask;

    switch (GetCVar("dakka_keepscore"))
    {
      case 0:
      default:
        resetMask = REWARDMASK_ALL;
        break;

      case 1:
        resetMask = REWARDMASK_NOTDISPLAY;
        break;

      case 2:
        resetMask = REWARDMASK_NOTDISPLAY & REWARDMASK_NOTREWARDS;
        break;

      case 3:
        resetMask = 0;
        break;
    }

    Score_ResetMask(pln, resetMask);
}


function void Score_ResetMask(int pln, int mask)
{
    if (mask & REWARDMASK_SCORE)        { Score_SetScore(pln, 0); }
    if (mask & REWARDMASK_DISPLAYSCORE) { Score_SetDisplayScore(pln, 0); }
    if (mask & REWARDMASK_REWARDCOUNT)  { Score_SetRewardCount(pln, 0); }
    if (mask & REWARDMASK_REGENTIMER)   { Score_SetRegenTimer(pln, 0); }
    if (mask & REWARDMASK_REGENSPENT)   { Score_SetRegenSpent(pln, 0); }
    if (mask & REWARDMASK_EXTRALIVES)   { Score_SetExtraLives(pln, 0); }

    // Never clear HASLIVES because that's kinda important for making
    //  Dakkaguy not BUDDHA at the start of the map.
}
