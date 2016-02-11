#define TRACE_BULLET        0
#define TRACE_PLASMA        1
#define TRACE_ARC_FIRER     2
#define TRACE_ARC_MASTER    3
#define TRACE_BFG           4
#define TRACE_BFG_INSTANT   5
#define TRACE_BFG_DELAYED   6


// DakkaTracer is the tracer actor
// DakkaTracerBright for lesseffects version

script DAKKA_TRACER (int which, int yoff, int zoff)
{
    int shotX = GetActorX(0);
    int shotY = GetActorY(0);
    int shotZ = GetActorZ(0);

    switch (which)
    {
      default:
        SetActivatorToTarget(0);
        break;

      case TRACE_ARC_MASTER:
        SetActivator(0, AAPTR_MASTER);
        break;
    }

    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }

    int myTID = defaultTID(-1);

    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0) + GetActorViewHeight(myTID);

    int myAngle = GetActorAngle(0);
    int myPitch = GetActorPitch(0);

    int pointX;

    switch (which)
    {
      default:
        pointX = 12.0;
        break;

      case TRACE_PLASMA:
      case TRACE_ARC_FIRER:
      case TRACE_BFG:
        pointX = 20.0;
        break;

      case TRACE_ARC_MASTER:
        pointX = 0;
        break;
    }

    // Negative y should mean left, not right, dammit
    int pointY = -itof(yoff);
    int pointZ =  itof(zoff);


    // Rotation matrix time.
    // See helix.txt to see how I got to this.
    //
    // x' = cos( angle)cos( pitch)x + -sin( angle)y + cos( angle)sin( pitch)z
    // y' = sin(-angle)cos(-pitch)x +  cos(-angle)y + sin(-angle)sin(-pitch)z
    // z' =            sin(-pitch)x                 +            cos(-pitch)z
    
    int rotateX = FixedMul(pointX, FixedMul(cos( myAngle), cos( myPitch)))
                - FixedMul(pointY,          sin( myAngle))
                + FixedMul(pointZ, FixedMul(cos( myAngle), sin( myPitch)));

    int rotateY = FixedMul(pointX, FixedMul(sin( myAngle), cos( myPitch)))
                + FixedMul(pointY,          cos( myAngle))
                + FixedMul(pointZ, FixedMul(sin( myAngle), sin( myPitch)));

    int rotateZ = FixedMul(pointX,                        -sin( myPitch))
                + FixedMul(pointZ,                         cos( myPitch));

    int spawnX = myX + rotateX;
    int spawnY = myY + rotateY;
    int spawnZ = myZ + rotateZ;

    
    int tracerTID1 = unusedTID(15000, 16999);
    int tracerTID2 = unusedTID(17000, 19000);

    SpawnForced("TracerDummy", spawnX, spawnY, spawnZ, tracerTID1);
    SpawnForced("TracerDummy",  shotX,  shotY,  shotZ, tracerTID2);


    switch (which)
    {
      default:
        if (IsServer)
        {
            ACS_ExecuteWithResult(DAKKA_TRACER_CLIENT, which, tracerTID1, tracerTID2);
        }
        else
        {
            ACS_ExecuteAlways(DAKKA_TRACER_CLIENT, 0, which, tracerTID1, tracerTID2);
        }
        break;

      case TRACE_BFG:
        // These will be in dakka_bfg.h
        ACS_ExecuteWithResult(DAKKA_BFGTRACE_SERVER, tracerTID1, tracerTID2);
        
        if (IsServer)
        {
            ACS_ExecuteWithResult(DAKKA_BFGTRACE_CLIENT, tracerTID1, tracerTID2);
        }
        else
        {
            ACS_ExecuteAlways(DAKKA_BFGTRACE_CLIENT, 0, tracerTID1, tracerTID2);
        }
        break;
    }
}


script DAKKA_TRACER_CLIENT (int which, int startTID, int endTID) clientside
{
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

    int particleType = "";
    int density;
    int speed;

    switch (which)
    {
      case TRACE_BULLET:
        if (GetCVar("dakka_cl_notracers") > 0) { break; }

        speed = 512.0;

        if (lesseffects)
        {
            particleType = "DakkaTracerBright";
            density      = 36.0;
        }
        else
        {
            particleType = "DakkaTracer";
            density      = 12.0;
        }
        break;

      case TRACE_PLASMA:
        speed = 0;

        particleType = "DakkaPlasmaTrail";
        density      = cond(lesseffects, 36.0, 12.0);
        break;

      case TRACE_ARC_FIRER:
      case TRACE_ARC_MASTER:
        speed = 0;
        particleType = "ChannelerTrail";
        density      = cond(lesseffects, 32.0, 8.0);
    }

    if (StrLen(particleType) == 0) { terminate; }

    // miraculously, this works on both ints and fixed
    int startPoint = mod(dist, density) / 2;

    int step;
    int ticDistance = startPoint;

    for (step = startPoint; step < dist; step += density)
    {
        int spawnX = startX + FixedMul(step, nX);
        int spawnY = startY + FixedMul(step, nY);
        int spawnZ = startZ + FixedMul(step, nZ);

        SpawnForced(particleType, spawnX, spawnY, spawnZ);

        ticDistance += density;

        if (speed > 0)
        {
            int ticWait = ticDistance / speed;
            Delay(ticWait);
            ticDistance -= (ticWait * speed);
        }
    }
}
