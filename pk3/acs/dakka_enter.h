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
        Dakka_DoDMStart();
    }

    if (DakkaEnterLocks[pln]) { terminate; }
    DakkaEnterLocks[pln] = true;

    // Everything past this point will only be run by one script per player.

    // Cooperative does standard level starts.
    if (GameType() != GAME_NET_DEATHMATCH)
    {
        Dakka_DoLevelStart();
    }

    while (true)
    {
        PlayerTIDs[pln] = defaultTID(-1);

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
}


script DAKKA_DISCONNECT (int pln) disconnect
{
    DakkaEnterLocks[pln] = false;
}
