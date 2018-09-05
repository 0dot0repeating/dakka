#define LIGHTNING_X     0
#define LIGHTNING_Y     1
#define LIGHTNING_Z     2
#define LIGHTNING_DIST  3
#define LIGHTNING_LEN   4

#define LIGHTNINGSLOTS  256
#define MAXPOINTS       256


script "Dakka_LightningStart" (int x, int y, int z, int type) clientside
{
    // TracerData is in dakka_tracers.h
    TracerData[0][0] = true;
    TracerData[0][1] = x;
    TracerData[0][2] = y;
    TracerData[0][3] = z;
    TracerData[0][4] = type;
    
    if (TracerData[0][0] && TracerData[1][0])
    {
        ACS_NamedExecuteWithResult("Dakka_Lightning");
        TracerData[0][0] = false;
        TracerData[1][0] = false;
    }
}


script "Dakka_LightningEnd" (int x, int y, int z) clientside
{
    TracerData[1][0] = true;
    TracerData[1][1] = x;
    TracerData[1][2] = y;
    TracerData[1][3] = z;
    
    if (TracerData[0][0] && TracerData[1][0])
    {
        ACS_NamedExecuteWithResult("Dakka_Lightning");
        TracerData[0][0] = false;
        TracerData[1][0] = false;
    }
}


script "Dakka_LightningStart_Firer" (int x, int y, int z) clientside
{
    ACS_NamedExecuteWithResult("Dakka_LightningStart", x,y,z, TRACE_ARC_FIRER);
}


script "Dakka_LightningStart_Master" (int x, int y, int z) clientside
{
    ACS_NamedExecuteWithResult("Dakka_LightningStart", x,y,z, TRACE_ARC_MASTER);
}



int Lightning_Points[LIGHTNINGSLOTS][MAXPOINTS][5];
int Lightning_InUse[LIGHTNINGSLOTS];

