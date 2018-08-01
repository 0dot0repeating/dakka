script "Dakka_ImpalerAltFly" (void)
{
    SetUserVariable(0, "user_updated",  true);
    SetUserVariable(0, "user_velx",     GetActorVelX(0));
    SetUserVariable(0, "user_vely",     GetActorVelY(0));
    SetUserVariable(0, "user_velz",     GetActorVelZ(0));
}



script "Dakka_ImpalerAltHit" (int power)
{
    int projTID = defaultTID(0);
    int projX   = GetActorX(0);
    int projY   = GetActorY(0);
    int projZ   = GetActorZ(0);

    ACS_NamedExecuteWithResult("Dakka_ProjDeathUpdate");

    int projVX    = GetUserVariable(0, "user_velx");
    int projVY    = GetUserVariable(0, "user_vely");
    int projVZ    = GetUserVariable(0, "user_velz");
    int projSpeed = VectorLength(VectorLength(projVX, projVY), projVZ);

    int projNVX = 0, projNVY = 0, projNVZ = 0;
    if (projSpeed != 0)
    {
        projNVX = FixedDiv(projVX, projSpeed);
        projNVY = FixedDiv(projVY, projSpeed);
        projNVZ = FixedDiv(projVZ, projSpeed);
    }

    if (!GetUserVariable(0, "user_t"))
    {
        ACS_NamedExecuteWithResult("Dakka_ImpalerAlt_FindTarget", projVX, projVY, projVZ, projSpeed);
    }


    SetActivator(0, AAPTR_TRACER);

    if (IsWorld())
    {
        SetActorState(projTID, "Detonate");
        terminate;
    }

    if (!CheckFlag(0, "SHOOTABLE") || CheckFlag(0, "BOSS") || CheckFlag(0, "DONTRIP") || CheckFlag(0, "DORMANT"))
    {
        SetActorState(projTID, "Detonate");
        terminate;
    }
    
    int monX      = GetActorX(0);
    int monY      = GetActorY(0);
    int monZ      = GetActorZ(0);
    int monRadius = GetActorProperty(0, APROP_Radius);
    int monHeight = GetActorProperty(0, APROP_Height);

    int monX1 = monX - monRadius,    monX2 = monX + monRadius;
    int monY1 = monY - monRadius,    monY2 = monY + monRadius;
    int monZ1 = monZ,                monZ2 = monZ + monHeight;

    int stepsBack = 0;
    int maxPushDist = min(monRadius, projSpeed);

    SetActivator(projTID);

    // Try multiple positions until we find one where the missile is actually inside the monster
    //  - purely for visual purposes, the pushing works either way
    while (stepsBack < 3)
    {
        int projNewX = projX + FixedMul(projNVX, maxPushDist);
        int projNewY = projY + FixedMul(projNVY, maxPushDist);
        int projNewZ = projZ + FixedMul(projNVZ, maxPushDist);

        if (projNewX > monX1 && projNewX < monX2
         && projNewY > monY1 && projNewY < monY2
         && projNewZ > monZ1 && projNewZ < monZ2)
        {
            projX = projNewX;
            projY = projNewY;
            projZ = projNewZ;
            
            Warp(0, projX, projY, projZ, 0, WARPF_ABSOLUTEPOSITION | WARPF_NOCHECKPOSITION | WARPF_INTERPOLATE);
            break;
        }
        else
        {
            maxPushDist /= 2;
            stepsBack++;
        }
    }
    
    int stage2TID = UniqueTID();
    SpawnForced("ImpalerAltMissile_Stage2", projX, projY, projZ, stage2TID);
    SetActorAngle(stage2TID, GetActorAngle(0));
    SetActorPitch(stage2TID, GetActorPitch(0));
    ACS_NamedExecuteWithResult("Dakka_TransferPointers", projTID, stage2TID);

    SetActivator(stage2TID);
    ACS_NamedExecuteWithResult("Dakka_ImpalerAltPush", projNVX, projNVY, projNVZ, power);
}



