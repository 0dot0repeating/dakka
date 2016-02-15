function void Dakka_GunsToGuns(void)
{
    int shotgun_maps  = CheckInventory("DakkaShotgunTracker");
    int chaingun_maps = CheckInventory("DakkaChaingunTracker");

    int shotgun_cvar  = GetCVar("dakka_shotgun2ssg");
    int chaingun_cvar = GetCVar("dakka_chaingun2minigun");

    int shotgun_change  = false;
    int chaingun_change = false;

    if (shotgun_cvar > 0)
    {
        if (shotgun_maps >= shotgun_cvar) { shotgun_change = true; }
    }

    if (chaingun_cvar > 0)
    {
        if (chaingun_maps >= chaingun_cvar) { chaingun_change = true; }
    }

    int pln = PlayerNumber();

    Sender_SetData(pln, S2C_D_SHOT2SSG,   shotgun_change);
    Sender_SetData(pln, S2C_D_CHAIN2MINI, chaingun_change);
}
