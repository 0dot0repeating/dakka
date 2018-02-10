function void ClearPoints(int pln, int bonustime)
{
    int i;
    for (i = 0; i < BONUSCOUNT; i++)
    {
        if (LastBonus[pln][i] == bonustime)
        {
            BonusValues[pln][i] = 0;
        }
    }
}


// Point multipliers:
//
// EFFICIENCY:
// - Spree: the more dudes you kill in quick succession, the more points you get.
// - Untouchable: kill dudes without getting hit yourself for more points
// - Infighter: cause an infight, and everyone gets points
//
// STYLE:
// - Switcharoo: switch up your weapons and fire modes when fighting? get more points!
// - Air: kill enemies mid-flight, get more points
// - Curveball: kill an enemy without them being able to see you? get points.
// - Scrapping: scrapper kill
//
// DARING:
// - Brawler: use your fists to get kills
// - Redline: kill enemies with no more than 25 hp
// - Swan Song: kill enemies while dead (overrides bone-dry)
// - Point Blank: get close, get points

int TmpBonuses[BONUSCOUNT];

script "Dakka_Score" (int pointValue)
{
    int myhp      = GetActorProperty(0, APROP_SpawnHealth);
    int deathhp   = GetActorProperty(0, APROP_Health);
    int deathtype = GetActorProperty(0, APROP_DamageType);
    
    int i;
    if (pointValue <= 0) { pointValue = SMult_Base(); }

    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);

    int myRadius  = GetActorProperty(0, APROP_Radius);
    int myHeight  = GetActorProperty(0, APROP_Height);
    int myTID_old = ActivatorTID();
    int myTID     = defaultTID(-1);

    if (!SetActivatorToTarget(0))
    {
        Thing_ChangeTID(myTID, myTID_old);
        terminate;
    }

    int firerTID_old = ActivatorTID();
    int firerTID     = defaultTID(-1);
    int pln          = PlayerNumber();

    if (pln == -1)
    {
        // either telefragged or didn't have room to spawn via A_PainAttack
        if (myhp - deathhp == 1000000)
        {
            // zandronum doesn't support APROP_DamageType, and since monsters
            //  usually can't telefrag each other, assume it's A_PainAttack
            if (deathtype == 0 || !stricmp(deathtype, "None"))
            {
                Thing_ChangeTID(myTID,    myTID_old);
                Thing_ChangeTID(firerTID, firerTID_old);
                terminate;
            }
        }
        
        if ((ClassifyActor(0) & ACTOR_MONSTER) && (GetActorProperty(0, APROP_Friendly) == 0))
        {
            ACS_NamedExecuteWithResult("Dakka_Infighter", pointValue);
        }

        Thing_ChangeTID(myTID,    myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        terminate;
    }

    if (GetUserCVar(pln, "dakka_cl_noscore"))
    {
        Thing_ChangeTID(myTID,    myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        terminate;
    }

    int plX      = GetActorX(0);
    int plY      = GetActorY(0);
    int plZ      = GetActorZ(0);
    int plRadius = GetActorProperty(0, APROP_Radius);
    int plHeight = GetActorProperty(0, APROP_Height);

    int curveCheck = CheckSight(myTID, firerTID, 0);

    SetActivator(myTID);

    Warp(0, myX, myY, myZ + GetActorProperty(0, APROP_Height) / 2, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION);
    curveCheck = curveCheck || CheckSight(myTID, firerTID, 0);

    Warp(0, myX, myY, myZ + GetActorProperty(0, APROP_Height), 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION);
    curveCheck = curveCheck || CheckSight(myTID, firerTID, 0);

    Warp(0, myX, myY, myZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION);
    SetActivator(firerTID);

    Thing_ChangeTID(myTID,    myTID_old);
    Thing_ChangeTID(firerTID, firerTID_old);

    int points_switcharoo   = round(pointValue * SMult_WeaponSwitch(pln, myhp));
    int points_killstreak   = round(pointValue * SMult_Killstreak(pln));
    int points_untouchable  = round(pointValue * SMult_Untouchable(pln, myhp));
    int points_bonedry      = round(pointValue * SMult_BoneDry());
    int points_soreloser    = round(pointValue * SMult_SoreLoser());
    int points_brawler      = round(pointValue * SMult_Brawler());
    int points_air          = round(pointValue * SMult_Air(pln));
    int points_curveball    = round(pointValue * SMult_Curveball(curveCheck));
    int points_scrapping    = round(pointValue * SMult_Scrapping(pln));
    int points_pointblank   = round(pointValue * SMult_PointBlank(myX, myY, myZ, myRadius, myHeight, plX, plY, plZ, plRadius, plHeight));

    Score_ModBothScores(pln, pointValue);
    Score_ModBothScores(pln, points_switcharoo);
    Score_ModBothScores(pln, points_killstreak);
    Score_ModBothScores(pln, points_untouchable);
    Score_ModBothScores(pln, points_bonedry);
    Score_ModBothScores(pln, points_soreloser);
    Score_ModBothScores(pln, points_brawler);
    Score_ModBothScores(pln, points_air);
    Score_ModBothScores(pln, points_curveball);
    Score_ModBothScores(pln, points_scrapping);
    Score_ModBothScores(pln, points_pointblank);

    int bonustime = Timer();

    TmpBonuses[BS_BASE]         = pointValue;
    TmpBonuses[BS_SPREE]        = points_killstreak;
    TmpBonuses[BS_UNTOUCHABLE]  = points_untouchable;
    TmpBonuses[BS_INFIGHTER]    = 0;
    TmpBonuses[BS_SWITCHAROO]   = points_switcharoo;
    TmpBonuses[BS_BONEDRY]      = points_bonedry;
    TmpBonuses[BS_SORELOSER]    = points_soreloser;
    TmpBonuses[BS_BRAWLER]      = points_brawler;
    TmpBonuses[BS_AIR]          = points_air;
    TmpBonuses[BS_CURVEBALL]    = points_curveball;
    TmpBonuses[BS_SCRAPPING]    = points_scrapping;
    TmpBonuses[BS_POINTBLANK]   = points_pointblank;

    for (i = 0; i < BONUSCOUNT; i++)
    {
        if (TmpBonuses[i] > 0)
        {
            BonusValues[pln][i] += TmpBonuses[i];
            LastBonus[pln][i]    = bonustime;
        }
    }

    AddUntouchable(pln, myhp);
    AddKillstreak(pln, myhp);
    Delay(35);

    ClearPoints(pln, bonustime);
}

// monster got the kill
script "Dakka_Infighter" (int pointValue)
{
    int points_base      = pointValue;
    int points_infighter = round(pointValue * SMult_Infighter());
    int bonustime = Timer();

    int i;

    for (i = 0; i < PLAYERMAX; i++)
    {
        if (!PlayerInGame(i) || GetUserCVar(i, "dakka_cl_noscore")) { continue; }

        Score_ModBothScores(i, points_base);
        Score_ModBothScores(i, points_infighter);

        BonusValues[i][BS_BASE]       += points_base;
        BonusValues[i][BS_INFIGHTER]  += points_infighter;

        LastBonus[i][BS_BASE]       = bonustime;
        LastBonus[i][BS_INFIGHTER]  = bonustime;
    }

    Delay(35);

    for (i = 0; i < PLAYERMAX; i++)
    {
        ClearPoints(i, bonustime);
    }
}
