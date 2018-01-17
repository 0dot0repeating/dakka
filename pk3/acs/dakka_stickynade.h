function int DShotgun_TestWallStick(int projX, int projY, int projZ, int projAngle, int projRadius)
{
    int stickyTID   = UniqueTID();
    
    int projEdgeX   = FixedMul(projRadius, cos(projAngle));
    int projEdgeY   = FixedMul(projRadius, sin(projAngle));
    int projEdgeMult = FixedDiv(projRadius + 8.0, max(abs(projEdgeX), abs(projEdgeY)));
    //Log(s:"\cd<", f:projEdgeX, s:", ", f:projEdgeY, s:">   \cf", f:projAngle, s:"   \ck", f:projEdgeMult);
    
    projEdgeX = projX + FixedMul(projEdgeX, projEdgeMult);
    projEdgeY = projY + FixedMul(projEdgeY, projEdgeMult);
    
    // check if stuck to upper or lower part of sector
    SpawnForced("ShotgunStickyPoint", projEdgeX, projEdgeY, projZ, stickyTID);
    
    int stickyZ = GetActorZ(stickyTID);
    int floorZ  = GetActorFloorZ(stickyTID);
    int ceilZ   = GetActorCeilingZ(stickyTID);
    
    Thing_Remove(stickyTID);
    
    if (stickyZ <= floorZ)
    {
        //Log(s:"stuck to lower part of wall (", f:stickyZ, s:" vs ", f:floorZ, s:")");
        SpawnForced("ShotgunStickyPoint_Floor", projEdgeX, projEdgeY, floorZ, stickyTID);
        return stickyTID;
    }
    
    if (stickyZ >= ceilZ)
    {
        //Log(s:"stuck to upper part of wall (", f:stickyZ, s:" vs ", f:ceilZ, s:")");
        SpawnForced("ShotgunStickyPoint_Ceiling", projEdgeX, projEdgeY, ceilZ, stickyTID);
        return stickyTID;
    }
    
    return 0;
}

