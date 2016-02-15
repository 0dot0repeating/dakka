script DAKKA_ENTER enter
{
    ACS_ExecuteWithResult(DAKKA_SPAWN, false);
}


script DAKKA_RESPAWN respawn
{
    ACS_ExecuteWithResult(DAKKA_SPAWN, true);
}


int DakkaEnterLocks[PLAYERMAX];

script DAKKA_SPAWN (int respawned)
{
    int pln = PlayerNumber();
    int curScore;

    // DM does its own level start, since it needs to do things every spawn,
    //  and variables like dakka_startmode don't apply.
    if (GameType() == GAME_NET_DEATHMATCH)
    {
        // These functions expect 'entered', not 'respawned'
        Dakka_DoDMSpawn(!respawned);
    }

    if (DakkaEnterLocks[pln]) { terminate; }
    DakkaEnterLocks[pln] = true;

    // Everything past this point will only be run by one script per player.

    // Cooperative does standard level starts.
    if (GameType() != GAME_NET_DEATHMATCH)
    {
        // These functions expect 'entered', not 'respawned'
        Dakka_DoLevelSpawn(!respawned);
    }

    while (true)
    {
        PlayerTIDs[pln] = defaultTID(-1);

        // idfa and idkfa give 'fake' blue armor. Let's make it real blue armor.
        Dakka_ProcessIDFAArmor();

        // This is in dakka_tempitems.h
        Dakka_UpdateTemporaryItems();

        // In dakka_scrap.h
        Dakka_ProcessScrap();

        // In dakka_ssg.h
        SSG_AutoReload();

        // In score/score_update.h
        Air_UpdateZHeight();
        Score_UpdateClient(pln);

        Delay(1);
    }

    DakkaEnterLocks[pln] = false;
}


script DAKKA_DISCONNECT (int pln) disconnect
{
    DakkaEnterLocks[pln] = false;
}
