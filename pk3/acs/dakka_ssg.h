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


script "DSSG_CalcGrenadeAngle" (int speed, int spread_angle, int spread_pitch)
{
    int randAngle = ACS_NamedExecuteWithResult("Dakka_Spread", -spread_angle, spread_angle) / 360;
    int randPitch = ACS_NamedExecuteWithResult("Dakka_Spread", -spread_pitch, spread_pitch) / 360;
    
    // updated by scripts in dakka_projangle.h, more reliable than reading velocity directly
    int vx  = GetUserVariable(0, "user_velx");
    int vy  = GetUserVariable(0, "user_vely");
    int vz  = GetUserVariable(0, "user_velz");
    
    int angle =  VectorAngle(vx, vy);
    int pitch = -VectorAngle(VectorLength(vx, vy), vz);
    
    int spawnVY = speed * FixedMul(sin(randAngle), cos(randPitch));
    int spawnVX = speed * FixedMul(cos(randAngle), cos(randPitch));
    int spawnVZ = speed * sin(randPitch);
    
    Rotate3D(spawnVX, spawnVY, spawnVZ, angle, pitch);
    
    SetUserVariable(0, "user_spawnvelx", Rotate3D_Ret[0]);
    SetUserVariable(0, "user_spawnvely", Rotate3D_Ret[1]);
    SetUserVariable(0, "user_spawnvelz", Rotate3D_Ret[2]);
}

script "DSSG_HitEnemy" (void)
{
    SetActivator(0, AAPTR_TRACER);
    SetResultValue(!(ClassifyActor(0) & ACTOR_WORLD));
}