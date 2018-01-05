#define LANCE_TID       0
#define LANCE_STARTX    1
#define LANCE_STARTY    2
#define LANCE_STARTZ    3
#define LANCE_NORMX     4
#define LANCE_NORMY     5
#define LANCE_NORMZ     6
#define LANCE_DISTX     7
#define LANCE_DISTY     8
#define LANCE_DISTZ     9
#define LANCE_LENGTH    10
#define LANCE_RADIUS    11
#define LANCE_DAMAGE    12

int CurLanceData[13];

script "Dakka_InitLanceHit" (int length, int radius, int damage, int isimpaler)
{
    int myTID = defaultTID(0);
    
    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);
    
    int velX, velY, velZ, mag;
    
    if (!GetUserVariable(0, "user_init"))
    {
        mag = GetActorProperty(0, APROP_Speed);
        
        SetActivator(0, AAPTR_TARGET);
        int firerAngle = GetActorAngle(0);
        int firerPitch = GetActorPitch(0);
        
        velX = FixedMul(mag, FixedMul(cos(firerAngle), cos(firerPitch)));
        velY = FixedMul(mag, FixedMul(sin(firerAngle), cos(firerPitch)));
        velZ = FixedMul(mag, -sin(firerPitch));
        
        SetActivator(myTID);
    }
    else
    {
        velx = GetUserVariable(0, "user_velx");
        vely = GetUserVariable(0, "user_vely");
        velz = GetUserVariable(0, "user_velz");
        mag  = VectorLength(VectorLength(velx, vely), velz);
    }
    
    int normX = FixedDiv(velx, mag);
    int normY = FixedDiv(vely, mag);
    int normZ = FixedDiv(velz, mag);
    
    //Log(s:"\cdPlain velocity (init): len ", f:mag, s:" <", f:velX, s:", ", f:velY, s:", ", f:velZ, s:">");
    //Log(s:"\cqNormalized velocity (init): <", f:normX, s:", ", f:normY, s:", ", f:normZ, s:">");
    
    int distX = normX * length;
    int distY = normY * length;
    int distZ = normZ * length;
    
    //SpawnForced("LanceMarker3", myX + distX, myY + distY, myZ + distZ);
    
    CurLanceData[LANCE_TID]    = myTID;
    CurLanceData[LANCE_STARTX] = myX;
    CurLanceData[LANCE_STARTY] = myY;
    CurLanceData[LANCE_STARTZ] = myZ;
    CurLanceData[LANCE_NORMX]  = normX;
    CurLanceData[LANCE_NORMY]  = normY;
    CurLanceData[LANCE_NORMZ]  = normZ;
    CurLanceData[LANCE_DISTX]  = distX;
    CurLanceData[LANCE_DISTY]  = distY;
    CurLanceData[LANCE_DISTZ]  = distZ;
    CurLanceData[LANCE_LENGTH] = itof(length);
    CurLanceData[LANCE_RADIUS] = itof(radius);
    CurLanceData[LANCE_DAMAGE] = damage;
    
    // Always do damage to the thing we hit
    SetActivator(0, AAPTR_TRACER);
    ACS_NamedExecuteWithResult("Dakka_OnLanceHit", 0, 0, isimpaler);
}

        
        
