function void Dakka_GunsToGuns(void)
{
    int pln = PlayerNumber();
    int shotgun_maps   = CheckInventory("DakkaShotgunTracker");
    int shotgun_cvar   = GetCVar("dakka_shotgun2ssg");
    int shotgun_change = false;

    if (shotgun_cvar > 0)
    {
        if (shotgun_maps >= shotgun_cvar) { shotgun_change = true; }
    }

    Sender_SetData(pln, S2C_D_SHOT2SSG,   shotgun_change);
}
