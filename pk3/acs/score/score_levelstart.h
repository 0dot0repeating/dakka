int Score_TotalNums[2];

function void Score_CalcMapPoints(void)
{
    if (Score_Thresholds[ST_FULLHEAL] > 0) { return; }

    int i;
    int totalMons   = 0;
    int totalPoints = 0;
    
    // Rough check to see if we should have RGF_NOSIGHT
    int testTID = UniqueTID();
    SpawnForced("Boolean", 0,0,0, testTID);
    int useNoSight = cond(GetActorProperty(testTID, APROP_DamageType) == 0, false, true);
    Thing_Remove(testTID);
    
    if (useNoSight)
    {
        Score_TotalNums[0] = 0;
        Score_TotalNums[1] = 0;
        
        // This will have a lot of Score_Report scripts run
        SpawnForced("ScoreMonsterFinder", 0,0,0, testTID);
        SetActorState(testTID, "Find");
        Thing_Remove(testTID);
        
        totalMons   = Score_TotalNums[0];
        totalPoints = Score_TotalNums[1];
    }
    else
    {
        int monTypes    = ACS_NamedExecuteWithResult("Score_MonsterTypeCount");
        int testmonTID  = UniqueTID();

        for (i = 0; i < monTypes; i++)
        {
            str name  = ACS_NamedExecuteWithResult("Score_MonsterAtIndex", i);
            int value = ACS_NamedExecuteWithResult("Score_RewardAtIndex",  i);
            
            if (value < 0)
            {
                SpawnForced(name, 0,0,0, testTID);
                value = GetActorProperty(testTID, APROP_SpawnHealth);
                Thing_Remove(testTID);
            }
        
            int count = ThingCountName(name, 0);
            //Log(s:"\cd[", s:name, s:"] \cf(", d:value, s:")\c- Count: ", d:count);

            totalMons   += count;
            totalPoints += count * value;
        }
    }
    
    
    int averagePoints = 0;
    if (totalMons > 0) { averagePoints  = totalPoints / totalMons; }
    
    // Controls for low and high monster counts
    int fullHealMult = middle(P_FULLHEAL_MIN, FixedMul(totalMons, P_FULLHEAL_PERCENT), P_FULLHEAL_MAX);
    int fullHealPoints = averagePoints * fullHealMult;
    
    Score_Thresholds[ST_FULLHEAL] = max(5000, ((fullHealPoints + 2500) / 5000) * 5000);
    Score_Thresholds[ST_UT_KILLS] = middle(P_UTKILLS_MIN, totalMons   / 10, P_UTKILLS_MAX);
    Score_Thresholds[ST_UT_HP]    = middle(P_UTHP_MIN,    totalPoints / 10, P_UTHP_MAX);
    
    // Adjust everyone's base score to match the percentage from the last map if it's non-zero
    for (i = 0; i < PLAYERMAX; i++)
    {
        if (!PlayerInGame(i)) { continue; }
        int percent = Score_GetScorePercent(i);

        // We add 1 because FixedMul loves rounding down, and we don't actually want that!
        Score_SetScore(i, FixedMul(Score_Thresholds[ST_FULLHEAL], percent + 1));
    }
}


script "Score_Count" (void)
{
    str name   = GetActorClass(0);
    int points = ACS_NamedExecuteWithResult("Score_Lookup", name);
    if (points == -1) { points = GetActorProperty(0, APROP_SpawnHealth); }
    
    Score_TotalNums[0] += 1;
    Score_TotalNums[1] += points;
    //Log(s:"\cd[", s:name, s:"] \cf(", d:points, s:")");
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
