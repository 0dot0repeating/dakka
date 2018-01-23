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
    
    int targetTID_old = PickActor(0, myAngle, myPitch, 0x7FFFFFFF, 0, MF_SHOOTABLE, ML_BLOCKING | ML_BLOCKEVERYTHING, PICKAF_RETURNTID);
    int targetTID_new = UniqueTID();
    
    if (PickActor(0, myAngle, myPitch, 0x7FFFFFFF, targetTID_new, MF_SHOOTABLE, ML_BLOCKING | ML_BLOCKEVERYTHING, PICKAF_FORCETID))
    {
        SetActivator(ptrTID);
        SetPointer(AAPTR_TRACER, targetTID_new);
        SetUserVariable(ptrTID, "user_timeout", 36);
        
        SetActivator(myTID_new);
        Thing_ChangeTID(targetTID_new, targetTID_old);
    }
    else
    {
        SetUserVariable(ptrTID, "user_timeout", max(0, GetUserVariable(ptrTID, "user_timeout") - 1));
        
        if (GetUserVariable(ptrTID, "user_timeout") == 0)
        {
            SetActivator(ptrTID);
            SetPointer(AAPTR_TRACER, 0);
            SetActivator(myTID_new);
        }
    }
    
    SetActorState(ptrTID, "KeepAlive");
    SetInventory("BFGPointerTID", ptrTID);
    Thing_ChangeTID(myTID_new, myTID_old);
    return ptrTID;
}