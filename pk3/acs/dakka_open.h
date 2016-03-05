script "Dakka_Open" open
{
    if (GameType() == GAME_TITLE_MAP) { terminate; }

    // Absorbed this from the PICKUP_OPEN script.
    Pickup_IsZandronum(); // side effect sets IsZandronum
    IsServer = true;

    Score_CalcMapPoints();

    while (true)
    {
        // Update CVars for everyone
        
        int i;

        for (i = 0; i < PLAYERMAX; i++)
        {
            // Update CVars for players
            Sender_SetData(i, S2C_D_BUNDLEPLASMA, GetCVar("dakka_bundleplasma") == 1);
            Sender_SetData(i, S2C_D_BUNDLEBFG,    GetCVar("dakka_bundlebfg")    == 1);
        }

        // Absorbed from PICKUP_OPEN
        Sender_UpdateClients();
        Sender_ActuallySend();
        Delay(1);
    }
}


script "Dakka_Open_Client" open clientside
{
    if (GameType() == GAME_TITLE_MAP) { terminate; }

    int cpln = ConsolePlayerNumber();

    while (true)
    {
        // In score/score_display.h
        Score_Update(cpln);
        Delay(1);
    }
}
