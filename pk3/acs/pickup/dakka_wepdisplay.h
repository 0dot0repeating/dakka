// This is for the plasma rifle with Dakkaguy. If he started the map with a
//  plasma rifle, he gets a channeler instead.

#define WEPDISP_TYPES   4


#define WDSTATE_NORMAL      0
#define WDSTATE_SECONDWEP   1
#define WDSTATE_BUNDLED     2

int WepDisplay_States[WEPDISP_TYPES][3] =
{
    {"Dakka", "Channeler",      "Bundled"},
    {"Dakka", "Impaler",        "Bundled"},
    {"Dakka", "SuperShotgun",   ""},
    {"Dakka", "Minigun",        ""},
};

int WepDisplay_ReturnIndex[WEPDISP_TYPES][3] =
{
    {It_PlasmaRifle,    It_Dakka_Channeler, It_Dakka_PlasmaBundle},
    {It_BFG9000,        It_Dakka_Impaler,   It_Dakka_BFGBundle},
    {It_Shotgun,        It_SuperShotgun,    -1},
    {It_Chaingun,       It_ST_Minigun,      -1},
};

#define WDCHECK_USESECONDWEP    0
#define WDCHECK_USEBUNDLE       1

int WepDisplay_S2CCheck[WEPDISP_TYPES][2] =
{
    {S2C_D_PLASMASTART, S2C_D_BUNDLEPLASMA},
    {S2C_D_BFGSTART,    S2C_D_BUNDLEBFG},
    {S2C_D_SHOT2SSG,    -1},
    {S2C_D_CHAIN2MINI,  -1},
};


script 494 (int weptype, int mode)
{
    if (DISP_ScriptArgs[DPASS_DOCLEANUP])
    {
        TakeInventory("DPickup_SecondWepState", 0x7FFFFFFF);
        TakeInventory("DPickup_BundleState", 0x7FFFFFFF);
        terminate;
    }

    int secondWepCheck = WepDisplay_S2CCheck[weptype][WDCHECK_USESECONDWEP];
    int bundleCheck    = WepDisplay_S2CCheck[weptype][WDCHECK_USEBUNDLE];

    // This is for the display script.
    // By the way, don't mix up CToS and SToC. Annoying bugs arise.
    int cpln = ConsolePlayerNumber();
    int classNum_client     = SToC_ClientData[cpln][S2C_D_CLASSNUM] - 1;
    int useSecond_client = false;
    int bundle_client    = false;


    // This is for the pickup script.
    int pln = PlayerNumber();
    int classNum_server     = Pickup_ClassNumber(0);
    int useSecond_server = false;
    int bundle_server    = false;

    if (secondWepCheck > -1)
    {
        useSecond_client    = SToC_ClientData[cpln][secondWepCheck];
        useSecond_server    = SToC_ServerData[cpln][secondWepCheck];
    }

    if (bundleCheck > -1)
    {
        bundle_client       = SToC_ClientData[cpln][bundleCheck];
        bundle_server       = SToC_ServerData[cpln][bundleCheck];
    }


    switch (mode)
    {
      // Display
      case 0:
        if (classNum_client != Cl_Dakkaguy) { break; }

        int normState   = WepDisplay_States[weptype][WDSTATE_NORMAL];
        int secondState = WepDisplay_States[weptype][WDSTATE_SECONDWEP];
        int bundleState = WepDisplay_States[weptype][WDSTATE_BUNDLED];

        int forceDisplay = false;

        // Gotta track this shit by inventory item so that every pickup changes
        //  properly - using an array had just one of the pickups change, and
        //  the rest of them to go "well the values match, time to do nothing :^)"
        int oldSecond = CheckInventory("DPickup_SecondWepState");
        int oldBundle = CheckInventory("DPickup_BundleState");

        if (oldBundle != bundle_client)     { forceDisplay = true; }
        if (oldSecond != useSecond_client)  { forceDisplay = true; }

        if (DISP_ScriptArgs[DPASS_CLASSNUM] != DISP_ScriptArgs[DPASS_OLDCLASSNUM])
        {
            forceDisplay = true;
        }

        SetInventory("DPickup_BundleState",     bundle_client);
        SetInventory("DPickup_SecondWepState",  useSecond_client);

        if (!forceDisplay) { break; }

        if (bundle_client == 1)     { SetActorState(0, bundleState); }
        else if (useSecond_client)  { SetActorState(0, secondState); }
        else                        { SetActorState(0, normState); }
        break;

      // Pickup
      case 1:
        int normPickup   = WepDisplay_ReturnIndex[weptype][WDSTATE_NORMAL];
        int secondPickup = WepDisplay_ReturnIndex[weptype][WDSTATE_SECONDWEP];
        int bundlePickup = WepDisplay_ReturnIndex[weptype][WDSTATE_BUNDLED];

        if (classNum_server != Cl_Dakkaguy)
        {
            SetResultValue(PKP_PickupData[PDATA_ITEMNUM]);
            break;
        }

        if (bundle_server)
        {
            SetResultValue(bundlePickup);
            break;
        }

        if (useSecond_server)
        {
            SetResultValue(secondPickup);
            break;
        }

        SetResultValue(normPickup);
        break;
    }
}
