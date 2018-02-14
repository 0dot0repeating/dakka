script "Dakka_Enter" enter
{
    ACS_NamedExecuteWithResult("Dakka_Spawn", false);
}


script "Dakka_Respawn" respawn
{
    ACS_NamedExecuteWithResult("Dakka_Spawn", true);
}


int DakkaEnterLocks[PLAYERMAX];

script "Dakka_Spawn" (int respawned)
{
    if (GameType() == GAME_TITLE_MAP) { terminate; }

    // Apparently ENTER can run before OPEN in plain ol ZDoom too - fuckin fantastic
    // In score/score_levelstart.h
    Score_CalcMapPoints();

    int pln      = PlayerNumber();
    int classNum = Pickup_ClassNumber(0);

    int curScore, lastScore;

    int myLockVal = DakkaEnterLocks[pln] + 1;
    DakkaEnterLocks[pln] = myLockVal;

    int bfgPtrTID = 0;

    // DM does its own level start, since it needs to do things every spawn,
    //  and variables like dakka_startmode don't apply.
    //
    // Cooperative does standard level starts.
    if (GameType() == GAME_NET_DEATHMATCH)
    {
        // These functions expect 'entered', not 'respawned'
        Dakka_DoDMSpawn(!respawned);
    }
    else
    {
        Dakka_DoLevelSpawn(!respawned, false);
    }

    curScore = Score_GetScore(pln);

    while (DakkaEnterLocks[pln] == myLockVal)
    {
        // Doesn't need its own script
        if (classNum == Cl_Dakkaguy) { SetInventory("DWep_Fist", 1); }

        // In score/score_reward.h
        lastScore = CheckInventory("DakkaScoreItem");
        curScore  = Score_GetScore(pln);
        Score_DoRewards(lastScore, curScore);
        SetInventory("DakkaScoreItem", curScore);

        // In score/score_update.h
        Air_UpdateZHeight();


        // Handle the dakka_shotgun2ssg and dakka_chaingun2minigun CVars.
        // - In dakka_gunstoguns.h
        Dakka_GunsToGuns();

        // idfa and idkfa give 'fake' blue armor. Let's make it real blue armor.
        // - In dakka_idfa.h
        Dakka_ProcessIDFAArmor();

        // Translate guns meant for other classes
        // - In dakka_translateguns.h
        Dakka_TranslateGuns();

        // In dakka_powerlevel.h
        Dakka_PowerLevel();

        // In dakka_tempitems.h
        Dakka_UpdateTemporaryItems();

        // In dakka_soundloopers.h
        Dakka_SoundLoopers();

        // In dakka_vampirism.h
        Dakka_VampireTick();

        // In dakka_fists.h
        Dakka_FistTap();

        // In dakka_scrap.h
        Dakka_ProcessScrap();

        // In dakka_pistols.h
        Pistols_SpreadRecovery();
        Pistols_CapBulletsLeft();
        
        // In dakka_ssg.h and dakka_bfg.h
        SSG_AutoReload();
        BFG_AutoReload();

        // In dakka_bfg.h
        bfgPtrTID = Dakka_GetNewTarget(bfgPtrTID);

        Score_UpdateClient(pln);

        Delay(1);
    }
}


script "Dakka_Return" return
{
    // Gotta do this so that dakka_ignorehubs works as expected
    Score_CalcMapPoints();
    
    // I don't think return scripts will ever run in DM but just in case
    if (GameType() == GAME_NET_DEATHMATCH)
    {
        Dakka_DoDMSpawn(false);
    }
    else
    {
        Dakka_DoLevelSpawn(true, true);
    }
}


script "Dakka_Disconnect" (int pln) disconnect
{
    DakkaEnterLocks[pln] = false;
    Score_ResetMask(pln, REWARDMASK_ALL);
    
    for (int i = 0; i < SLOOP_COUNT; i++)
    {
        Thing_Remove(SoundLooperTIDs[pln][i]);
        SoundLooperTIDs[pln][i] = 0;
    }
}
