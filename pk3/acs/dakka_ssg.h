function void SSG_AutoReload(void)
{
    int pln = PlayerNumber();

    // Don't tick down auto reload timer if it's in our hands
    if (CheckWeapon("DWep_SuperShotgun")) { return; }

    // Also don't tick it down if we haven't got ammo for it
    if (CheckInventory("DakkaShells") < 2 && CheckInventory("DakkaGyroShells") < 2)
    {
        return;
    }

    int time = max(0, SSG_AutoReloadCooldown[pln] - 1);

    if (time == 0)
    {
        if (CheckInventory("DakkaSSG_ShotsFired"))
        {
            TakeInventory("DakkaSSG_ShotsFired", 0x7FFFFFFF);
        }
    }

    SSG_AutoReloadCooldown[pln] = time;
}
