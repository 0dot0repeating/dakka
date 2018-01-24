// Yes I just copypasted DAKKA_TRACER_CLIENT. Fuckin' sue me, I'm not cramming
//  all of the BFG's stuff into that function.

#define BFG_HELIXRADIUS_X   16.0
#define BFG_HELIXRADIUS_Y   16.0
#define BFG_HELIX_PERIOD    1024.0
#define BFG_HELIX_SPEED     128.0
#define BFG_HELIX_STEP      16.0

#define BFG_INSTANT_STEP    16.0

script "Dakka_BFG_Server" (int startTID, int endTID)
{
    if (!IsServer) { terminate; }
    
    int myTID   = defaultTID(-1);
    int shotTID = unusedTID(12000, 16000);

    int startX = GetActorX(startTID);
    int startY = GetActorY(startTID);
    int startZ = GetActorZ(startTID);

    int endX   = GetActorX(endTID);
    int endY   = GetActorY(endTID);
    int endZ   = GetActorZ(endTID);

    int dX = endX - startX;
    int dY = endY - startY;
    int dZ = endZ - startZ;

    int dist = magnitudeThree_f(dX, dY, dZ);

    int nX = FixedDiv(dX, dist);
    int nY = FixedDiv(dY, dist);
    int nZ = FixedDiv(dZ, dist);

    int density = BFG_INSTANT_STEP;

    // miraculously, this works on both ints and fixed
    int startPoint = mod(dist, density) / 2;

    int step;

    for (step = startPoint; step < dist; step += density)
    {
        int spawnX = startX + FixedMul(step, nX);
        int spawnY = startY + FixedMul(step, nY);
        int spawnZ = startZ + FixedMul(step, nZ);

        SpawnForced("DakkaBFGInstantBeam", spawnX, spawnY, spawnZ, shotTID);

        SetActivator(shotTID);
        SetPointer(AAPTR_TARGET, myTID);
        Thing_ChangeTID(0, 0);
        SetActivator(myTID);
    }

    density     = BFG_HELIX_STEP;
    startPoint  = mod(dist, density) / 2;

    int speed = BFG_HELIX_SPEED;
    int ticDistance = 0;

    for (step = startPoint; step < dist; step += density)
    {
        spawnX = startX + FixedMul(step, nX);
        spawnY = startY + FixedMul(step, nY);
        spawnZ = startZ + FixedMul(step, nZ);

        SpawnForced("DakkaBFGMainBeam", spawnX, spawnY, spawnZ, shotTID);

        SetActivator(shotTID);
        SetPointer(AAPTR_TARGET, myTID);
        Thing_ChangeTID(0, 0);
        SetActivator(myTID);

        ticDistance += density;
        int ticWait = ticDistance / speed;
        ticDistance -= (ticWait * speed);

        Delay(ticWait);
    }
}

