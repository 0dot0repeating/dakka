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

    // Add 128 to the range as a fudge factor, since A_RadiusGive doesn't check
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
        if (IsWorld()) { break; }

        int masterTID_old = ActivatorTID();
        Thing_ChangeTID(0, masterTID_new);
        SetActivator(0, AAPTR_TRACER);

        if (!IsWorld())
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

    if (myPln >= 0 && firerPln >= 0)
    {
        int allied = false;

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

        if (allied)
        {
            Thing_ChangeTID(myTID_new, myTID_old);
            terminate;
        }
    }


    // Now we prune it down based on distance and angle difference.
    int bestScore;
    int maxDist = itof(INT_ArcData[arcType][ARC_I_RANGE]);

    SetActivator(arcerTID, AAPTR_TRACER);
    if (IsWorld())
    {
        bestScore = 0x7FFFFFFF;
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

    int dist = VectorLength(VectorLength(dx, dy), dz);

    // shouldn't need this, but whatever
    if (dist >= maxDist)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }

    int aimX = ATarget_ArcData[ATARGET_AIMX];
    int aimY = ATarget_ArcData[ATARGET_AIMY];
    int aimZ = ATarget_ArcData[ATARGET_AIMZ];

    // angle difference: acos(dot(vec1, vec2) / (mag(vec1) * mag(vec2)))
    // and the magnitude of aim vector is always 1
    //
    // this will never be above 0.5

    int angleDiff  = acos(FixedDiv(dot3(aimX, aimY, aimZ, dX, dY, dZ), dist));

    int endScore    = dist;
    int angleWeight = INT_ArcData[arcType][ARC_I_ANGLEWEIGHT];
    int angleLimit  = INT_ArcData[arcType][ARC_I_ANGLELIMIT];

    if (angleLimit > 0 && angleDiff > angleLimit)
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }

    if (angleWeight > 0)
    {
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