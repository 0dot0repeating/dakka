script DAKKA_ENTER enter
{
    ACS_ExecuteWithResult(DAKKA_SPAWN, false);
}


script DAKKA_RESPAWN respawn
{
    ACS_ExecuteWithResult(DAKKA_SPAWN, true);
}



script DAKKA_SPAWN (int respawned)
{
    int pln = PlayerNumber();
    int oldScore, curScore;

    while (true)
    {
        // This is in dakka_tempitems.h
        Dakka_UpdateTemporaryItems();

        // In dakka_scrap.h
        Dakka_ProcessScrap();

        oldScore = curScore;
        curScore = PlayerMapScores[pln];

        // In score/score_display.h
        // TODO: Move this to clientside display,
        //       using the pickup client->server system
        Score_Update(pln);
        Delay(1);
    }
}
