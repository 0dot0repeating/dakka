function void Pistols_SpreadRecovery(void)
{
    if (CheckInventory("DakkaPistolSpread"))
    {
        TakeInventory("DakkaPistolSpread", 1);
    }
}

function void Pistols_CapBulletsLeft(void)
{
    int capped = min(CheckInventory("DakkaPistol_ShotsLeft"), CheckInventory("DakkaBullet_50AE"));
    SetInventory("DakkaPistol_ShotsLeft", capped);
}

script "Dakka_PistolSpread" (int axis)
{
    int ret;
    int spreadMod = CheckInventory("DakkaPistolSpread");
    
    if (axis == 1) // vertical spread
    {
        ret = 0.5 + (0.1 * spreadMod);
    }
    else // horizontal spread
    {
        ret = 0.5 + (0.125 * spreadMod);
    }
    
    //Log(s:cond(axis == 1, "Y", "X"), s:" spread: ", f:ret);
    SetResultValue(ret);
}

script "Dakka_PistolReload" (void)
{
    int maxBullets = min(GetAmmoCapacity("DakkaPistol_ShotsLeft"), CheckInventory("DakkaBullet_50AE"));
    SetInventory("DakkaPistol_ShotsLeft", maxBullets);
}

script "Dakka_PistolFull" (void)
{
    int bullets    = CheckInventory("DakkaPistol_ShotsLeft");
    int maxBullets = min(GetAmmoCapacity("DakkaPistol_ShotsLeft"), CheckInventory("DakkaBullet_50AE"));
    SetResultValue(bullets >= maxBullets);
}

script "Dakka_CanPistolReload" (int noflag)
{
    int shotsCurrent = CheckInventory("DakkaPistol_ShotsLeft");
    int shotsAfter   = min(GetAmmoCapacity("DakkaPistol_ShotsLeft"), CheckInventory("DakkaBullet_50AE"));
    
    if (noflag)
    {
        SetResultValue(shotsAfter > shotsCurrent);
    }
    else
    {
        // WRF_ALLOWRELOAD == 16
        SetResultValue(cond(shotsAfter > shotsCurrent, 16, 0));
    }
}