#define ATARGET_TYPE  0
#define ATARGET_TID   1
#define ATARGET_POSX  2
#define ATARGET_POSY  3
#define ATARGET_POSZ  4

#define ATARGET_ANGLE 5
#define ATARGET_PITCH 6
#define ATARGET_AIMX  7
#define ATARGET_AIMY  8
#define ATARGET_AIMZ  9

#define ATARGET_PLAYERNUM   10
#define ATARGET_TEAM        11
#define ATARGET_FRIENDLY    12

int ATarget_ArcData[13];

function void Arc_GetTarget(int arcType, int arcerTID)
{
    // Precalculate as much stuff as needed to save cycles
    
    ATarget_ArcData[ATARGET_TYPE] = arcType;
    ATarget_ArcData[ATARGET_TID]  = arcerTID;
    ATarget_ArcData[ATARGET_POSX] = GetActorX(0);
    ATarget_ArcData[ATARGET_POSY] = GetActorY(0);
    ATarget_ArcData[ATARGET_POSZ] = GetActorZ(0);
    
    int arcAng   = GetActorAngle(0);
    int arcPitch = GetActorPitch(0);
    
    ATarget_ArcData[ATARGET_ANGLE] = arcAng;
    ATarget_ArcData[ATARGET_PITCH] = arcPitch;
    ATarget_ArcData[ATARGET_AIMX]  = FixedMul(cos(arcAng), cos(arcPitch));
    ATarget_ArcData[ATARGET_AIMY]  = FixedMul(sin(arcAng), cos(arcPitch));
    ATarget_ArcData[ATARGET_AIMZ]  = sin(arcPitch);
    
    
    SetActivator(0, AAPTR_TARGET);
    int pln  = PlayerNumber();
    int team = NoTeamIndex;
    if (pln >= 0) { team = GetPlayerInfo(pln, PLAYERINFO_TEAM); }
    
    ATarget_ArcData[ATARGET_PLAYERNUM] = pln;
    ATarget_ArcData[ATARGET_TEAM]      = team;
    ATarget_ArcData[ATARGET_FRIENDLY]  = GetActorProperty(0, APROP_Friendly);
    
    SetActivator(arcerTID);
    
    // Add 128 to the range as a fudge faster, since A_RadiusGive doesn't check
    //  the full hitbox, just origins
    SetUserVariable(0, "user_lookrange", INT_ArcData[arcType][ARC_I_RANGE] + 128);
    SetActorState(0, "Arc_Look");
}


script "Arc_CheckTarget" (void)
{
    int arcType  = ATarget_ArcData[ATARGET_TYPE];
    int arcerTID = ATarget_ArcData[ATARGET_TID];
    
    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);
    
    // First check: did we target the firer?
    int testnum = random(1, 0x7FFFFFFF);
    
    SetInventory("SameThingChecker", testnum);
    SetActivator(arcerTID, AAPTR_TARGET);
    
    int sameDude = (testnum == CheckInventory("SameThingChecker"));
    
    SetActivator(myTID_new);
    TakeInventory("SameThingChecker", 0x7FFFFFFF);
    
    if (sameDude)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }
    
    
    // Second check: have we hit this target already?
    testnum = random(1, 0x7FFFFFFF);
    SetInventory("SameThingChecker", testnum);
    SetActivator(arcerTID);
    
    int hitAlready    = false;
    int masterTID_new = UniqueTID();
    
    while (!hitAlready)
    {
        SetActivator(0, AAPTR_MASTER);
        if (ClassifyActor(0) & ACTOR_WORLD) { break; }
        
        int masterTID_old = ActivatorTID();
        Thing_ChangeTID(0, masterTID_new);
        SetActivator(0, AAPTR_TRACER);
        
        if (!(ClassifyActor(0) & ACTOR_WORLD))
        {
            hitAlready = (testnum == CheckInventory("SameThingChecker"));
        }
        
        SetActivator(masterTID_new);
        Thing_ChangeTID(masterTID_new, masterTID_old);
    }
    
    SetActivator(myTID_new);
    TakeInventory("SameThingChecker", 0x7FFFFFFF);
    
    if (hitAlready)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }
    
    
    // Third check: are they allied to each other?
    int myPln    = PlayerNumber();
    int firerPln = ATarget_ArcData[ATARGET_PLAYERNUM];
    int allied   = false;
    
    if (myPln >= 0 && firerPln >= 0)
    {
        if (GameType() == GAME_NET_DEATHMATCH)
        {
            int myTeam    = GetPlayerInfo(myPln, PLAYERINFO_TEAM);
            int firerTeam = ATarget_ArcData[ATARGET_TEAM];
            
            allied = ((myTeam == firerTeam) && (myTeam != NoTeamIndex) && (firerTeam != NoTeamIndex));
        }
        else
        {
            // If your friends can be hurt, they *will* be hurt
            allied = (GetCVar("teamdamage") <= 0);
        }
    }
    else
    {
        allied = (GetActorProperty(0, APROP_Friendly) == ATarget_ArcData[ATARGET_FRIENDLY]);
    }
    
    if (allied)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }
    
    
    // Now we prune it down based on distance and angle difference.
    int bestScore;
    int maxDist = itof(INT_ArcData[arcType][ARC_I_RANGE]);
    
    SetActivator(arcerTID, AAPTR_TRACER);
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        bestScore = maxDist;
    }
    else
    {
        bestScore = GetUserVariable(arcerTID, "user_bestweight");
    }
    
    SetActivator(myTID_new);
    
    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);
    int myRadius = GetActorProperty(0, APROP_Radius);
    int myHeight = GetActorProperty(0, APROP_Height);
    
    int arcerX = ATarget_ArcData[ATARGET_POSX];
    int arcerY = ATarget_ArcData[ATARGET_POSY];
    int arcerZ = ATarget_ArcData[ATARGET_POSZ];

    int myX_closest = middle(arcerX, safeAdd(myX, myRadius), safeAdd(myX, -myRadius));
    int myY_closest = middle(arcerY, safeAdd(myY, myRadius), safeAdd(myY, -myRadius));
    int myZ_closest = middle(arcerZ, safeAdd(myZ, myHeight), myZ);
    
    int dx   = myX_closest - arcerX;
    int dy   = myY_closest - arcerY;
    int dz   = myZ_closest - arcerZ;
    
    // A_RadiusGive works with a sphere by default, so just check distance
    int dist = VectorLength(VectorLength(dx, dy), dz);
    if (dist >= bestScore)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }
    
    
    int endScore    = dist;
    int angleWeight = INT_ArcData[arcType][ARC_I_ANGLEWEIGHT];
    
    if (angleWeight > 0)
    {
        int aimX = ATarget_ArcData[ATARGET_AIMX];
        int aimY = ATarget_ArcData[ATARGET_AIMY];
        int aimZ = ATarget_ArcData[ATARGET_AIMZ];
        
        // angle difference: acos(dot(vec1, vec2) / (mag(vec1) * mag(vec2)))
        // and the magnitude of aim vector is always 1
        //
        // this will never be above 0.5
        
        int angleDiff  = acos(FixedDiv(dot3(aimX, aimY, aimZ, dX, dY, dZ), dist));
        
        // since lower is better, we want to raise the score for worse values
        //
        // division has the nice property that the closer you get to zero, the
        //  higher the rate of change gets. therefore, the greater the angle
        //  difference, the less likely that angle will actually be used.
        // 
        // this also used to help optimize the A_LookEx calls (since looking
        //  beyond the current best score was pointless), but now that the code
        //  uses A_RadiusGive, that doesn't matter anymore.
        
        int angleScore = FixedDiv(dist, 1.0 - angleDiff);
        endScore = dist + FixedMul(angleWeight, angleScore - dist);
    }
    
    if (endScore >= bestScore)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }
    
    SetActivator(arcerTID);
    SetUserVariable(0, "user_bestweight", endScore);
    SetPointer(AAPTR_TRACER, myTID_new);
    Thing_ChangeTID(myTID_new, myTID_old);
}


