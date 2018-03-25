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


script "Dakka_StickyGrenade" (void)
{
    int projTID_old = ActivatorTID();
    int projTID     = UniqueTID();
    Thing_ChangeTID(0, projTID);

    int projX      = GetActorX(0);
    int projY      = GetActorY(0);
    int projZ      = GetActorZ(0);
    int projHeight = GetActorProperty(0, APROP_Height);
    int projRadius = GetActorProperty(0, APROP_Radius);

    ACS_NamedExecuteWithResult("Dakka_ProjDeathUpdate");
    int projVelX   = GetUserVariable(0, "user_velx");
    int projVelY   = GetUserVariable(0, "user_vely");
    int projVelZ   = GetUserVariable(0, "user_velz");
    int projAngle  = VectorAngle(projVelX, projVelY);

    int projVel    = VectorLength(VectorLength(projVelX, projVelY), projVelZ);
    int projNormX = 0, projNormY = 0, projNormZ = 0;

    if (projVel != 0)
    {
        projNormX = FixedDiv(projVelX, projVel);
        projNormY = FixedDiv(projVelY, projVel);
        projNormZ = FixedDiv(projVelZ, projVel);
    }


    int stuckToObject, stickyStartAngle, stickyTID_old;
    int stickyTID = UniqueTID();
    SetActivator(projTID, AAPTR_TRACER);

    if (IsWorld())
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

            // don't even bother in zandronum until it supports Warp better
            //  - it ain't ACS without a hack
            if (GetActorProperty(projTID, APROP_DamageType) != 0)
            {
                for (int i = 0; i <= 0.5; i += 0.025)
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
    SetPointer(AAPTR_MASTER, projTID);
    SetPointer(AAPTR_TRACER, stickyTID);
    SetUserVariable(0, "user_onmonster", ClassifyActor(stickyTID) & ACTOR_MONSTER);
        
    SetActivator(projTID);
    SetPointer(AAPTR_TRACER, ptrTID);
    
    SetActivator(projTID, AAPTR_TARGET);
    
    if (!IsWorld())
    {
        int firerTID_old = ActivatorTID();
        int firerTID     = UniqueTID();
        Thing_ChangeTID(0, firerTID);
        
        SetActivator(ptrTID);
        SetPointer(AAPTR_TARGET, firerTID);
        Thing_ChangeTID(firerTID, firerTID_old);
    }

    Thing_ChangeTID(projTID,   projTID_old);
    Thing_ChangeTID(stickyTID, stickyTID_old);

    int t = 0;

    while (true)
    {
        // check if things are alive
        SetActivator(ptrTID, AAPTR_MASTER);
        if (IsWorld()) { break; }

        SetActivator(ptrTID, AAPTR_TRACER);
        if (IsWorld()) { break; }

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
        if (!Warp(0, newProjX, newProjY, newProjZ, newProjAngle, warpFlags))
        {
            int floorZ = GetActorFloorZ(0);
            int ceilZ  = GetActorCeilingZ(0);

            // So that the grenade counts as being on the floor/ceiling when it explodes
            if (newProjZ <= floorZ)
            {
                Warp(0, newProjX, newProjY, floorZ, newProjAngle | WARPF_NOCHECKPOSITION, warpFlags);
            }
            else if (newProjZ + projHeight >= ceilZ)
            {
                Warp(0, newProjX, newProjY, ceilZ - projHeight, newProjAngle | WARPF_NOCHECKPOSITION, warpFlags);
            }

            break;
        }
        SetActorAngle(0, newProjAngle); // for zandronum

        t++;
        Delay(1);
    }

    if (!stuckToObject)
    {
        SetActivator(ptrTID, AAPTR_TRACER);
        Thing_Remove(0);
    }

    Thing_Remove(ptrTID);
}


script "DShotgun_CheckEarlyDetonate" (void)
{
    SetActivator(0, AAPTR_TARGET);
    if (PlayerNumber() == -1) { terminate; }
    SetResultValue(inputDown(BT_RELOAD));
}


script "DShotgun_CheckDetonate" (void)
{
    // doing it this way because hey let's have scripts use SetResultValue() and terminate
    //  instead of the perfectly good return keyword that makes total fucking sense doesn't it
    SetResultValue(DShotgun_CheckDetonate());
}

// if not doing sticky stuff: boom
// if firer is not player, or is dead: boom
// if firer has not-a-shotgun out: boom
// if firer has reload button down: boom
// if firer has auto-detonate always on: boom
// if firer has auto-detonate on for monsters, and stuck to monster: boom

function int DShotgun_CheckDetonate(void)
{
    SetActivator(0, AAPTR_TRACER);
    if (strcmp(GetActorClass(0), "ShotgunStickyPoint")) { return true; }
    
    int ptrTID = ActivatorTID();
    SetActivator(0, AAPTR_TARGET);
    int pln    = PlayerNumber();
    if ((pln == -1) || isDead(0)) { return true; }
    
    if (strcmp(GetWeapon(), "DWep_Shotgun")) { return true; }
    if (inputDown(BT_RELOAD)) { return true; }
    
    int manual    = GetUserCVar(pln, "dakka_cl_autodetonate");
    int onMonster = GetUserVariable(ptrTID, "user_onmonster");
    
    if (manual >= 2) { return true; }
    if (onMonster && manual == 1) { return true; }
    
    return false;
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