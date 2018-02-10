#define PSYNC_INGAME    0
#define PSYNC_INSERVER  1
#define PSYNC_SYNCTO    2
#define PSYNC_NEEDSYNC  3

int PlayerSync[4][PLAYERMAX];

script "Dakka_Open" open
{
    if (GameType() == GAME_TITLE_MAP) { terminate; }

    // Absorbed this from the PICKUP_OPEN script.
    Pickup_IsZandronum(); // side effect sets IsZandronum
    IsServer = true;

    // In score/score_levelstart.h
    Score_CalcMapPoints();

    int toSyncTo_count;
    int needSync_count;
    
    int i, j;
    
    while (true)
    {
        if (ConsolePlayerNumber() == -1)
        {
            toSyncTo_count = 0;
            needSync_count = 0;
            
            for (i = 0; i < PLAYERMAX; i++)
            {
                int inGame   = PlayerInGame(i);
                int inServer = inGame || PlayerIsSpectator(i);
                
                if (PlayerSync[PSYNC_INGAME][i] && inGame)
                {
                    PlayerSync[PSYNC_SYNCTO][toSyncTo_count++] = i;
                }
                
                if (inServer && !PlayerSync[PSYNC_INSERVER][i])
                {
                    PlayerSync[PSYNC_NEEDSYNC][needSync_count++] = i;
                }
                
                PlayerSync[PSYNC_INGAME][i]   = inGame;
                PlayerSync[PSYNC_INSERVER][i] = inServer;
            }
            
            if (toSyncTo_count > 0)
            {
                for (i = 0; i < needSync_count; i++)
                {
                    int pln_for = PlayerSync[PSYNC_NEEDSYNC][i];
                    
                    for (j = 0; j < toSyncTo_count; j++)
                    {
                        int pln_to = PlayerSync[PSYNC_SYNCTO][i];
                        ACS_NamedExecuteWithResult("Dakka_SoundLooper_FollowSync", pln_to, pln_for);
                    }
                }
            }
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

    //CSender_ForceSendAll(cpln);

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

        CSender_SetData(cpln, C2S_D_NOSCOREREWARDS, GetUserCVar(cpln, "dakka_cl_noscorerewards"));
        CSender_SetData(cpln, C2S_D_NOSCORE,        GetUserCVar(cpln, "dakka_cl_noscore"));

        CSender_UpdateServer(cpln);
        CSender_ActuallySend(cpln);
        */

        Delay(1);
    }
}
