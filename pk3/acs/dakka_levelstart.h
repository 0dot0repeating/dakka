function void Dakka_DoLevelStart(void)
{
    int pln       = PlayerNumber();
    int classNum  = Pickup_ClassNumber(0);


    if (classNum == Cl_Dakkaguy)
    {
        int hasPlasma = CheckInventory("DWep_PlasmaRifle");
        Sender_SetData(pln, S2C_D_PLASMASTART, hasPlasma);
        Log(s:"HAS PLASMA: ", d:hasPlasma);
    }
}