script "Dakka_BFG_Client" (int startTID, int endTID)
{
    if (!IsServer) { terminate; }
    
    int myTID    = defaultTID(-1);
    int helixTID = unusedTID(12000, 16000);

    int startX = GetActorX(startTID);
    int startY = GetActorY(startTID);
    int startZ = GetActorZ(startTID);

    int endX   = GetActorX(endTID);
    int endY   = GetActorY(endTID);
    int endZ   = GetActorZ(endTID);

    int dX = endX - startX;
    int dY = endY - startY;
    int dZ = endZ - startZ;

    int dist = magnitudeThree_f(dX, dY, dZ);

    int nX = FixedDiv(dX, dist);
    int nY = FixedDiv(dY, dist);
    int nZ = FixedDiv(dZ, dist);

    int lesseffects = GetCVar("dakka_cl_lesseffects");

    int beamInstant = "DakkaBFGBeam_Client";
    int beamCenter  = "DakkaBFGBeam_HelixCenter";
    int beamHelix   = "DakkaBFGBeam_Helix";

    int skipHelix = false;

    if (lesseffects)
    {
        beamCenter = "DakkaBFGBeam_HelixCenter_LessEffects";
        skipHelix  = true;
    }

    int density = BFG_INSTANT_STEP;

    // miraculously, this works on both ints and fixed
    int startPoint = mod(dist, density) / 2;

    int step;

    for (step = startPoint; step < dist; step += density)
    {
        int spawnX = startX + FixedMul(step, nX);
        int spawnY = startY + FixedMul(step, nY);
        int spawnZ = startZ + FixedMul(step, nZ);

        SpawnForced(beamInstant, spawnX, spawnY, spawnZ);
    }

    density     = BFG_HELIX_STEP;
    startPoint  = mod(dist, density) / 2;

    int speed = BFG_HELIX_SPEED;
    int ticDistance = 0;


    // Precalculate matrix multipliers for helix rotation.
    // look at helix.txt for how I got to what I did
    
    int angle =  VectorAngle(nX, nY);

    // Think of side A as the line [(0, 0), (sqrt(nX**2 + nY**2), 0)],
    //      and side B as the line [(0, 0), (0, nZ)]
    int pitch = -VectorAngle(magnitudeTwo_f(nX, nY), nZ);

    int mx_x = FixedMul(cos(angle), cos(pitch));
    int mx_y = -sin(angle);
    int mx_z = FixedMul(cos(angle), sin(pitch));

    int my_x = FixedMul(sin(angle), cos(pitch));
    int my_y = cos(angle);
    int my_z = FixedMul(sin(angle), sin(pitch));

    int mz_x = -sin(pitch);
    int mz_z = cos(pitch);



    for (step = startPoint; step < dist; step += density)
    {
        // Center of the beam
        spawnX = startX + FixedMul(step, nX);
        spawnY = startY + FixedMul(step, nY);
        spawnZ = startZ + FixedMul(step, nZ);

        SpawnForced(beamCenter, spawnX, spawnY, spawnZ);

        // Beam helix
        if (!skipHelix)
        {
            int helixAngle = FixedDiv(step, BFG_HELIX_PERIOD);
            int hx = step;
            int hy = FixedMul(sin(helixAngle), BFG_HELIXRADIUS_X);
            int hz = FixedMul(cos(helixAngle), BFG_HELIXRADIUS_Y);

            int ex = FixedMul(hx, mx_x) + FixedMul(hy, mx_y) + FixedMul(hz, mx_z);
            int ey = FixedMul(hx, my_x) + FixedMul(hy, my_y) + FixedMul(hz, my_z);
            int ez = FixedMul(hx, mz_x)                      + FixedMul(hz, mz_z);

            SpawnForced(beamHelix, startX+ex, startY+ey, startZ+ez, helixTID);

            // Apply extra velocity to it
            int randRadius = random(0.9, 1.25);
            helixAngle += random(-0.05, 0.05);

            hy = FixedMul(sin(helixAngle), FixedMul(randRadius, BFG_HELIXRADIUS_X));
            hz = FixedMul(cos(helixAngle), FixedMul(randRadius, BFG_HELIXRADIUS_Y));

            ex = FixedMul(hy, mx_y) + FixedMul(hz, mx_z);
            ey = FixedMul(hy, my_y) + FixedMul(hz, my_z);
            ez =                      FixedMul(hz, mz_z);

            SetActorVelocity(helixTID, ex / 16, ey / 16, ez / 16, 0,0);
            Thing_ChangeTID(helixTID, 0);
        }


        // Speed delay
        ticDistance += density;
        int ticWait = ticDistance / speed;
        ticDistance -= (ticWait * speed);

        Delay(ticWait);
    }
}


#define BFGPICKFLAGS_THINGS (MF_SHOOTABLE)
#define BFGPICKFLAGS_LINES  (ML_BLOCKING | ML_BLOCKEVERYTHING | ML_BLOCKPROJECTILE)

function int Dakka_GetNewTarget(int ptrTID)
{
    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);
    
    if (ptrTID == 0)
    {
        ptrTID = UniqueTID();
        SpawnForced("BFGPointerDummy", GetActorX(0), GetActorY(0), GetActorZ(0), ptrTID);
        
        SetActivator(ptrTID);
        SetPointer(AAPTR_TARGET, myTID_new);
        SetActivator(myTID_new);
    }
    
    int myAngle = GetActorAngle(0);
    int myPitch = GetActorPitch(0);
    
    int targetTID_old = PickActor(0, myAngle, myPitch, 0x7FFFFFFF, 0, BFGPICKFLAGS_THINGS, BFGPICKFLAGS_LINES, PICKAF_RETURNTID);
    int targetTID_new = UniqueTID();
    
    int t = Timer();
    
    if (PickActor(0, myAngle, myPitch, 0x7FFFFFFF, targetTID_new, BFGPICKFLAGS_THINGS, BFGPICKFLAGS_LINES, PICKAF_FORCETID))
    {
        SetActivator(ptrTID);
        SetPointer(AAPTR_TRACER, targetTID_new);
        SetUserVariable(0, "user_timeout", t + 36);
        
        SetActivator(myTID_new);
        Thing_ChangeTID(targetTID_new, targetTID_old);
    }
    else
    {
        SetActivator(ptrTID, AAPTR_TRACER);
        int targetDead = (ClassifyActor(0) & ACTOR_WORLD) || isDead(0);
        int timedOut   = GetUserVariable(ptrTID, "user_timeout") <= t;
        SetActivator(myTID_new);
        
        if (targetDead || timedOut)
        {
            // I feel like this shouldn't work because this starts right on top of us
            //   and yet it does work
            
            SpawnForced("BFGCrosshairTarget", GetActorX(0), GetActorY(0), GetActorZ(0) + GetActorViewHeight(0), targetTID_new);
            SetActorVelocity(targetTID_new, 32767 * FixedMul(cos(myAngle), cos(myPitch)),
                                            32767 * FixedMul(sin(myAngle), cos(myPitch)),
                                            32767 * -sin(myPitch), false, false);
            
            SetActivator(ptrTID);
            SetPointer(AAPTR_TRACER, targetTID_new);
            Thing_ChangeTID(targetTID_new, 0);
            
            SetActivator(myTID_new);
        }
    }

    SetActorState(ptrTID, "KeepAlive");
    SetInventory("BFGPointerTID", ptrTID);
    Thing_ChangeTID(myTID_new, myTID_old);
    return ptrTID;
}



