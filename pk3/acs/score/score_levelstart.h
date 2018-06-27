// 0 is monsters
// 1 is points
// 2 is "debug mode on"
int Score_TotalNums[3];

script "Score_InHub" (void)
{
    int i;
    
    for (i = 0; i < PLAYERMAX; i++)
    {
        SetActivator(0);
        ActivatorToPlayer(i);
        
        if (IsWorld()) { continue; }
        
        if (CheckInventory("HubTracker"))
        {
            SetResultValue(true);
            terminate;
        }
    }
    
    SetResultValue(false);
}

function void Score_CalcMapPoints(void)
{
    if (Score_Thresholds[ST_LASTSET] == Score_Thresholds[ST_WORLDTIMER] + 1) { return; }
    if (ACS_NamedExecuteWithResult("Score_InHub") && !GetCVar("dakka_ignorehubs")) { return; }

    int i;
    int debug       = GetCVar("dakka_debug_score");
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
        Score_TotalNums[2] = debug;

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
            
            if (debug)
            {
                Log(s:cond(count == 0, "\cu", ""), s:name, s:": ", d:count, s:cond(count == 0, "", "\cq"), s:" (", d:count * value, s:")");
            }
            
            totalMons   += count;
            totalPoints += count * value;
        }
    }


    int averagePoints = 0;
    if (totalMons > 0) { averagePoints  = totalPoints / totalMons; }
    
    int monsterMin    = GetCVar("dakka_score_monstermin");
    int monsterMax    = GetCVar("dakka_score_monstermax");
    int monsterScalar = itofDiv(GetCVar("dakka_score_monsterscalar"), 100);
    int scoreInterval = GetCVar("dakka_score_interval");

    // Controls for low and high monster counts
    int fullHealMult   = monsterScalar * middle(monsterMin, totalMons, monsterMax);
    int fullHealPoints = FixedMul(averagePoints, fullHealMult);

    Score_Thresholds[ST_LASTSET]  = Score_Thresholds[ST_WORLDTIMER] + 1;
    Score_Thresholds[ST_FULLHEAL] = max(scoreInterval, roundTo(fullHealPoints, scoreInterval));
    
    Score_Thresholds[ST_UT_KILLS] = middle(P_UTKILLS_MIN, totalMons   / 10, P_UTKILLS_MAX);
    Score_Thresholds[ST_UT_HP]    = middle(P_UTHP_MIN,    totalPoints / 10, P_UTHP_MAX);
    
    if (debug)
    {
        int mapMons = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
        str monColor;
        
        switch (sign(totalMons - mapMons))
        {
            case -1: monColor = "\ca"; break;
            case  0: monColor = "\cd"; break;
            case  1: monColor = "\ct"; break;
        }
        
        Log(s:"\nScore totals: ", d:totalPoints, s:" points, ", s:monColor, d:totalMons, s:"/", d:mapMons, s:"\c- monsters");
        Log(s:" Average points/monster: ", d:averagePoints);
        Log(s:" Point multiplier: ", f:fullHealMult);
        Log(s:" Target score: ", d:fullHealPoints, s:" (rounded to ", d:Score_Thresholds[ST_FULLHEAL], s:")");
    }
    
    
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
}



// This reads dakka_keepscore.
function void Score_Reset(int pln)
{
    int resetMask;

    switch (GetCVar("dakka_startmode_score"))
    {
      case 0:
      default:
        resetMask = SMASK_ALL;
        break;

      case 1:
        resetMask = SMASK_NOTDISPLAY;
        break;

      case 2:
        resetMask = SMASK_NOTDISPLAY & SMASK_NOTREWARDS;
        break;

      case 3:
        resetMask = 0;
        break;
    }

    Score_ResetMask(pln, resetMask);
    Score_SetLastReset(pln, Timer());
}


function void Score_ResetMask(int pln, int mask)
{
    if (mask & SMASK_SCORE)        { Score_SetScore(pln, 0); }
    if (mask & SMASK_DISPLAYSCORE) { Score_SetDisplayScore(pln, 0); }
    if (mask & SMASK_REWARDCOUNT)  { Score_SetRewardCount(pln, 0); }
    if (mask & SMASK_REGENTIMER)   { Score_SetRegenTimer(pln, 0); }
    if (mask & SMASK_REGENSPENT)   { Score_SetRegenSpent(pln, 0); }
    if (mask & SMASK_EXTRALIVES)   { Score_SetExtraLives(pln, 0); }

    // Never clear HASLIVES because that's kinda important for making
    //  Dakkaguy not BUDDHA at the start of the map.
}
