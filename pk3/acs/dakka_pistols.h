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


#define PUAC_SHELL      0x01
#define PUAC_GRENADE    0x02
#define PUAC_5MM        0x04
#define PUAC_FLAMER     0x08
#define PUAC_ROCKET     0x10
#define PUAC_CELL       0x20

script "Dakka_PickedUpAClip" (int count, int check)
{
    if (GetCVar("dakka_pickedupaclip") != 1)
    {
        SetResultValue(false);
        terminate;
    }
    
    if (CheckInventory("DakkaBullet_50AE") >= GetAmmoCapacity("DakkaBullet_50AE"))
    {
        SetResultValue(false);
        terminate;
    }
        
    
    if (check)
    {
        int full = true;
        
        if (check & PUAC_SHELL)   { full &= CheckInventory("DakkaShells")       >= GetAmmoCapacity("DakkaShells"); }
        if (check & PUAC_GRENADE) { full &= CheckInventory("DakkaShotGrenades") >= GetAmmoCapacity("DakkaShotGrenades"); }
        if (check & PUAC_5MM)     { full &= CheckInventory("DakkaBullet_5mm")   >= GetAmmoCapacity("DakkaBullet_5mm"); }
        if (check & PUAC_FLAMER)  { full &= CheckInventory("DakkaFlamerFuel")   >= GetAmmoCapacity("DakkaFlamerFuel"); }
        if (check & PUAC_ROCKET)  { full &= CheckInventory("DakkaRockets")      >= GetAmmoCapacity("DakkaRockets"); }
        if (check & PUAC_CELL)    { full &= CheckInventory("DakkaCells")        >= GetAmmoCapacity("DakkaCells"); }
        
        if (full)
        {
            SetResultValue(false);
            terminate;
        }
    }
    
    GiveInventory("DakkaBullet_50AE", count);
    SetResultValue(true);
}


script "Dakka_ClipSpawn" (int count, int big)
{
    if (GetCVar("dakka_pickedupaclip") != 2) { terminate; }
    if (CheckInventory("SpawnedClipAlready")) { terminate; }
    
    int clipTID = UniqueTID();
    int dropped = CheckFlag(0, "DROPPED");
    int x = GetActorX(0);
    int y = GetActorY(0);
    int z = GetActorZ(0);
    
    int actor = cond(big, "Dakka_BigClip", "Dakka_TinyClip");
    
    for (int i = 0; i < count; i++)
    {
        int randAngle = random(0, 1.0);
        int randDist  = random(1.0, 3.0);
        
        SpawnForced(actor, x,y,z, clipTID);
        SetActorVelocity(clipTID, FixedMul(randDist, cos(randAngle)), FixedMul(randDist, sin(randAngle)), 0, false,false);
        
        if (!dropped) { GiveActorInventory(clipTID, "DakkaNotDropped", 1); }
        Thing_ChangeTID(clipTID, 0);
    }
    
    GiveInventory("SpawnedClipAlready", 1);
}