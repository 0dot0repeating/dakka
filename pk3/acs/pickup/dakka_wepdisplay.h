// This is for the plasma rifle with Dakkaguy. If he started the map with a
//  plasma rifle, he gets a channeler instead.

#define WEPDISP_TYPES   1


#define WDSTATE_NORMAL      0
#define WDSTATE_SECONDWEP   1

int WepDisplay_States[WEPDISP_TYPES][2] =
{
    {"Dakka", "SuperShotgun"},
};

int WepDisplay_ReturnIndex[WEPDISP_TYPES][2] =
{
    {It_Shotgun,        It_SuperShotgun},
};


int WepDisplay_S2CCheck[WEPDISP_TYPES] =
{
    S2C_D_SHOT2SSG
};


// Array lookups are surprisingly expensive. Let's not do them if we don't have to.
//
// Restructuring this script to do as few lookups as possible cut the instruction
//  count for this down from 170-ish to 77. Good shit.
script "Dakka_WepDisplay" (int weptype)
{
    if (DISP_ScriptArgs[DPASS_DOCLEANUP])
    {
        TakeInventory("DPickup_SecondWepState", 0x7FFFFFFF);
        TakeInventory("DPickup_BundleState", 0x7FFFFFFF);
        terminate;
    }

    int cpln             = ConsolePlayerNumber();
    int classNum_client  = SToC_ClientData[cpln][S2C_D_CLASSNUM] - 1;

    if (classNum_client != Cl_Dakkaguy) { terminate; }

    int secondWepCheck = WepDisplay_S2CCheck[weptype];

    int useSecond_client = false;
    int bundle_client    = false;

    if (secondWepCheck > -1)
    {
        useSecond_client    = SToC_ClientData[cpln][secondWepCheck];
    }

    // Gotta track this shit by inventory item so that every pickup changes
    //  properly - using an array had just one of the pickups change, and
    //  the rest of them to go "well the values match, time to do nothing :^)"
    int oldSecond = CheckInventory("DPickup_SecondWepState");
    if (oldSecond == useSecond_client) { terminate; }

    SetInventory("DPickup_SecondWepState",  useSecond_client);

    if (useSecond_client)
    {
        SetActorState(0, WepDisplay_States[weptype][WDSTATE_SECONDWEP]);
    }
    else
    {
        SetActorState(0, WepDisplay_States[weptype][WDSTATE_NORMAL]);
    }
}

script "Dakka_WepPickup" (int weptype)
{
    int pln = PlayerNumber();

    int classNum_server = Pickup_ClassNumber(0);
    int secondWepCheck  = WepDisplay_S2CCheck[weptype];

    int useSecond_server = false;

    if (secondWepCheck > -1)
    {
        useSecond_server    = SToC_ServerData[pln][secondWepCheck];
    }


    int normPickup   = WepDisplay_ReturnIndex[weptype][WDSTATE_NORMAL];
    int secondPickup = WepDisplay_ReturnIndex[weptype][WDSTATE_SECONDWEP];

    if (classNum_server != Cl_Dakkaguy)
    {
        SetResultValue(PKP_PickupData[PDATA_ITEMNUM]);
        terminate;
    }

    if (useSecond_server)
    {
        SetResultValue(secondPickup);
        terminate;
    }

    SetResultValue(normPickup);
}