script "Dakka_SetupBFGStage2" (int facetracer, int speedadd, int speedmax, int evenIfBehind)
{
    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);
    
    SetActivator(0, AAPTR_TARGET);
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        SetResultValue(false);
        terminate;
    }
    
    SetActivator(CheckInventory("BFGPointerTID"), AAPTR_TRACER);
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        SetResultValue(false);
        terminate;
    }
    
    int targetTID_old = ActivatorTID();
    int targetTID_new = UniqueTID();
    Thing_ChangeTID(0, targetTID_new);
    
    SetActivator(myTID_new);
    SetPointer(AAPTR_TRACER, targetTID_new);
    
    int dx = GetActorX(targetTID_new) - GetActorX(0);
    int dy = GetActorY(targetTID_new) - GetActorY(0);
    int dz = GetActorZ(targetTID_new) - GetActorZ(0) + (GetActorProperty(targetTID_new, APROP_Height) / 2);
    
    if (facetracer)
    {
        SetActorAngle(0,  VectorAngle(dx, dy));
        SetActorPitch(0, -VectorAngle(VectorLength(dx, dy), dz));
    }
    
    Thing_ChangeTID(myTID_new,     myTID_old);
    Thing_ChangeTID(targetTID_new, targetTID_old);
    SetResultValue(true);
}



script "Dakka_BFGHomeIn" (int speedadd, int speedmax, int evenIfBehind)
{
    speedadd = itof(speedadd);
    speedmax = itof(speedmax);
    
    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);
    
    int x = GetActorX(0);
    int y = GetActorY(0);
    int z = GetActorZ(0);
    
    int angle = GetActorAngle(0);
    int pitch = GetActorPitch(0);
    
    int aimX = FixedMul(cos(angle), cos(pitch));
    int aimY = FixedMul(sin(angle), cos(pitch));
    int aimZ = -sin(pitch);
    
    int tx,ty,tz;
    
    SetActivator(0, AAPTR_TRACER);
    
    if ((ClassifyActor(0) & ACTOR_WORLD) || isDead(0))
    {
        if (GetUserVariable(myTID_new, "user_hadtarget"))
        {
            tx = GetUserVariable(myTID_new, "user_targetx");
            ty = GetUserVariable(myTID_new, "user_targety");
            tz = GetUserVariable(myTID_new, "user_targetz");
        }
        else // I guess just keep going then?
        {
            tx = x + FixedMul(speedmax, aimX);
            ty = y + FixedMul(speedmax, aimY);
            tz = z + FixedMul(speedmax, aimZ);
        }
    }
    else
    {
        tx = GetActorX(0);
        ty = GetActorY(0);
        tz = GetActorZ(0) + (GetActorProperty(0, APROP_Height) / 2);
        
        SetUserVariable(myTID_new, "user_hadtarget", true);
        SetUserVariable(myTID_new, "user_targetx", tx);
        SetUserVariable(myTID_new, "user_targety", ty);
        SetUserVariable(myTID_new, "user_targetz", tz);
    }
    
    int dx = tx - x;
    int dy = ty - y;
    int dz = tz - z;
    
    SetActivator(myTID_new);
    
    if (!evenIfBehind)
    {
        int distInFront = dot3(dx,dy,dz, aimX,aimY,aimZ);
        
        if (distInFront < 0)
        {
            SetResultValue(false);
            Thing_ChangeTID(myTID_new, myTID_old);
            terminate;
        }
    }
    
    int dmag = VectorLength(VectorLength(dx, dy), dz);
    int ndx = 0, ndy = 0, ndz = 0;
    
    if (dmag > 0)
    {
        ndx = FixedDiv(dx, dmag);
        ndy = FixedDiv(dy, dmag);
        ndz = FixedDiv(dz, dmag);
    }
    
    int vx  = GetActorVelX(0) + FixedMul(ndx, speedadd);
    int vy  = GetActorVelY(0) + FixedMul(ndy, speedadd);
    int vz  = GetActorVelZ(0) + FixedMul(ndz, speedadd);
    int vel = VectorLength(VectorLength(vx, vy), vz);
    
    if (speedmax > 0 && vel > speedmax)
    {
        int vdiff = FixedDiv(speedmax, vel);
        vx = FixedMul(vx, vdiff);
        vy = FixedMul(vy, vdiff);
        vz = FixedMul(vz, vdiff);
    }
    
    SetActorVelocity(0, vx,vy,vz, false,false);
    SetActorAngle(0,  VectorAngle(vx,vy));
    SetActorPitch(0, -VectorAngle(VectorLength(vx,vy), vz));
    
    SetResultValue(true);
}