/*
    // At this point, we're pretty damn sure we have a valid target.
    //  Now only distance checks will rule them out.

    // Any score above this can be ruled out.
    int rangeLimiter = min(maxRange, currentBestScore);

    int targetX = GetActorX(targetTID);
    int targetY = GetActorY(targetTID);
    int targetZ = GetActorZ(targetTID) + GetActorViewHeight(targetTID);

    int dX = targetX - APos[APOS_X];
    int dY = targetY - APos[APOS_Y];
    int dZ = targetZ - APos[APOS_Z];

    // dX and dY were already implicitly checked by A_LookEx, but not dZ,
    //  so let's handle that to quickly rule out targets too high or low up.
    if (abs(dZ) >= rangeLimiter) { return -1; }


    // Well, that's all the fast checks out of the way. Time for slow stuff.

    int distanceScore = magnitudeThree_f(dX, dY, dZ);

    // If the distance is higher than the current best score, there's no way
    //  it can beat it when adjusted, so disqualify it. Also, range limits.
    if (distanceScore >= rangeLimiter) { return -1; }


    // Time to factor in angle difference to the target's score.
    //
    // First off, we're just gonna grab this now. If it's 0,
    //  it saves us a lot of time, because angle won't matter.

    int angleWeight = INT_ArcData[arctype][ADATA_INT_ANGLEWEIGHT];
    if (angleWeight <= 0) { return distanceScore; }


    // Okay, now we need to weigh the angle.
    // 
    // Calculate difference between 3D angles.
    //
    //      acos(dot3(vec1, vec2) / (mag(vec1) * mag(vec2)))
    //
    // Since the arc angle vector's (vec1 here) magnitude always 1,
    //  this just becomes:
    //
    //      acos(dot3(vec1, vec2) / mag(vec2))
    //
    //  angleDiff will never be above 0.5.

    int angleDiff = acos(FixedDiv(dot3(APos[APOS_ANGX], APos[APOS_ANGY], APos[APOS_ANGZ], dX, dY, dZ), distanceScore));


    // Now calculate the score. We can only go up from distanceScore, or else
    //  some core assumptions for (very necessary) optimization are broken.
    //
    // Basically, this: if the best score is 128, anything at least 128 units
    //  away cannot, under any circumstances, beat that score. This means we
    //  can stop looking past 128 units because it's a complete waste of time,
    //  which allows us to look in layers and stop exactly when it becomes
    //  pointless.
    //
    // As for why we do FixedDiv, it's because of how as 1/x approaches x=0,
    //  the rate of change gets faster and faster. For us, that means angles
    //  get weighted much more harshly the further away they stray from the
    //  zero-line.

    int angleScore = FixedDiv(distanceScore, 1.0 - angleDiff);

    // Angle weight scale goes from 0 to 1.
    if (angleWeight >= 1.0)
    {
        return angleScore;
    }
    
    int weightedScore = distanceScore + FixedMul(angleWeight, angleScore - distanceScore);
    return weightedScore;
*/