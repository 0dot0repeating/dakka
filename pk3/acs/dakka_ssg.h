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



script "DSSG_ShotsLeft" (void)
{
    SetResultValue(SSG_SHOTMAX - CheckInventory("DakkaSSG_ShotsFired"));
}

// Only reason this exists is because A_GiveInventory occasionally desyncs
//  online. This doesn't desync. As often. I think.
script "DSSG_ChangeShots" (int amount)
{
    int pln = PlayerNumber();
    
    if (amount == 0) { amount = 1; }

    if (amount < 0)
    {
        TakeInventory("DakkaSSG_ShotsFired", -amount);
        SSG_AutoReloadCooldown[pln] = 0;
    }
    else
    {
        GiveInventory("DakkaSSG_ShotsFired",  amount);
        SSG_AutoReloadCooldown[pln] = 35;
    }
}

script "DSSG_BothClicked" (void)
{
    SetResultValue(keyPressed(BT_ATTACK | BT_ALTATTACK));
}

script "DSSG_Refire" (void)
{
    int ret = keyPressed_any(BT_ATTACK | BT_ALTATTACK)
              && CheckInventory("DakkaSSG_ShotsFired");

    SetResultValue(ret);
}
