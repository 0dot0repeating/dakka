// This file holds the main loop responsible for making sure clients get
//  all the information they should have.


script PICKUP_MAINLOOP open
{
    // We're just interested in the side effect.
    Pickup_IsZandronum();
    IsServer = true;

    while (true)
    {
        Pickup_UpdateClients();
        Pickup_ActuallySend();
        Delay(1);
    }
}

script PICKUP_ENTER enter
{
    int pln = PlayerNumber();
    Pickup_ForceUpdateClient(pln);

    while (true)
    {
        Pickup_SetData(pln, S2C_D_HEALTH,       GetActorProperty(0, APROP_Health));
        Pickup_SetData(pln, S2C_D_CLASSNUM,     Pickup_ClassNumber(0));
        Pickup_SetData(pln, S2C_D_CLIP,         CheckInventory("Clip"));
        Pickup_SetData(pln, S2C_D_SHELL,        CheckInventory("Shell"));
        Pickup_SetData(pln, S2C_D_ROCKET,       CheckInventory("RocketAmmo"));
        Pickup_SetData(pln, S2C_D_CELL,         CheckInventory("Cell"));
        Delay(1);
    }
}

script PICKUP_DISCONNECT (int pln) disconnect
{
    Pickup_ClearData(pln);
    Pickup_ForceUpdateClient(pln);
}



script PICKUP_OPEN_CLIENT open clientside
{
    int cpln = ConsolePlayerNumber();
    int oldPickupState;
    int pickupState = -1;

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

        if ((pickupState != oldPickupState) || (time % 350 == 0))
        {
            RequestScriptPuke(PICKUP_WEAPONSWITCH, pickupState,0,0);
        }

        Delay(1);
    }
}


script PICKUP_WEAPONSWITCH (int pickupState) net
{
    int pln = PlayerNumber();
    C2S_WeaponSwitchState[pln] = pickupState;
}
