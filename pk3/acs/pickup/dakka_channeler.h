// This is for the plasma rifle with Dakkaguy. If he started the map with a
//  plasma rifle, he gets a channeler instead.

int LastBundlePlasma = 0;

script 494 (int mode)
{
    // This is for the display script.
    // By the way, don't mix up CToS and SToC. Annoying bugs arise.
    int cpln = ConsolePlayerNumber();
    int classNum_client          = SToC_ClientData[cpln][S2C_D_CLASSNUM] - 1;
    int startedWithPlasma_client = SToC_ClientData[cpln][S2C_D_PLASMASTART];
    int bundlePlasma_client      = SToC_ClientData[cpln][S2C_D_BUNDLEPLASMA];

    // This is for the pickup script.
    int pln = PlayerNumber();
    int classNum_server          = Pickup_ClassNumber(0);
    int startedWithPlasma_server = SToC_ServerData[pln][S2C_D_PLASMASTART];
    int bundlePlasma_server      = SToC_ServerData[pln][S2C_D_BUNDLEPLASMA];


    switch (mode)
    {
      // Display
      case 0:
        if (classNum_client != Cl_Dakkaguy) { break; }

        int forceDisplay = false;

        if (               (LastBundlePlasma != bundlePlasma_client)
         || (DISP_ScriptArgs[DPASS_CLASSNUM] != DISP_ScriptArgs[DPASS_OLDCLASSNUM]))
        {
            forceDisplay = true;
        }

        LastBundlePlasma = bundlePlasma_client;

        if (!forceDisplay) { break; }

        if (bundlePlasma_client == 1)       { SetActorState(0, "Bundled"); }
        else if (startedWithPlasma_client)  { SetActorState(0, "Channeler"); }
        else                                { SetActorState(0, "Dakka"); }
        break;

      // Pickup
      case 1:
        if (classNum_server != Cl_Dakkaguy)
        {
            SetResultValue(PKP_PickupData[PDATA_ITEMNUM]);
            break;
        }

        if (bundlePlasma_server)
        {
            SetResultValue(It_Dakka_PlasmaBundle);
            break;
        }

        if (startedWithPlasma_server)
        {
            SetResultValue(It_Dakka_Channeler);
            break;
        }

        SetResultValue(PKP_PickupData[PDATA_ITEMNUM]);
        break;
    }
}
