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

    int pln = PlayerNumber();

    int curScore, lastScore;

    int myLockVal = DakkaEnterLocks[pln] + 1;
    DakkaEnterLocks[pln] = myLockVal;

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
        Dakka_DoLevelSpawn(!respawned);
    }

    curScore = Score_GetScore(pln);

    while (DakkaEnterLocks[pln] == myLockVal)
    {
        lastScore = curScore;
        curScore  = Score_GetScore(pln);

        PlayerTIDs[pln] = defaultTID(-1);

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

        // In dakka_scrap.h
        Dakka_ProcessScrap();

        // In dakka_ssg.h
        SSG_AutoReload();

        // In score/score_reward.h
        Score_DoRewards(lastScore, curScore);

        // In score/score_update.h
        Air_UpdateZHeight();

        Score_UpdateClient(pln);

        Delay(1);
    }
}


script "Dakka_Return" return
{
    // I don't think return scripts will ever run in DM but just in case
    if (GameType() == GAME_NET_DEATHMATCH)
    {
        Dakka_DoDMSpawn(false);
    }
    else
    {
        Dakka_DoLevelSpawn(false);
    }
}


script "Dakka_Disconnect" (int pln) disconnect
{
    DakkaEnterLocks[pln] = false;
    Score_ResetMask(pln, REWARDMASK_ALL);
}
