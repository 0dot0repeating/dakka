function void ClearPoints(int pln, int bonustime)
{
    int i;
    for (i = 0; i < BONUSCOUNT; i++)
    {
        if (LastBonus[pln][i] == bonustime || bonustime == -1)
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
// - Darwin: get an enemy to kill itself
// - Telefragged: doesn't really work on zandronum
//
// DARING:
// - Brawler: use your fists to get kills
// - Redline: kill enemies with no more than 25 hp
// - Swan Song: kill enemies while dead (overrides bone-dry)
// - Point Blank: get close, get points

int TmpBonuses[BONUSCOUNT];

script "Dakka_Score" (int pointValue, int damagetype)
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
    int myTID     = UniqueTID();
    Thing_ChangeTID(0, myTID);

    if (!SetActivatorToTarget(0))
    {
        Thing_ChangeTID(myTID, myTID_old);
        terminate;
    }

    int firerTID_old = ActivatorTID();
    int firerTID     = UniqueTID();
    Thing_ChangeTID(0, firerTID);
    
    if (firerTID_old == myTID)
    {
        ACS_NamedExecuteWithResult("Dakka_InfighterSelf", pointValue);
        Thing_ChangeTID(myTID,    myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        terminate;
    }
    
    int pln = PlayerNumber();
    
    // m-m-m-m-monster kill
    if (pln == -1)
    {
        // zandronum doesn't support APROP_DamageType, and since monsters
        //  usually can't telefrag each other, assume it's A_PainAttack
        if ((myhp - deathhp == 1000000) && (deathtype == 0 || !stricmp(deathtype, "None")))
        {
            Thing_ChangeTID(myTID,    myTID_old);
            Thing_ChangeTID(firerTID, firerTID_old);
            terminate;
        }
        
        // infighting
        if ((ClassifyActor(0) & ACTOR_MONSTER) && (GetActorProperty(0, APROP_Friendly) == 0))
        {
            ACS_NamedExecuteWithResult("Dakka_Infighter", pointValue);
        }

        Thing_ChangeTID(myTID,    myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        terminate;
    }
    
    int plX      = GetActorX(0);
    int plY      = GetActorY(0);
    int plZ      = GetActorZ(0);
    int plRadius = GetActorProperty(0, APROP_Radius);
    int plHeight = GetActorProperty(0, APROP_Height);
    
    int curveCheck = true, telefragged = false;
    
    // check location in zandronum - it only really works on map load, but that's good enough
    // check damage type in gzdoom - this actually works, wow
    if (deathtype == 0)
    {
        telefragged = (myhp - deathhp >= 1000000)
                   && (plX - plRadius < myX + myRadius) && (plX + plRadius > myX - myRadius)
                   && (plY - plRadius < myY + myRadius) && (plY + plRadius > myY - myRadius)
                   && (plZ            < myZ + myHeight) && (plZ + plHeight > myZ);
    }
    else 
    {
        telefragged = !stricmp(deathtype, "Telefrag");
    }
    
    if (!telefragged)
    {
        curveCheck = CheckSight(myTID, firerTID, 0);

        SetActivator(myTID);

        Warp(0, myX, myY, myZ + GetActorProperty(0, APROP_Height) / 2, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION);
        curveCheck |= CheckSight(myTID, firerTID, 0);

        Warp(0, myX, myY, myZ + GetActorProperty(0, APROP_Height), 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION);
        curveCheck |= CheckSight(myTID, firerTID, 0);

        Warp(0, myX, myY, myZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION);
        SetActivator(firerTID);
    }

    Thing_ChangeTID(myTID,    myTID_old);
    Thing_ChangeTID(firerTID, firerTID_old);

    int points_switcharoo   = oldRound(pointValue * SMult_WeaponSwitch(pln, myhp));
    int points_killstreak   = oldRound(pointValue * SMult_Killstreak());
    int points_untouchable  = oldRound(pointValue * SMult_Untouchable(myhp));
    int points_bonedry      = oldRound(pointValue * SMult_BoneDry());
    int points_soreloser    = oldRound(pointValue * SMult_SoreLoser());
    int points_brawler      = oldRound(pointValue * SMult_Brawler(damagetype));
    int points_air          = oldRound(pointValue * SMult_Air(pln));
    int points_curveball    = oldRound(pointValue * SMult_Curveball(curveCheck));
    int points_scrapping    = oldRound(pointValue * SMult_Scrapping(pln, damagetype));
    int points_pointblank   = oldRound(pointValue * SMult_PointBlank(myX, myY, myZ, myRadius, myHeight, plX, plY, plZ, plRadius, plHeight));
    int points_telefrag     = oldRound(pointValue * SMult_Telefrag(telefragged));

    Score_ModBothScores(pln, pointValue
                           + points_switcharoo
                           + points_killstreak
                           + points_untouchable
                           + points_bonedry
                           + points_soreloser
                           + points_brawler
                           + points_air
                           + points_curveball
                           + points_scrapping
                           + points_pointblank
                           + points_telefrag);

    int bonustime = Timer();

    TmpBonuses[BS_BASE]         = pointValue;
    TmpBonuses[BS_SPREE]        = points_killstreak;
    TmpBonuses[BS_UNTOUCHABLE]  = points_untouchable;
    TmpBonuses[BS_INFIGHTER]    = 0;
    TmpBonuses[BS_SWITCHAROO]   = points_switcharoo;
    TmpBonuses[BS_BONEDRY]      = points_bonedry;
    TmpBonuses[BS_SORELOSER]    = points_soreloser;
    TmpBonuses[BS_BRAWLER]      = points_brawler;
    TmpBonuses[BS_DARWIN]       = 0;
    TmpBonuses[BS_AIR]          = points_air;
    TmpBonuses[BS_CURVEBALL]    = points_curveball;
    TmpBonuses[BS_SCRAPPING]    = points_scrapping;
    TmpBonuses[BS_POINTBLANK]   = points_pointblank;
    TmpBonuses[BS_TELEFRAG]     = points_telefrag;

    for (i = 0; i < BONUSCOUNT; i++)
    {
        if (TmpBonuses[i] > 0)
        {
            BonusValues[pln][i] += TmpBonuses[i];
            LastBonus[pln][i]    = bonustime;
        }
    }

    AddUntouchable(myhp);
    AddKillstreak(myhp);
    Delay(35);

    ClearPoints(pln, bonustime);
}

// monster got the kill
script "Dakka_Infighter" (int pointValue)
{
    int points_base      = pointValue;
    int points_infighter = oldRound(pointValue * SMult_Infighter());
    int bonustime = Timer();

    int i;

    for (i = 0; i < PLAYERMAX; i++)
    {
        if (!PlayerInGame(i)) { continue; }

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

// monster nuked himself like an idiot
script "Dakka_InfighterSelf" (int pointValue)
{
    int points_base      = pointValue;
    int points_infighter = oldRound(pointValue * SMult_Darwin());
    int bonustime = Timer();

    int i;

    for (i = 0; i < PLAYERMAX; i++)
    {
        if (!PlayerInGame(i)) { continue; }

        Score_ModBothScores(i, points_base);
        Score_ModBothScores(i, points_infighter);

        BonusValues[i][BS_BASE]     += points_base;
        BonusValues[i][BS_DARWIN]   += points_infighter;

        LastBonus[i][BS_BASE]        = bonustime;
        LastBonus[i][BS_DARWIN]      = bonustime;
    }

    Delay(35);

    for (i = 0; i < PLAYERMAX; i++)
    {
        ClearPoints(i, bonustime);
    }
}