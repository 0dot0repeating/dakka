function void SSG_AutoReload(void)
{
    int pln = PlayerNumber();

    // Don't tick down auto reload timer if it's in our hands
    if (CheckWeapon("DWep_SuperShotgun")) { return; }

    int time = max(0, AutoReloadCooldowns[pln][0] - 1);
    if (time == 0) { SetInventory("DakkaSSG_ShotsFired", 0); }

    AutoReloadCooldowns[pln][0] = time;
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
        AutoReloadCooldowns[pln][0] = 0;
    }
    else
    {
        GiveInventory("DakkaSSG_ShotsFired",  amount);
        AutoReloadCooldowns[pln][0] = 35;
    }
}

script "DSSG_BothClicked" (void)
{
    SetResultValue(keyPressed(BT_ATTACK | BT_ALTATTACK));
}

script "DSSG_Refire" (void)
{
    int canFire = ACS_NamedExecuteWithResult("DSSG_ShotsLeft") > 0;

    // This bug was hilarious.
    if (GetCVar("dakka_bug_nossginvcheck")) { canFire = true; }

    int ret = keyPressed_any(BT_ATTACK | BT_ALTATTACK)
              && CheckInventory("DakkaSSG_ShotsFired");

    SetResultValue(ret && canFire);
}