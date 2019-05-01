// OPEN scripts run in the opposite order they were defined.
// except sometimes they don't?
// yeah I don't fucking know, it's dumb and I hate it

script "Dakka_Open_Client" open clientside
{
    if (IsServer || GameType() == GAME_TITLE_MAP || GameType() == GAME_SINGLE_PLAYER) { terminate; }

    int cpln = ConsolePlayerNumber();
    IsServer = true;

    //CSender_ForceSendAll(cpln);
    
    int monmin   = GetCVar("dakka_score_monstermin");
    int monmax   = GetCVar("dakka_score_monstermax");
    int monscale = GetCVar("dakka_score_monsterscalar");
    int interval = GetCVar("dakka_score_interval");
    int rewards  = GetCVar("dakka_score_rewardtypes");
    
    str message = "";
    int harderOrEasier = 0;
    
    if (monmin != 140 || monmax != 800 || monscale != 50)
    {
        int moncount    = GetLevelInfo(LEVELINFO_TOTAL_MONSTERS);
        
        int serverCount  = middle(monmin, moncount, monmax);
        int defaultCount = middle(140,    moncount, 800);
        
        int serverScalar  = FixedMul(serverCount, itofDiv(monscale, 100));
        int defaultScalar = FixedMul(defaultCount, 0.5);
        
        harderOrEasier = sign(serverScalar - defaultScalar);
        
        if (monmin != 140)
        {
            message = StrParam(s:"- The minimum monster count has been ", s:cond(monmin > 140, "\caraised\c-", "\cdlowered\c-"), s:" to ", d:monmin, s:" for scoring purposes.\n");
        }
        
        if (monmax != 800)
        {
            message = StrParam(s:message, s:"- The maximum monster count has been ", s:cond(monmax > 800, "\caraised\c-", "\cdlowered\c-"), s:" to ", d:monmax, s:" for scoring purposes.\n");
        }
        
        if (monscale != 50)
        {
            message = StrParam(s:message, s:"- The monster count scalar has been ", s:cond(monscale > 50, "\caraised\c-", "\cdlowered\c-"), s:" to ", d:monscale, s:"%.\n");
        }
    }
    
    if (interval != 5000)
    {
        message = StrParam(s:message, s:"- Rewards fall on multiples of ", d:interval, s:".\n");
    }
    
    switch (rewards)
    {
      default:
        break;
      
      case 1:
        message = StrParam(s:message, s:"- Your first reward is an extra life, rather than ammo regen.\n");
        break;
      
      case 2:
        message = StrParam(s:message, s:"- Extra lives are disabled.\n");
        break;
      
      case 3:
        message = StrParam(s:message, s:"- Ammo regen is disabled.\n");
        break;
      
      case 4:
        message = StrParam(s:message, s:"- Score rewards are disabled.\n");
        break;
    }
    
    if (!stringBlank(message))
    {
        SetHudSize(480, 360, true);
        HudMessage(s:"\nChanges to scoring:\n\n", s:message;
            HUDMSG_FADEOUT | HUDMSG_LOG, -19293, CR_WHITE, 240.4, 80.0, 4.0, 0.5);
        
        HudMessage(s:"This server changes how scores work (details in console).";
            HUDMSG_FADEOUT, -19293, CR_WHITE, 240.4, 80.0, 3.0, 0.5);
            
        switch (harderOrEasier)
        {
          case -1:
            HudMessage(s:"(You'll need \cdfewer\c- points than normal)";
                HUDMSG_FADEOUT, -19294, CR_WHITE, 240.4, 94.0, 3.0, 0.5);
            break;
            
          case 1:
            HudMessage(s:"(You'll need \camore\c- points than normal)";
                HUDMSG_FADEOUT, -19294, CR_WHITE, 240.4, 94.0, 3.0, 0.5);
            break;
        }
    }

    while (true)
    {
        // In score/score_display.h
        Score_UpdateDisplay(cpln);

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



#define PSYNC_INGAME    0
#define PSYNC_INSERVER  1
#define PSYNC_SYNCTO    2
#define PSYNC_NEEDSYNC  3

int PlayerSync[4][PLAYERMAX];

script "Dakka_Open" open
{
    if (GameType() == GAME_TITLE_MAP) { terminate; }

    // Absorbed this from the PICKUP_OPEN script.
    IsServer = true;
    Pickup_IsZandronum(); // side effect sets IsZandronum

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
                
                // Looking at this after the fact, I think it's done this way so
                //  that when everyone joins as a map starts, they don't all sync
                //  to each other unnecessarily.
                if (PlayerSync[PSYNC_INGAME][i] && inGame)
                {
                    PlayerSync[PSYNC_SYNCTO][toSyncTo_count++] = i;
                }
                
                // And this is a straightforward "did they just join the server" check.
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
                        int pln_to = PlayerSync[PSYNC_SYNCTO][j];
                        ACS_NamedExecuteWithResult("Dakka_SoundLooper_FollowSync", pln_to, pln_for);
                    }
                }
            }
        }
        
        // Absorbed from PICKUP_OPEN
        Sender_UpdateClients();
        Sender_ActuallySend();
        Delay(1);
        
        Score_Thresholds[ST_WORLDTIMER]++;
    }
}