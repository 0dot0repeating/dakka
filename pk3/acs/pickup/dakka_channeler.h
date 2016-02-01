// This is for the plasma rifle with Dakkaguy. If he started the map with a
//  plasma rifle, he gets a channeler instead.

script 494 (int mode)
{
    // This is for the display script.
    // By the way, don't mix up CToS and SToC. Annoying bugs arise.
    int cpln = ConsolePlayerNumber();
    int classNum_client = SToC_ClientData[cpln][S2C_D_CLASSNUM] - 1;
    int startedWithPlasma_client = SToC_ClientData[cpln][S2C_D_PLASMASTART];

    // This is for the pickup script.
    int pln = PlayerNumber();
    int classNum_server = Pickup_ClassNumber(0);
    int startedWithPlasma_server = SToC_ServerData[pln][S2C_D_PLASMASTART];

    switch (mode)
    {
      // Display
      case 0:
        if (DISP_ScriptArgs[DPASS_CLASSNUM] == DISP_ScriptArgs[DPASS_OLDCLASSNUM])
        {
            terminate;
        }

        Log(s:"my arse (cnum: ", d:classNum_client, s:", has plasma: ", d:startedWithPlasma_client, s:")");

        if (classNum_client == Cl_Dakkaguy && startedWithPlasma_client)
        {
            SetActorState(0, "Channeler");
        }
        break;

      // Pickup
      case 1:
        if (classNum_server == Cl_Dakkaguy && startedWithPlasma_server)
        {
            SetResultValue(It_Dakka_Channeler);
        }
        else
        {
            SetResultValue(PKP_PickupData[PDATA_ITEMNUM]);
        }
        break;
    }
}
