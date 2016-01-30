// This file holds the main loop responsible for making sure clients get
//  all the information they should have.


script PICKUP_MAINLOOP open
{
    // We're just interested in the side effect.
    Pickup_IsZandronum();
    IsServer = true;

    while (true)
    {
        Sender_UpdateClients();
        Sender_ActuallySend();
        Delay(1);
    }
}

script PICKUP_ENTER enter
{
    int pln = PlayerNumber();
    int i;
    Sender_ForceUpdateClient(pln);

    while (true)
    {
        Sender_SetData(pln, S2C_D_HEALTH,       GetActorProperty(0, APROP_Health));
        Sender_SetData(pln, S2C_D_CLASSNUM,     Pickup_ClassNumber(0));
        Sender_SetData(pln, S2C_D_CLIP,         CheckInventory("Clip"));
        Sender_SetData(pln, S2C_D_SHELL,        CheckInventory("Shell"));
        Sender_SetData(pln, S2C_D_ROCKET,       CheckInventory("RocketAmmo"));
        Sender_SetData(pln, S2C_D_CELL,         CheckInventory("Cell"));

// Debug shit
/*
        int time = Timer();

        SetHudSize(640, 480, 1);
        SetFont("CONFONT");

        HudMessage(s:"T:", d:Timer(); HUDMSG_PLAIN, 13999, CR_YELLOW, 60.1, 48.0, 0.25);

        for (i = 0; i < C2S_DATACOUNT; i++)
        {
            HudMessage(d:i, s:": ", d:CToS_ServerData[pln][i];
                HUDMSG_PLAIN, 14000 + (i * 4), CR_WHITE, 60.1, 60.0 + (12.0 * i), 0.25);

            if (CToS_LastReceiveTime[pln][i] - 1 == time)
            {
                HudMessage(s:"Ping!";
                    HUDMSG_FADEOUT, 14001 + (i * 4), CR_GREEN,     50.2, 60.0 + (12.0 * i), 0.0, 1.0);
                HudMessage(s:"Ping!";
                    HUDMSG_PLAIN,   14002 + (i * 4), CR_DARKGRAY, 50.2, 60.0 + (12.0 * i), 1.0);
            }
        }
// */
        Delay(1);
    }
}

script PICKUP_DISCONNECT (int pln) disconnect
{
    Sender_ClearData(pln);
    Sender_ForceUpdateClient(pln);
}



script PICKUP_OPEN_CLIENT open clientside
{
    int cpln = ConsolePlayerNumber();
    int oldPickupState;
    int pickupState = -1;

    CSender_ForceSendAll(cpln);

    while (true)
    {
        int time = Timer();
        int i;

// Debug shit, comment out whenever
/*
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
// */
        oldPickupState = pickupState;

        if (Pickup_IsZandronum())
        {
            pickupState = GetCVar("switchonpickup");
        }
        else
        {
            pickupState = cond(GetCvar("neverswitchonpickup"), 0, 2);
        }

        CSender_SetData(cpln, C2S_D_SWITCHONPICKUP, pickupState);

        CSender_UpdateServer(cpln);
        CSender_ActuallySend(cpln);

        Delay(1);
    }
}
