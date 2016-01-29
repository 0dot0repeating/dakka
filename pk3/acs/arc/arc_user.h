script 193 (int damage)
{
    int myTID = defaultTID(-1);

    SetActivator(0, AAPTR_TRACER);
    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }

    int hisTID     = ActivatorTID();
    int hisTID_new = defaultTID(-1);

    SetActivator(myTID, AAPTR_TARGET);

    Thing_Damage2(hisTID_new, damage, "Normal");
    Thing_ChangeTID(hisTID_new, hisTID);
}

script 194 (int useFirer, int zOffset, int aimOffset)
{
    int startX = GetActorX(0);
    int startY = GetActorY(0);
    int startZ = GetActorZ(0);

    // run from Arc_Spawn, the master will be the previous arc

    if (useFirer)
    {
        // okay I guess we'll use target then :(
        SetActivator(0, AAPTR_TARGET);
    }
    else
    {
        SetActivator(0, AAPTR_MASTER);
    }

    // just in case we fukd up
    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }

    int endX = GetActorX(0);
    int endY = GetActorY(0);
    int endZ = GetActorZ(0) + itof(zOffset);

    int ang  = GetActorAngle(0);
    int pit  = GetActorPitch(0);

    int aimX = aimOffset * FixedMul(cos(ang), cos(pit));
    int aimY = aimOffset * FixedMul(sin(ang), cos(pit));
    int aimZ = aimOffset * -sin(pit);

    endX += aimX;
    endY += aimY;
    endZ += aimZ;

    int startTID = unusedTID(13050, 15000);
    int endTID   = unusedTID(15050, 17000);

    SpawnForced("ChannelerBeamDummy", startX, startY, startZ, startTID);
    SpawnForced("ChannelerBeamDummy", endX,   endY,   endZ,   endTID);
    
    if (ConsolePlayerNumber() == -1)
    {
        ACS_ExecuteAlways(195, 0, startTID, endTID);
    }
    else
    {
        // Have it appear instantly in single player - I've actually noticed the
        //  sub-tic delay with beams in the old arc code, and I'm *not* crazy.
        ACS_ExecuteWithResult(195, startTID, endTID);
    }
}

// Distance between beam actors
#define BEAMSTEP_NORM   8.0

// Distance between beam actors with dakka_cl_lesseffects 1
#define BEAMSTEP_LE     32.0

script 195 (int startTID, int endTID) clientside
{
    int x1 = GetActorX(startTID);
    int y1 = GetActorY(startTID);
    int z1 = GetActorZ(startTID);

    int x2 = GetActorX(endTID);
    int y2 = GetActorY(endTID);
    int z2 = GetActorZ(endTID);

    int dx = x2-x1;
    int dy = y2-y1;
    int dz = z2-z1;
    int dmag = magnitudeThree_f(dx,dy,dz);

    int nx = FixedDiv(dx, dmag);
    int ny = FixedDiv(dy, dmag);
    int nz = FixedDiv(dz, dmag);

    int beamStep;

    if (GetCVar("dakka_cl_lesseffects"))
    {
        beamStep = BEAMSTEP_LE;
    }
    else
    {
        beamStep = BEAMSTEP_NORM;
    }

    // Instead of starting from 0, start from a spot so the start of the beam
    //  is the same distance from the start as the end of the beam is from the
    //  end of it
    int beamOffset = (dmag % beamStep) >> 1;

    int i;

    for (i = beamOffset; i < dmag; i += beamStep)
    {
        int x = x1 + FixedMul(nx, i);
        int y = y1 + FixedMul(ny, i);
        int z = z1 + FixedMul(nz, i);

        SpawnForced("ChannelerTrail", x,y,z);
    }
}
