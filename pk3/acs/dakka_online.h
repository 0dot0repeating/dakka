script "Dakka_AmmoSync" (int ammoindex)
{
    if (!(IsZandronum && ConsolePlayerNumber() == -1)) { terminate; }
    if (ammoindex < 0 || ammoindex >= AMMOCOUNT) { terminate; }
    
    str ammotype = PKP_KnownAmmo[ammoindex];
    
    if (CheckInventory(ammotype) == 0)
    {
        GiveAmmo(ammotype, 1);
        TakeInventory(ammotype, 1);
    }
    else
    {
        TakeInventory(ammotype, 1);
        GiveAmmo(ammotype, 1);
    }
}


script "Dakka_PlayerNumber" (int ptr)
{
    SetActivator(0, ptr);
    SetResultValue(PlayerNumber());
}

script "Dakka_FollowTarget" (void)
{
    int targetPln = ACS_NamedExecuteWithResult("Dakka_PlayerNumber", AAPTR_TARGET);
    
    if (IsZandronum && targetPln != -1)
    {
        ACS_NamedExecuteAlways("Dakka_FollowTarget_Player", 0, targetPln);
        terminate;
    }
    
    int  x, y, z, a;
    int vx,vy,vz;
    
    while (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        int myTID_old = ActivatorTID();
        int myTID     = UniqueTID();
        Thing_ChangeTID(0, myTID);
        
        SetActivator(0, AAPTR_TARGET);
        x = GetActorX(0);
        y = GetActorY(0);
        z = GetActorZ(0) + GetActorProperty(0, APROP_Height) / 2;
        a = GetActorAngle(0);
        
        vx = GetActorVelX(0);
        vy = GetActorVelY(0);
        vz = GetActorVelZ(0);
        
        SetActivator(myTID);
        
        Warp(0, x,y,z, a, WARPF_ABSOLUTEPOSITION | WARPF_NOCHECKPOSITION | WARPF_INTERPOLATE);
        SetActorVelocity(0, vx,vy,vz, false,false);
        Thing_ChangeTID(0, myTID_old);
        Delay(1);
    }
}

script "Dakka_FollowTarget_Player" (int pln) clientside
{
    if (!IsZandronum) { terminate; }
    
    // angle seems to make a difference with zandronum when HRTF is on
    // I'm not really sure, to be honest
    int  x, y, z, a;
    int vx,vy,vz;
    
    while (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        int myTID_old = ActivatorTID();
        int myTID     = UniqueTID();
        Thing_ChangeTID(0, myTID);
        
        SetActivatorToPlayer(pln);
        
        x = GetActorX(0);
        y = GetActorY(0);
        z = GetActorZ(0) + cond((Timer() / 35) % 2, GetActorProperty(0, APROP_Height) / 2, GetActorViewHeight(0));
        a = GetActorAngle(0);
        
        vx = GetActorVelX(0);
        vy = GetActorVelY(0);
        vz = GetActorVelZ(0);
        
        SetActivator(myTID);
        
        Warp(0, x,y,z, a, WARPF_ABSOLUTEPOSITION | WARPF_NOCHECKPOSITION | WARPF_INTERPOLATE);
        SetActorVelocity(0, vx,vy,vz, false,false);
        Thing_ChangeTID(0, myTID_old);
        Delay(1);
    }
}