#define TRACE_BULLET        0
#define TRACE_BULLET_BIG    1
#define TRACE_ARC_FIRER     2
#define TRACE_ARC_MASTER    3


script "Dakka_Tracer" (int which, int yoff, int zoff, int fractional)
{
    if (!IsServer) { terminate; }

    int shotX = GetActorX(0);
    int shotY = GetActorY(0);
    int shotZ = GetActorZ(0);

    switch (which)
    {
      default:
        SetActivator(0, AAPTR_TARGET);
        break;

      case TRACE_ARC_MASTER:
        SetActivator(0, AAPTR_MASTER);
        break;
    }

    if (IsWorld()) { terminate; }

    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0) + GetActorViewHeight(0);

    int myAngle = GetActorAngle(0);
    int myPitch = GetActorPitch(0);

    int pointX;

    switch (which)
    {
      default:
        pointX = 12.0;
        break;

      case TRACE_ARC_FIRER:
        pointX = 20.0;
        break;

      case TRACE_ARC_MASTER:
        pointX = 0;
        break;
    }
    
    // Flip the Y coordinate; negative y should mean left, not right, dammit
    int pointY, pointZ;
    
    if (fractional)
    {
        pointY = -percFloat(0, yoff);
        pointZ =  percFloat(0, zoff);
    }
    else
    {
        pointY = -itof(yoff);
        pointZ =  itof(zoff);
    }

    Rotate3D(pointX, pointY, pointZ, myAngle, myPitch);
    int rotX = Rotate3D_Ret[0];
    int rotY = Rotate3D_Ret[1];
    int rotZ = Rotate3D_Ret[2];
    
    int spawnX = myX + rotX;
    int spawnY = myY + rotY;
    int spawnZ = myZ + rotZ;
    
    int dx = shotX - spawnX;
    int dy = shotY - spawnY;
    int dz = shotZ - spawnZ;
    
    int tracerAngle =  VectorAngle(dx, dy);
    int tracerPitch = -VectorAngle(VectorLength(dx, dy), dz);
    int tracerDist  =  VectorLength(VectorLength(dx, dy), dz);

    int tracerTID = UniqueTID();
    SpawnForced("TracerDummy", spawnX, spawnY, spawnZ, tracerTID);
    SetActorAngle(tracerTID, tracerAngle);

    switch (which)
    {
      default:
        if (ConsolePlayerNumber() == -1)
        {
            ACS_NamedExecuteAlways("Dakka_Tracer_Client", 0, tracerTID, tracerDist, tracerPitch);
        }
        else
        {
            ACS_NamedExecuteWithResult("Dakka_Tracer_Client", tracerTID, tracerDist, tracerPitch);
        }
        break;
        
      case TRACE_BULLET_BIG:
        if (ConsolePlayerNumber() == -1)
        {
            ACS_NamedExecuteAlways("Dakka_TracerBig_Client", 0, tracerTID, tracerDist, tracerPitch);
        }
        else
        {
            ACS_NamedExecuteWithResult("Dakka_Tracer_Client", tracerTID, tracerDist, tracerPitch, TRACE_BULLET_BIG);
        }
        break;

      case TRACE_ARC_FIRER:
      case TRACE_ARC_MASTER:
        SetActorPitch(tracerTID, tracerPitch);
        if (which == TRACE_ARC_MASTER) { SetActivator(0, AAPTR_TARGET); }
        
        if (ConsolePlayerNumber() == -1)
        {
            ACS_NamedExecuteAlways("Dakka_Lightning", 0, which, tracerTID, tracerDist);
        }
        else
        {
            ACS_NamedExecuteWithResult("Dakka_Lightning", which, tracerTID, tracerDist);
        }
        break;
    }
}


script "Dakka_TracerBig_Client" (int startTID, int dist, int startPitch) clientside
{
    ACS_NamedExecuteWithResult("Dakka_Tracer_Client", startTID, dist, startPitch, TRACE_BULLET_BIG);
}

script "Dakka_Tracer_Client" (int startTID, int dist, int startPitch, int type) clientside
{
    int firerPln = PlayerNumber();
    int pln = cond(IsZandronum, ConsolePlayerNumber(), firerPln);
    int tracermode = GetUserCVar(pln, "dakka_cl_tracermode");
    
    if (tracermode <= 0) { terminate; }
    if (tracermode == 1 && pln != firerPln) { terminate; }
    
    int waitTimer = 0;

    while (!IsTIDUsed(startTID))
    {
        waitTimer++;
        Delay(1);
        if (waitTimer > 36) { terminate; }
    }

    int startX     = GetActorX(startTID);
    int startY     = GetActorY(startTID);
    int startZ     = GetActorZ(startTID);
    int startAngle = GetActorAngle(startTID);
    
    if (!GetCVar("cl_capfps"))
    {
        // Interpolation makes tracers appear ahead of you, so push it back
        startX -= GetActorVelX(0);
        startY -= GetActorVelY(0);
        startZ -= GetActorVelZ(0);
    }

    int dX = FixedMul(dist, FixedMul(cos(startAngle), cos(startPitch)));
    int dY = FixedMul(dist, FixedMul(sin(startAngle), cos(startPitch)));
    int dZ = FixedMul(dist, -sin(startPitch));

    int nX = FixedDiv(dX, dist);
    int nY = FixedDiv(dY, dist);
    int nZ = FixedDiv(dZ, dist);
    
    
    int step, density;
    int speed = itof(middle(64, GetUserCVar(pln, "dakka_cl_tracerspeed"), 2048));

    switch (middle(-4, GetUserCVar(pln, "dakka_cl_tracerdensity"), 4))
    {
        case -4: density = 36.0; break;
        case -3: density = 30.0; break;
        case -2: density = 24.0; break;
        case -1: density = 18.0; break;
        default: density = 12.0; break;
        case  1: density = 8.0;  break;
        case  2: density = 6.0;  break;
        case  3: density = 4.8;  break;
        case  4: density = 4.0;  break;
    }

    // miraculously, this works on both ints and fixed
    int startPoint  = mod(dist, density) / 2;
    int ticDistance = startPoint;
    
    int actualParticles = GetUserCVar(pln, "dakka_cl_particletracers");
    
    if (actualParticles)
    {
        int particleSize = cond(type == TRACE_BULLET_BIG, 6,4);
    }
    else
    {
        str particleType = cond(type == TRACE_BULLET_BIG, "DakkaBigTracer", "DakkaTracer");
        int particleTID = UniqueTID();
    }

    for (step = startPoint; step < dist; step += density)
    {
        int spawnX = startX + FixedMul(step, nX);
        int spawnY = startY + FixedMul(step, nY);
        int spawnZ = startZ + FixedMul(step, nZ);
        
        int alphaScalar = FixedDiv(ticDistance, speed);
        
        if (actualParticles)
        {
            int startAlpha = 128 + FixedMul(128, alphaScalar);
            SpawnParticle(0xfff68c, true, 2, particleSize, spawnX, spawnY, spawnZ, 0,0,0, 0,0,0, startAlpha, 128);
        }
        else
        {
            SpawnForced(particleType, spawnX, spawnY, spawnZ, particleTID);
            SetActorProperty(particleTID, APROP_Alpha, alphaScalar);
            Thing_ChangeTID(particleTID, 0);
        }

        ticDistance += density;

        if (speed > 0)
        {
            int ticWait = ticDistance / speed;
            Delay(ticWait);
            ticDistance -= (ticWait * speed);
        }
    }
}

