function void Score_CalcMapPoints(void)
{
    if (MapStart_FullHealPoints > 0) { return; }

    int i;
    int name, count;

    int totalMons   = 0;
    int totalPoints = 0;

    for (i = 0; i < MONTYPES; i++)
    {
        name = Monster_KnownMons[i];
        count = ThingCountName(name, 0);

        totalMons   += count;
        totalPoints += count * Monster_Points[i];
    }

    int averagePoints;

    if (totalMons == 0) { averagePoints = 0; }
    else { averagePoints  = totalPoints / totalMons; }

    
    int fullHealMult = middle(P_FULLHEAL_MIN, FixedMul(totalMons, P_FULLHEAL_PERCENT), P_FULLHEAL_MAX);

    int fullHealPoints = averagePoints * fullHealMult;
    fullHealPoints = ((fullHealPoints + 2500) / 5000) * 5000; // Round to nearest 5000

    MapStart_FullHealPoints = max(5000, fullHealPoints);

    // Adjust everyone's base score to match the percentage from the last map if it's non-zero

    Log(s:"threshold: ", d:MapStart_FullHealPoints);
    
    for (i = 0; i < PLAYERMAX; i++)
    {
        if (!PlayerInGame(i)) { continue; }
        int percent = Score_GetScorePercent(i);

        // We add 1 because FixedMul loves rounding down, and we don't actually want that!
        Log(s:"player ", d:i, s:" going from ", d:Score_GetScore(i), s:" to ", d:FixedMul(MapStart_FullHealPoints, percent + 1));
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
