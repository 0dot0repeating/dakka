script "Dakka_RLFull" (void)
{
    int ret = (CheckInventory("DakkaRockets") - CheckInventory("DakkaRocketsLoaded")) > 0;
    ret |= CheckInventory("DakkaInfiniteAmmo");
    SetResultValue(ret);
}


// No I don't expect this to ever get reused ever again.
// Quite frankly, 80% of it is hacks to get things to move as expected.
// It's pretty awful.


script "Dakka_ImpalerSideMissiles" (void)
{
    int myX     = GetActorX(0);
    int myY     = GetActorY(0);
    int myZ     = GetActorZ(0);
    int myVelX  = GetActorVelX(0);
    int myVelY  = GetActorVelY(0);
    int myVelZ  = GetActorVelZ(0);
    int myAng   = GetActorAngle(0);
    int myPitch = -VectorAngle(VectorLength(myVelX, myVelY), myVelZ);
    int myTID   = defaultTID(0);
    
    SetActorPitch(0, myPitch);
    
    int leftTID  = UniqueTID();
    int leftX    = myX - FixedMul(1.2, myVelX) + (6 * cos(myAng + 0.25));
    int leftY    = myY - FixedMul(1.2, myVelY) + (6 * sin(myAng + 0.25));
    int leftZ    = myZ - FixedMul(1.2, myVelZ);
    
    int rightTID = UniqueTID();
    int rightX   = myX - FixedMul(1.2, myVelX) + (6 * cos(myAng - 0.25));
    int rightY   = myY - FixedMul(1.2, myVelY) + (6 * sin(myAng - 0.25));
    int rightZ   = myZ - FixedMul(1.2, myVelZ) ;
    
    SpawnForced("ImpalerAltMissile_Side", leftX,  leftY,  leftZ,  leftTID);
    SpawnForced("ImpalerAltMissile_Side", rightX, rightY, rightZ, rightTID);
    
    SetActorVelocity(leftTID,  myVelX, myVelY, myVelZ, false, false);
    SetActorVelocity(rightTID, myVelX, myVelY, myVelZ, false, false);
    
    SetActorAngle(leftTID,  myAng);
    SetActorPitch(leftTID,  myPitch);
    SetActorAngle(rightTID, myAng);
    SetActorPitch(rightTID, myPitch);
    
    SetActivator(0, AAPTR_TARGET);
    int myFirer = defaultTID(0);
    
    SetActivator(leftTID);
    SetPointer(AAPTR_TARGET, myFirer);
    SetPointer(AAPTR_MASTER, myTID);
    
    SetActivator(rightTID);
    SetPointer(AAPTR_TARGET, myFirer);
    SetPointer(AAPTR_MASTER, myTID);
}


script "Dakka_ImpalerMaster" (int justSpawned)
{
    int myX    = GetActorX(0);
    int myY    = GetActorY(0);
    int myZ    = GetActorZ(0);
    int myVelX = GetActorVelX(0);
    int myVelY = GetActorVelY(0);
    int myVelZ = GetActorVelZ(0);
    
    int myPitch = -VectorAngle(VectorLength(myVelX, myVelY), myVelZ);
    SetActorPitch(0, myPitch);
    
    int myTID = defaultTID(0);
    
    SetActivator(0, AAPTR_TRACER);
    
    if (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        int flytime = GetUserVariable(myTID, "user_flytime") - 1;
        
        if (flytime < 0)
        {
            SetActorVelocity(0, (myVelX / 4) + random(-8.0, 8.0), (myVelY / 4) + random(-8.0, 8.0), (myVelZ / 4) + random(-2.0, 8.0), false, false);
            SetActivator(myTID);
            Warp(0, myX - myVelX, myY - myVelY, myZ - myVelZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION | WARPF_STOP | WARPF_INTERPOLATE);
            SetActorState(myTID, "Detonate");
            terminate;
        }
        
        SetUserVariable(myTID, "user_flytime", flytime);
        
        int impaleX = GetUserVariable(myTID, "user_impalex");
        int impaleY = GetUserVariable(myTID, "user_impaley");
        int impaleZ = GetUserVariable(myTID, "user_impalez");
        
        int newX = myX + impaleX;
        int newY = myY + impaleY;
        int newZ = myZ + impaleZ;
        
        int moved = SetActorPosition(0, newX, newY, newZ, false);
        
        if ((!moved) && (myVelZ < 0 && myVelZ > -6.0))
        {
            moved = SetActorPosition(0, newX, newY, GetActorZ(0), false);
        }
        
        if (!moved)
        {
            SetActorVelocity(0, (myVelX / 4) + random(-8.0, 8.0), (myVelY / 4) + random(-8.0, 8.0), (myVelZ / 4) + random(-2.0, 8.0), false, false);
            SetActivator(myTID);
            Warp(0, myX - myVelX, myY - myVelY, myZ - myVelZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION | WARPF_STOP | WARPF_INTERPOLATE);
            SetActorState(myTID, "Detonate");
        }
        else
        {
            SetActorVelocity(0, myVelX, myVelY, myVelZ-1.0, false, false);
            SetActivator(myTID);
            SetActorVelocity(0, 0,0,-0.67, true, false);
        }
    }
    else
    {
        SetActivator(myTID);
        SetUserVariable(0, "user_impaled", false);
        int mySpeed = VectorLength(VectorLength(myVelX, myVelY), myVelZ);
        
        if (justSpawned)
        {
            Warp(0, myX - myVelX, myY - myVelY, myZ - myVelZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION | WARPF_INTERPOLATE);
            LineAttack(0,  GetActorAngle(0),       GetActorPitch(0)+0.025, 0, "ImpalerImpaleChecker", "None", mySpeed);
            LineAttack(0,  GetActorAngle(0)-0.025, GetActorPitch(0)+0.025, 0, "ImpalerImpaleChecker", "None", mySpeed);
            LineAttack(0,  GetActorAngle(0)+0.025, GetActorPitch(0)+0.025, 0, "ImpalerImpaleChecker", "None", mySpeed);
            LineAttack(0,  GetActorAngle(0),       GetActorPitch(0)+0.05,  0, "ImpalerImpaleChecker", "None", mySpeed);
            LineAttack(0,  GetActorAngle(0),       GetActorPitch(0),       0, "ImpalerImpaleChecker", "None", mySpeed);
            
            if (GetUserVariable(0, "user_impaled"))
            {
                SetUserVariable(0, "user_impalex", GetUserVariable(0, "user_impalex") + (myVelX * 2));
                SetUserVariable(0, "user_impaley", GetUserVariable(0, "user_impaley") + (myVelY * 2));
                SetUserVariable(0, "user_impalez", GetUserVariable(0, "user_impalez") + (myVelZ * 2));
            }
            
            Warp(0, myX, myY, myZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION | WARPF_INTERPOLATE);
        }
        
        LineAttack(0,  GetActorAngle(0),       GetActorPitch(0)+0.025, 0, "ImpalerImpaleChecker", "None", mySpeed);
        LineAttack(0,  GetActorAngle(0)-0.025, GetActorPitch(0)+0.025, 0, "ImpalerImpaleChecker", "None", mySpeed);
        LineAttack(0,  GetActorAngle(0)+0.025, GetActorPitch(0)+0.025, 0, "ImpalerImpaleChecker", "None", mySpeed);
        LineAttack(0,  GetActorAngle(0),       GetActorPitch(0)+0.05,  0, "ImpalerImpaleChecker", "None", mySpeed);
        LineAttack(0,  GetActorAngle(0),       GetActorPitch(0),       0, "ImpalerImpaleChecker", "None", mySpeed);
    }
}