script "Dakka_ImpalerAltPush" (int normX, int normY, int normZ, int power)
{
    int projTID = defaultTID(0);
    int projX   = GetActorX(0);
    int projY   = GetActorY(0);
    int projZ   = GetActorZ(0);

    SetActivator(0, AAPTR_TRACER);
    if (IsWorld()) // this should never happen
    {
        SetActorState(projTID, "Detonate");
        terminate;
    }

    int monX = GetActorX(0);
    int monY = GetActorY(0);
    int monZ = GetActorZ(0);
    int monMass = GetActorProperty(0, APROP_Mass);

    int offsetX = projX - monX;
    int offsetY = projY - monY;
    int offsetZ = projZ - monZ;

    int thrustMult;

    if (monMass <= 0)
    {
        thrustMult = itof(min(32767, power));
    }
    else
    {
        int thrustMult_int  = power / monMass;
        int thrustMult_frac = itof(power % monMass) / monMass;
            thrustMult      = itof(min(32767, thrustMult_int)) + thrustMult_frac;
    }

    // Normalization, so zombies don't fly so much further away than everyone else
    thrustMult = FixedSqrt(thrustMult);

    if (!(isDead(0) || CheckFlag(0, "NOPAIN") || CheckFlag(0, "INVULNERABLE") || CheckFlag(0, "DORMANT")))
    {
        SetActorState(0, "Pain");
    }

    SetActorVelocity(0, FixedMul(normX, thrustMult * 3), FixedMul(normY, thrustMult * 3), FixedMul(normZ, thrustMult * 3), false, false);

    int timelimit = 12;
    
    int timecheck = -1; // test for time frreze
    int oldtimecheck;

    while (timelimit > 0 && IsTIDUsed(projTID) && GetUserVariable(projTID, "user_detonated") == false)
    {
        oldtimecheck = timecheck;
        timecheck    = GetUserVariable(0, "user_timecheck");
        
        if (oldtimecheck == timecheck)
        {
            Delay(1);
            continue;
        }
        
        SetActivator(projTID, AAPTR_TRACER);
        SetActorVelocity(0, FixedMul(normX, thrustMult), FixedMul(normY, thrustMult), FixedMul(normZ, thrustMult), true, false);

        monX = GetActorX(0);
        monY = GetActorY(0);
        monZ = GetActorZ(0);

        SetActivator(projTID);
        Warp(0, offsetX + monX, offsetY + monY, offsetZ + monZ, 0, WARPF_ABSOLUTEPOSITION | WARPF_NOCHECKPOSITION | WARPF_INTERPOLATE);

        timelimit--;
        Delay(1);
    }
    
    SetActorState(0, "Detonate");
}



#define IMPALERMASK_ACTORS  (MF_SHOOTABLE)
#define IMPALERMASK_WALLS   (ML_BLOCKING | ML_BLOCKEVERYTHING | ML_BLOCKPROJECTILE)

// Find target if projectile hit something before it could run any states
script "Dakka_ImpalerAlt_FindTarget" (int velX, int velY, int velZ, int speed)
{
    int myTID     = ActivatorTID(); // This is already set to something unique
    int myX       = GetActorX(0);
    int myY       = GetActorY(0);
    int myZ       = GetActorZ(0);
    int myRadius  = GetActorProperty(0, APROP_Radius);
    int myHeight  = GetActorProperty(0, APROP_Height);
    int fireAngle =  VectorAngle(velX, velY);
    int firePitch = -VectorAngle(VectorLength(velX, velY), velZ);

    int targetTID_old = PickActor(0, fireAngle, firePitch, speed, 0, IMPALERMASK_ACTORS, IMPALERMASK_WALLS, PICKAF_RETURNTID);
    int targetTID_new = UniqueTID();

    if (PickActor(0, fireAngle, firePitch, speed, targetTID_new, IMPALERMASK_ACTORS, IMPALERMASK_WALLS, PICKAF_FORCETID))
    {
        SetActivator(targetTID_new);
        if (ClassifyActor(0) & ACTOR_MONSTER)
        {
            //Log(s:"\"", n:0, s:"\c-\" is a huge Jian, please break his legs");

            int monX      = GetActorX(0);
            int monY      = GetActorY(0);
            int monZ      = GetActorZ(0);
            int monRadius = GetActorProperty(0, APROP_Radius);
            int monHeight = GetActorProperty(0, APROP_Height);

            int sharedRadius = (myRadius + monRadius);
            int sharedHeight = (myHeight + monHeight);

            int testX1 = monX - sharedRadius, testX2 = monX + sharedRadius;
            int testY1 = monY - sharedRadius, testY2 = monY + sharedRadius;
            int testZ1 = monZ - myHeight,     testZ2 = monZ + sharedHeight;

            if (myX >= testX1 && myX <= testX2
             && myY >= testY1 && myY <= testY2
             && myZ >= testZ1 && myZ <= testZ2)
            {
                 //Log(s:"\cg -- LEG BREAKING IN PROGRESS -- ");
                 SetActivator(myTID);
                 SetPointer(AAPTR_TRACER, targetTID_new);
                 SetActivator(targetTID_new);
            }
        }

        Thing_ChangeTID(0, targetTID_old);
    }
}