script "Dakka_StickyGrenade" (int manual, int autoTimer)
{
    int projTID_old = ActivatorTID();
    int projTID     = UniqueTID();
    Thing_ChangeTID(0, projTID);
    
    int projX      = GetActorX(0);
    int projY      = GetActorY(0);
    int projZ      = GetActorZ(0);
    int projVelX   = GetUserVariable(0, "user_velx"); // multiplied by 65536 in
    int projVelY   = GetUserVariable(0, "user_vely"); // the DECORATE, so we can
    int projVelZ   = GetUserVariable(0, "user_velz"); // just grab them raw here
    int projHeight = GetActorProperty(0, APROP_Height);
    int projRadius = GetActorProperty(0, APROP_Radius);
    int projAngle  = VectorAngle(projVelX, projVelY);
    
    SetActivator(0, AAPTR_TARGET);
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        Thing_ChangeTID(projTID, projTID_old);
        terminate;
    }
    
    if (manual && PlayerNumber())
    {
        if (GetUserCVar(PlayerNumber(), "dakka_cl_autodetonate"))
        {
            SetActorState(projTID, "Detonate");
            Thing_ChangeTID(projTID,   projTID_old);
            terminate;
        }
    }
    
    str firerStartWeapon = GetWeapon();
    int firerTID_old = ActivatorTID();
    int firerTID     = UniqueTID();
    
    Thing_ChangeTID(0, firerTID);
    
    int projVel    = VectorLength(VectorLength(projVelX, projVelY), projVelZ);
    int projNormX, projNormY, projNormZ;
    
    if (projVel == 0)
    {
        int firerAngle = GetActorAngle(0);
        int firerPitch = GetActorPitch(0);
        projAngle = firerAngle;
        
        projNormX = FixedMul(cos(firerAngle), cos(firerPitch));
        projNormY = FixedMul(sin(firerAngle), cos(firerPitch));
        projNormZ = -sin(firerPitch);
        
        projVel  = GetActorProperty(projTID, APROP_Speed);
        projVelX = FixedMul(projVel, projNormX);
        projVelY = FixedMul(projVel, projNormY);
        projVelZ = FixedMul(projVel, projNormZ);
    }
    else
    {
        projNormX = FixedDiv(projVelX, projVel);
        projNormY = FixedDiv(projVelY, projVel);
        projNormZ = FixedDiv(projVelZ, projVel);
    }
    
    
    int stuckToObject, stickyStartAngle, stickyTID_old;
    int stickyTID = UniqueTID();
    SetActivator(projTID, AAPTR_TRACER);
    
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        int projFloorZ = GetActorFloorZ(projTID);
        int projCeilZ  = GetActorCeilingZ(projTID);
        
        stickyTID_old  = stickyTID;
        
        if (projCeilZ <= projZ + projHeight)
        {
            //Log(s:"stuck to ceiling");
            SpawnForced("ShotgunStickyPoint_Ceiling", projX, projY, projCeilZ, stickyTID);
        }
        else if (projFloorZ >= projZ)
        {
            //Log(s:"stuck to floor");
            SpawnForced("ShotgunStickyPoint_Floor", projX, projY, projFloorZ, stickyTID);
        }
        else
        {
            //Log(s:"stuck to wall");
            
            int newStickyTID = 0;
            
            // stop this from running in zandronum until it supports Warpit
            //  - it ain't ACS without a hack
            if (GetActorProperty(projTID, APROP_DamageType) != 0)
            {
                for (int i = 0; i <= 0.5; i += 0.1)
                {
                    newStickyTID = DShotgun_TestWallStick(projX, projY, projZ, projAngle + i, projRadius);
                    if (!newStickyTID && (i != 0)) { newStickyTID = DShotgun_TestWallStick(projX, projY, projZ, projAngle - i, projRadius); }
                    if (newStickyTID) { break; }
                }
            }
            
            //Log(s:"\cqnewStickyTID: \cd", d:newStickyTID);
            
            if (newStickyTID)
            {
                stickyTID = newStickyTID;
            }
            else
            {
                SpawnForced("ShotgunStickyPoint", projX, projY, projZ, stickyTID);
            }   
        }
        
        stickyStartAngle = projAngle;
        stuckToObject   = false;
        
        SetActivator(stickyTID);
        SetActorAngle(0, projAngle);
    }
    else
    {
        //Log(s:"stuck to thing");
        
        stickyTID_old = ActivatorTID();
        Thing_ChangeTID(0, stickyTID);
        stickyStartAngle   = GetActorAngle(0);
        stuckToObject      = true;
        
        // sink into the hit thing some
        int stickyRadius = GetActorProperty(0, APROP_Radius);
        int stickyHeight = GetActorProperty(0, APROP_Height);
        projX += FixedMul(projNormX, stickyRadius / 2);
        projY += FixedMul(projNormY, stickyRadius / 2);
        projZ += FixedMul(projNormZ, stickyHeight / 4);
    }
    
    int stickyDiffX     = projX     - GetActorX(0);
    int stickyDiffY     = projY     - GetActorY(0);
    int stickyDiffZ     = projZ     - GetActorZ(0);
    int stickyDiffAngle = projAngle - stickyStartAngle;
    
    // if the target dies on impact, lower this by 2.0 every tic
    int stickyDeadDiffZ = stickyDiffZ;
    
    int ptrTID = UniqueTID();
    SpawnForced("ShotgunStickyPoint", projX,projY,projZ, ptrTID);
    SetActivator(ptrTID);
    SetPointer(AAPTR_TARGET, firerTID);
    SetPointer(AAPTR_MASTER, projTID);
    SetPointer(AAPTR_TRACER, stickyTID);
    
    Thing_ChangeTID(firerTID,  firerTID_old);
    Thing_ChangeTID(projTID,   projTID_old);
    Thing_ChangeTID(stickyTID, stickyTID_old);
    
    int t = 0;
    
    while (true)
    {
        // check if things are alive
        SetActivator(ptrTID, AAPTR_MASTER);
        if (ClassifyActor(0) & ACTOR_WORLD) { break; }
        
        SetActivator(ptrTID, AAPTR_TARGET);
        if (ClassifyActor(0) & ACTOR_WORLD) { break; }
        if (isDead(0) && manual) { break; }
        
        SetActivator(ptrTID, AAPTR_TRACER);
        if (ClassifyActor(0) & ACTOR_WORLD) { break; }
        
        
        // check if should go kaboom anyway
        SetActivator(ptrTID, AAPTR_TARGET);
        
        if (manual)
        {
            int noManualDet  = false;
            if (PlayerNumber() > -1)
            {
                noManualDet = GetUserCVar(PlayerNumber(), "dakka_cl_autodetonate");
            }
            
            if (noManualDet) { break; }
            if (inputDown(BT_RELOAD) || strcmp(GetWeapon(), firerStartWeapon))
            {
                GiveInventory("DakkaGrenadeClick", 1);
                break;
            }
        }
        
        if (autoTimer > 0 && t >= autoTimer) { break; }
        
        
        // onward to sticky shit
        SetActivator(ptrTID, AAPTR_TRACER);
        
        int stickyX     = GetActorX(0);
        int stickyY     = GetActorY(0);
        int stickyZ     = GetActorZ(0);
        int stickyAngle = GetActorAngle(0);
        
        int newDiffAngle = stickyAngle - stickyStartAngle;
        int newDiffX     = FixedMul(stickyDiffX, cos(newDiffAngle)) - FixedMul(stickyDiffY, sin(newDiffAngle));
        int newDiffY     = FixedMul(stickyDiffX, sin(newDiffAngle)) + FixedMul(stickyDiffY, cos(newDiffAngle));
        int newDiffZ     = stickyDiffZ;
        
        if (isDead(0))
        {
            newDiffZ = min(newDiffZ, stickyDeadDiffZ);
            stickyDeadDiffZ = max(8.0, stickyDeadDiffZ - 2.0);
        }
        
        int newProjX     = stickyX + newDiffX;
        int newProjY     = stickyY + newDiffY;
        int newProjZ     = stickyZ + newDiffZ;
        int newProjAngle = stickyAngle + stickyDiffAngle;
        
        SetActivator(ptrTID, AAPTR_MASTER);
        int warpFlags = WARPF_INTERPOLATE | WARPF_ABSOLUTEPOSITION | WARPF_ABSOLUTEANGLE | cond(stuckToObject, WARPF_NOCHECKPOSITION, 0);
        if (!Warp(0, newProjX, newProjY, newProjZ, newProjAngle, warpFlags)) { break; }
        SetActorAngle(0, newProjAngle); // for zandronum
        
        t++;
        Delay(1);
    }
    
    SetActivator(ptrTID, AAPTR_MASTER);
    if (~ClassifyActor(0) & ACTOR_WORLD)
    {
        SetActorState(0, "Detonate");
    }
    
    if (!stuckToObject)
    {
        SetActivator(ptrTID, AAPTR_TRACER);
        Thing_Remove(0);
    }
    
    Thing_Remove(ptrTID);
}


script "Dakka_AlignToSurface" (int which)
{
    int ang = GetActorAngle(0); // thanks zandronum
    
    switch (which)
    {
      default:
        Warp(0, GetActorX(0), GetActorY(0), GetActorFloorZ(0), ang, WARPF_ABSOLUTEPOSITION | WARPF_ABSOLUTEANGLE | WARPF_NOCHECKPOSITION);
        break;
      
      case 1:
        Warp(0, GetActorX(0), GetActorY(0), GetActorCeilingZ(0), ang, WARPF_ABSOLUTEPOSITION | WARPF_ABSOLUTEANGLE | WARPF_NOCHECKPOSITION);
        break;
    }
    
    SetActorAngle(0, ang);
}

script "Dakka_CheckDetonate" (void)
{
    SetActivator(0, AAPTR_TARGET);
    SetResultValue(inputDown(BT_RELOAD));
}