script "Dakka_Lightning" (void) clientside
{
    int pln         = cond(IsZandronum, ConsolePlayerNumber(), PlayerNumber());
    int lesseffects = GetUserCVar(pln, "dakka_cl_lesseffects");

    int startX = TracerData[0][1];
    int startY = TracerData[0][2];
    int startZ = TracerData[0][3];
    
    int endX   = TracerData[1][1];
    int endY   = TracerData[1][2];
    int endZ   = TracerData[1][3];
    
    int which  = TracerData[0][4];

    int dX   = endX - startX;
    int dY   = endY - startY;
    int dZ   = endZ - startZ;
    int dist = VectorLength(VectorLength(dX, dY), dZ);

    int nX = FixedDiv(dX, dist);
    int nY = FixedDiv(dY, dist);
    int nZ = FixedDiv(dZ, dist);
    
    int startAngle =  VectorAngle(dX, dY);
    int startPitch = -VectorAngle(VectorLength(dX, dY), dZ);

    int particleType = "LanceArcTrail";
    int density = cond(lesseffects, 18.0, 6.0);
    int speed   = 0;

    int pointMin,  pointMax;
    int rotateMin, rotateMax;
    int radiusMin, radiusMax;

    switch (which)
    {
      case TRACE_ARC_FIRER:
        pointMin = 32.0;
        pointMax = 128.0;

        rotateMin = 0.25;
        rotateMax = 0.75;

        radiusMin = 2.0;
        radiusMax = 6.0;
        break;

      case TRACE_ARC_MASTER:
        pointMin = 16.0;
        pointMax = 64.0;

        rotateMin = 0.35;
        rotateMax = 0.65;

        radiusMin = 6.0;
        radiusMax = 12.0;
        break;
    }



    // Precalculate matrix multipliers to rotate lightning points into place
    // look at helix.txt for how I got to what I did

    int mx_x = FixedMul(cos(startAngle), cos(startPitch));
    int mx_y = -sin(startAngle);
    int mx_z = FixedMul(cos(startAngle), sin(startPitch));

    int my_x = FixedMul(sin(startAngle), cos(startPitch));
    int my_y = cos(startAngle);
    int my_z = FixedMul(sin(startAngle), sin(startPitch));

    int mz_x = -sin(startPitch);
    int mz_z = cos(startPitch);

    int curPointX = startX;
    int curPointY = startY;
    int curPointZ = startZ;

    int pointCount = 0;
    int pointDist  = 0;
    int newPointAngle = random(0, 1.0);

    // find free lightning slot

    int lightningSlot = -1;
    int i;

    for (i = 0; i < LIGHTNINGSLOTS; i++)
    {
        if (!Lightning_InUse[i])
        {
            lightningSlot = i;
            Lightning_InUse[i] = true;
            break;
        }
    }

    if (lightningSlot == -1)
    {
        Log(s:"\cgERROR: \cfDakka_Lightning: no more lightning slots");
        terminate;
    }

    // all points from the start up until the one leading to the target
    while (true)
    {
        Lightning_Points[lightningSlot][pointCount][LIGHTNING_X]    = curPointX;
        Lightning_Points[lightningSlot][pointCount][LIGHTNING_Y]    = curPointY;
        Lightning_Points[lightningSlot][pointCount][LIGHTNING_Z]    = curPointZ;
        Lightning_Points[lightningSlot][pointCount][LIGHTNING_DIST] = pointDist;

        pointCount += 1;
        pointDist += random(pointMin, pointMax);

        // always reach end point without running out of points
        if (pointCount + 1 >= MAXPOINTS)
        {
            Log(s:"\caWARNING: \cfDakka_Lightning: ran out of points, going straight to end");
            break;
        }

        // do final point
        if (pointDist > dist) { break; }

        // calc position of next point
        int lastAngle = newPointAngle;
        newPointAngle  = mod(lastAngle + random(rotateMin, rotateMax), 1.0);

        int newPointRadius = random(radiusMin, radiusMax);

        int newPointX = pointDist;
        int newPointY = FixedMul(cos(newPointAngle), newPointRadius);
        int newPointZ = FixedMul(sin(newPointAngle), newPointRadius);

        // rotate into place
        curPointX = startX + FixedMul(newPointX, mx_x) + FixedMul(newPointY, mx_y) + FixedMul(newPointZ, mx_z);
        curPointY = startY + FixedMul(newPointX, my_x) + FixedMul(newPointY, my_y) + FixedMul(newPointZ, my_z);
        curPointZ = startZ + FixedMul(newPointX, mz_x)                             + FixedMul(newPointZ, mz_z);
    }

    // final point, to target
    Lightning_Points[lightningSlot][pointCount][LIGHTNING_X] = endX;
    Lightning_Points[lightningSlot][pointCount][LIGHTNING_Y] = endY;
    Lightning_Points[lightningSlot][pointCount][LIGHTNING_Z] = endZ;
    Lightning_Points[lightningSlot][pointCount][LIGHTNING_DIST] = dist;

    pointCount += 1;

    // calculate total length

    int totalLength = 0;
    int lastPointX, lastPointY, lastPointZ;

    for (i = 0; i < pointCount - 1; i++)
    {
        lastPointX = Lightning_Points[lightningSlot][i][LIGHTNING_X];
        lastPointY = Lightning_Points[lightningSlot][i][LIGHTNING_Y];
        lastPointZ = Lightning_Points[lightningSlot][i][LIGHTNING_Z];

        curPointX = Lightning_Points[lightningSlot][i+1][LIGHTNING_X];
        curPointY = Lightning_Points[lightningSlot][i+1][LIGHTNING_Y];
        curPointZ = Lightning_Points[lightningSlot][i+1][LIGHTNING_Z];

        pointDist = VectorLength(VectorLength(lastPointX - curPointX, lastpointY - curPointY), lastPointZ - curPointZ);

        Lightning_Points[lightningSlot][i][LIGHTNING_LEN] = pointDist;

        totalLength += pointDist;
    }

    // miraculously, this works on both ints and fixed
    int startPoint = mod(totalLength, density) / 2;

    // track how far the beam has gone from the firer (for speed)
    int currentDist = 0;

    // tracks the total length of the segments we've traversed (for particle placement)
    int currentSegLength = 0;

    // tracks the total distance we've gone down the beam
    int currentLength = startPoint;

    // keep track of how far we've gone since the last delay
    //  based off distance from firer, not beam length
    int delayDistance = 0;

    // draw each point now
    for (i = 0; i < pointCount - 1; i++)
    {
        int startPointX     = Lightning_Points[lightningSlot][i][LIGHTNING_X];
        int startPointY     = Lightning_Points[lightningSlot][i][LIGHTNING_Y];
        int startPointZ     = Lightning_Points[lightningSlot][i][LIGHTNING_Z];
        int startPointDist  = Lightning_Points[lightningSlot][i][LIGHTNING_DIST];

        int endPointX       = Lightning_Points[lightningSlot][i+1][LIGHTNING_X];
        int endPointY       = Lightning_Points[lightningSlot][i+1][LIGHTNING_Y];
        int endPointZ       = Lightning_Points[lightningSlot][i+1][LIGHTNING_Z];
        int endPointDist    = Lightning_Points[lightningSlot][i+1][LIGHTNING_DIST];

        int pointDiffX  = endPointX - startPointX;
        int pointDiffY  = endPointY - startPointY;
        int pointDiffZ  = endPointZ - startPointZ;

        int length = Lightning_Points[lightningSlot][i][LIGHTNING_LEN];

        int distTraveled = endPointDist - startPointDist;
        int distStep = FixedDiv(distTraveled, length);

        int pointNormX  = FixedDiv(pointDiffX, length);
        int pointNormY  = FixedDiv(pointDiffY, length);
        int pointNormZ  = FixedDiv(pointDiffZ, length);

        int startOffset = currentLength - currentSegLength;

        int j;

        // add delay distance from start of segment to first particle
        delayDistance += FixedMul(distStep, startOffset);

        // particles between each point
        for (j = startOffset; j < length; j += density)
        {
            if (speed > 0)
            {
                Delay(delayDistance / speed);
                delayDistance = mod(delayDistance, speed);
            }

            currentLength += density;

            int particleX = startPointX + FixedMul(pointNormX, j);
            int particleY = startPointY + FixedMul(pointNormY, j);
            int particleZ = startPointZ + FixedMul(pointNormZ, j);

            SpawnForced(particleType, particleX, particleY, particleZ);

            delayDistance += FixedMul(distStep, density);
        }

        // roll j back one step
        j -= density;

        // add delay distance from last particle to end of segment
        delayDistance += FixedMul(distStep, length - j);

        currentSegLength += length;
    }

    Lightning_InUse[lightningSlot] = false;
}

