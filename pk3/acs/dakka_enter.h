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
    int oldScore, curScore;

    if (DakkaEnterLocks[pln]) { terminate; }
    DakkaEnterLocks[pln] = true;

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

        oldScore = curScore;
        curScore = PlayerMapScores[pln];

        // In score/score_display.h
        // TODO: Move this to clientside display,
        //       using the pickup client->server system
        Score_Update(pln);
        Delay(1);
    }
}


script DAKKA_DISCONNECT (int pln) disconnect
{
    DakkaEnterLocks[pln] = false;
}