script "Dakka_ImpalerSlave" (void)
{
    int myTID = defaultTID(0);
    SetActivator(myTID, AAPTR_MASTER);
    
    int detonated = GetUserVariable(0, "user_detonated");
    
    
    if (detonated)
    {
        SetActivator(myTID);
        
        int myX    = GetActorX(0);
        int myY    = GetActorY(0);
        int myZ    = GetActorZ(0);
        int myVelX = GetActorVelX(0);
        int myVelY = GetActorVelY(0);
        int myVelZ = GetActorVelZ(0);
        
        Warp(0, myX - myVelX, myY - myVelY, myZ - myVelZ, 0, WARPF_NOCHECKPOSITION | WARPF_ABSOLUTEPOSITION | WARPF_STOP | WARPF_INTERPOLATE);
        SetActorState(0, "Death");
    }
    else
    {
        int masterVX = GetActorVelX(0);
        int masterVY = GetActorVelY(0);
        int masterVZ = GetActorVelZ(0);
        SetActorVelocity(myTID, masterVX, masterVY, masterVZ, false, false);
    }
}


script "Dakka_DoTheImpale" (void)
{
    int myTID = defaultTID(0);
    
    SetActivator(myTID, AAPTR_TARGET);
    if (GetUserVariable(0, "user_impaled")) { terminate; }
    
    SetActivator(myTID, AAPTR_TRACER);
    
    if (CheckFlag(0, "ISMONSTER") && !CheckFlag(0, "BOSS") && !CheckFlag(0, "DONTRIP"))
    {
        int monTID  = defaultTID(0);
        int monX    = GetActorX(0);
        int monY    = GetActorY(0);
        int monZ    = GetActorZ(0);
        int monMass = GetActorProperty(0, APROP_Mass);
        
        SetActorState(0, "Pain");
        
        SetActivator(myTID, AAPTR_TARGET);
        int projX  = GetActorX(0);
        int projY  = GetActorY(0);
        int projZ  = GetActorZ(0);
        int projVX = GetActorVelX(0);
        int projVY = GetActorVelY(0);
        int projVZ = GetActorVelZ(0);
        
        projVX /= (monMass / 150);
        projVY /= (monMass / 150);
        projVZ /= (monMass / 150);
        
        int impaleX = monX - projX - (projVX / 2);
        int impaleY = monY - projY - (projVY / 2);
        int impaleZ = monZ - projZ - (projVZ / 2);
        
        SetPointer(AAPTR_TRACER, monTID);
        SetUserVariable(0, "user_impaled", true);
        SetUserVariable(0, "user_impalex", impaleX / 3);
        SetUserVariable(0, "user_impaley", impaleY / 3);
        SetUserVariable(0, "user_impalez", impaleZ);
        SetUserVariable(0, "user_flytime", 18);
        SetActorVelocity(0, projVX, projVY, projVZ, false, false);
    }
}