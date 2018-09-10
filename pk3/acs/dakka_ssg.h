function void SSG_AutoReload(void)
{
    if (CheckWeapon("DWep_SuperShotgun")) { return; }
    
    int pln = PlayerNumber();
    if (pln == -1) { return; }
    
    int t = AutoReloadCooldowns[pln][0];
    if (Timer() >= t) { SetInventory("DakkaSSG_ShotsFired", 0); }
}


script "DSSG_CanAutoReload" (void)
{
    int pln = PlayerNumber();

    if (pln == -1 || CheckInventory("DakkaSSG_ShotsFired") == 0)
    {
        SetResultValue(false);
        terminate;
    }
    
    SetResultValue(Timer() >= AutoReloadCooldowns[pln][0]);
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
        AutoReloadCooldowns[pln][0] = Timer() + 49;
    }
}


script "DSSG_BothClicked" (void)
{
    SetResultValue(inputPressed(BT_ATTACK | BT_ALTATTACK));
}


script "DSSG_Refire" (void)
{
    int canFire = ACS_NamedExecuteWithResult("DSSG_ShotsLeft") > 0;

    // This bug was hilarious.
    if (GetCVar("dakka_bug_nossginvcheck")) { canFire = true; }

    int ret = inputPressed_any(BT_ATTACK | BT_ALTATTACK)
              && CheckInventory("DakkaSSG_ShotsFired");

    SetResultValue(ret && canFire);
}