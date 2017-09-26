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
            if (!(PlayerInGame(i) || PlayerIsSpectator(i))) { continue; }
            
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
    int oldPickupState;
    int pickupState = -1;

    CSender_ForceSendAll(cpln);

    while (true)
    {
        // In score/score_display.h
        Score_Update(cpln);
        Score_UpdateLives(cpln);

        // Debug shit
        /*
        int time = Timer();
        int i;

        SetHudSize(640, 480, 1);
        SetFont("CONFONT");
        for (i = 0; i < S2C_DATACOUNT; i++)
        {
            HudMessage(d:i, s:": ", d:SToC_ClientData[cpln][i];
                HUDMSG_PLAIN, 12000 + (i * 4), CR_WHITE, 560.1, 60.0 + (12.0 * i), 0.25);

            if (SToC_LastReceiveTime[cpln][i] - 1 == time)
            {
                HudMessage(s:"Ping!";
                    HUDMSG_FADEOUT, 12001 + (i * 4), CR_GREEN,     550.2, 60.0 + (12.0 * i), 0.0, 1.0);
                HudMessage(s:"Ping!";
                    HUDMSG_PLAIN,   12002 + (i * 4), CR_DARKGRAY, 550.2, 60.0 + (12.0 * i), 1.0);
            }
        }
        */

        CSender_SetData(cpln, C2S_D_NOSCOREREWARDS, GetUserCVar(cpln, "dakka_cl_noscorerewards"));
        CSender_SetData(cpln, C2S_D_NOSCORE,        GetUserCVar(cpln, "dakka_cl_noscore"));

        CSender_UpdateServer(cpln);
        CSender_ActuallySend(cpln);

        Delay(1);
    }
}