script "Dakka_CheckLanceHit" (int isimpaler)
{
    // First check we aren't the thing directly hit
    int projTID   = CurLanceData[LANCE_TID];
    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);
    
    SetActivator(projTID, AAPTR_TRACER);
    if (ActivatorTID() == myTID_new)
    {
        Thing_ChangeTID(0, myTID_old);
        terminate;
    }
    
    SetActivator(myTID_new);
    Thing_ChangeTID(0, myTID_old);
    
    
    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);
    
    int myWidth  = GetActorProperty(0, APROP_Radius);
    int myHeight = GetActorProperty(0, APROP_Height);
    
    int startX = CurLanceData[LANCE_STARTX];
    int startY = CurLanceData[LANCE_STARTY];
    int startZ = CurLanceData[LANCE_STARTZ];
    
    // Not the closest point, so it won't be 100% accurate, but you
    //  go do fucking vector math in ACS and not pull your hair out
    int adjX = myX                  - startX;
    int adjY = myY                  - startY;
    int adjZ = myZ + (myHeight / 2) - startZ;
    
    int normX = CurLanceData[LANCE_NORMX];
    int normY = CurLanceData[LANCE_NORMY];
    int normZ = CurLanceData[LANCE_NORMZ];
    
    int projectDist = dot3(adjX, adjY, adjZ, normX, normY, normZ);
    
    int projX = FixedMul(normX, projectDist);
    int projY = FixedMul(normY, projectDist);
    int projZ = FixedMul(normZ, projectDist);
    
    int rejX  = adjX - projX;
    int rejY  = adjY - projY;
    int rejZ  = adjZ - projZ;
    
    int rejectDist = magnitudeThree_f(rejX, rejY, rejZ);
    
    int normRejX = FixedDiv(rejX, rejectDist);
    int normRejY = FixedDiv(rejY, rejectDist);
    int normRejZ = FixedDiv(rejZ, rejectDist);
    
    int maxProject = CurLanceData[LANCE_LENGTH];
    int maxReject  = CurLanceData[LANCE_RADIUS];
    
    int projectToUse = middle(0, projectDist, CurLanceData[LANCE_LENGTH]);
    int rejectToUse  = min(rejectDist,  CurLanceData[LANCE_RADIUS]);
    
    int closestX = startX + FixedMul(normX, projectToUse) + FixedMul(normRejX, rejectToUse);
    int closestY = startY + FixedMul(normY, projectToUse) + FixedMul(normRejY, rejectToUse);
    int closestZ = startZ + FixedMul(normZ, projectToUse) + FixedMul(normRejZ, rejectToUse);
    
    if (middle(myX - myWidth, closestX, myX + myWidth)  == closestX
     && middle(myY - myWidth, closestY, myY + myWidth)  == closestY
     && middle(myZ,           closestZ, myZ + myHeight) == closestZ)
    {
        ACS_NamedExecuteWithResult("Dakka_OnLanceHit", projectToUse, rejectToUse, isimpaler);
    }
}



script "Dakka_OnLanceHit" (int projectDist, int rejectDist, int isimpaler)
{   
    int monX      = GetactorX(0);
    int monY      = GetActorY(0);
    int monZ      = GetActorZ(0);
    int monHeight = GetActorProperty(0, APROP_Height);
    
    int projTID   = CurLanceData[LANCE_TID];
    SetActivator(projTID, AAPTR_TARGET);
    
    int firerTID_old = ActivatorTID();
    int firerTID_new = UniqueTID();
    Thing_ChangeTID(0, firerTID_new);
    
    int hurterTID  = UniqueTID();
    str hurterType = cond(isimpaler, "ImpalerPrimaryHurter", "PLPrimaryHurter");
    SpawnForced(hurterType, monX, monY, monZ + (monHeight / 2), hurterTID);
    
    SetActivator(hurterTID);
    SetPointer(AAPTR_TARGET, firerTID_new);
    SetUserVariable(0, "user_damage", CurLanceData[LANCE_DAMAGE]);
    SetActorState(0, "DoHurt");
    Thing_ChangeTID(firerTID_new, firerTID_old);
}


script "Dakka_ArcSpark" (int ptr)
{
    SetActivator(0, ptr);
    
    int myXYRadius = GetActorProperty(0, APROP_Radius);
    int myZRadius  = GetActorProperty(0, APROP_Height) / 2;
    
    int myCenterX  = GetActorX(0);
    int myCenterY  = GetActorY(0);
    int myCenterZ  = GetActorZ(0) + myZRadius;
    
    int sparkTID   = UniqueTID();
    int sparkCount = random(3, 6);
    
    for (int i = 0; i < 5; i++)
    {
        int randX = random(0.1, 0.6) * randSign();
        int randY = random(0.1, 0.6) * randSign();
        int randZ = random(0.1, 0.6) * randSign();
        
        int randAngle = VectorAngle(randX, randY);
        int randPitch = VectorAngle(VectorLength(randX, randY), randZ);
        int randVel   = random(2.0, 6.0);
        
        int spawnX = myCenterX + FixedMul(randX, myXYRadius);
        int spawnY = myCenterY + FixedMul(randY, myXYRadius);
        int spawnZ = myCenterZ + FixedMul(randZ, myZRadius);
        
        SpawnForced("LanceArcMidSpark", spawnX, spawnY, spawnZ);
        //SpawnForced("LanceArcSpark", spawnX, spawnY, spawnZ, sparkTID);
        
        SetActivator(sparkTID);
        SetActorAngle(0, randAngle);
        SetActorPitch(0, randPitch);
        SetActorVelocity(0, FixedMul(FixedMul(cos(randAngle), cos(randPitch)), randVel),
                            FixedMul(FixedMul(sin(randAngle), cos(randPitch)), randVel),
                            FixedMul(sin(randPitch), randVel), false, false);
        Thing_ChangeTID(0, 0);
    }
}