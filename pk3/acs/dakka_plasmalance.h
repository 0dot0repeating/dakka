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

script "Dakka_InitLanceHit" (int length, int radius, int damage)
{
    int myTID = defaultTID(0);
    
    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);
    
    int velx = itof(GetUserVariable(0, "user_velx")) / 100;
    int vely = itof(GetUserVariable(0, "user_vely")) / 100;
    int velz = itof(GetUserVariable(0, "user_velz")) / 100;
    int mag = magnitudeThree_f(velx, vely, velz);
    
    int normX = FixedDiv(velx, mag);
    int normY = FixedDiv(vely, mag);
    int normZ = FixedDiv(velz, mag);
    
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
}

        
        
script "Dakka_CheckLanceHit" (int isimpaler)
{
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
    
    //Log(s:"Project dist: ", f:projectDist, s:" <", f:projX, s:", ", f:projY, s:", ", f:projZ, s:">");
    //Log(s:"Reject dist: ",  f:rejectDist,  s:" <", f:rejX,  s:", ", f:rejY,  s:", ", f:rejZ,  s:">");
    //SpawnForced("LanceMarker1", startX + projX, startY + projY, startZ + projZ);
    //SpawnForced("LanceMarker2", startX + rejX,  startY + rejY,  startZ + rejZ);
    
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
        //SpawnForced("LanceMarker3", closestX, closestY, closestZ);
        ACS_NamedExecuteWithResult("Dakka_OnLanceHit", projectToUse, rejectToUse, isimpaler);
    }
}



script "Dakka_OnLanceHit" (int projectDist, int rejectDist, int isimpaler)
{
    int oldMonTID = ActivatorTID();
    int monTID    = defaultTID(0);
    int projTID   = CurLanceData[LANCE_TID];
    
    SetActivator(projTID, AAPTR_TARGET);
    int hurterTID = UniqueTID();
    int myTID     = defaultTID(0);
    
    str hurterType = "PLPrimaryHurter";
    if (isimpaler) { hurterType = "ImpalerPrimaryHurter"; }
    SpawnForced(hurterType, GetActorX(monTID), GetActorY(monTID), GetActorZ(monTID) + (GetActorProperty(monTID, APROP_Height) / 2), hurterTID);
    
    SetActivator(hurterTID);
    SetPointer(AAPTR_TARGET, myTID);
    SetUserVariable(0, "user_damage", CurLanceData[LANCE_DAMAGE]);
    
    SetActivator(monTID);
    Thing_ChangeTID(0, oldMonTID);
    SetActorState(hurterTID, "DoHurt");
}