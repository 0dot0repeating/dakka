script "Dakka_ProjUpdate" (void)
{
    SetUserVariable(0, "user_t",    GetUserVariable(0, "user_t") + 1);
    SetUserVariable(0, "user_velx", GetActorVelX(0));
    SetUserVariable(0, "user_vely", GetActorVelY(0));
    SetUserVariable(0, "user_velz", GetActorVelZ(0));
}

script "Dakka_ProjDeathUpdate" (void)
{
    if (!GetUserVariable(0, "user_t"))
    {
        int myTID_old = ActivatorTID();
        int myTID     = UniqueTID();
        Thing_ChangeTID(0, myTID);
        
        SetActivator(0, AAPTR_TARGET);
        if (ClassifyActor(0) & ACTOR_WORLD)
        {
            Thing_ChangeTID(myTID, myTID_old);
            terminate;
        }
        
        int firerAngle = GetActorAngle(0);
        int firerPitch = GetActorPitch(0);
        int projSpeed  = GetActorProperty(myTID, APROP_Speed);
        
        SetUserVariable(myTID, "user_velx",    FixedMul(projSpeed, FixedMul(cos(firerAngle), cos(firerPitch))));
        SetUserVariable(myTID, "user_vely",    FixedMul(projSpeed, FixedMul(sin(firerAngle), cos(firerPitch))));
        SetUserVariable(myTID, "user_velz",    FixedMul(projSpeed, -sin(firerPitch)));
        
        SetActorPitch(myTID, firerPitch);
        Thing_ChangeTID(myTID, myTID_old);
    }
    else
    {
        int projVX = GetUserVariable(0, "user_velx");
        int projVY = GetUserVariable(0, "user_vely");
        int projVZ = GetUserVariable(0, "user_velz");
        SetActorPitch(0, -VectorAngle(VectorLength(projVX, projVY), projVZ));
    }
}


script "ProjCheck_IsAllied" (int myPln, int myTeam, int firerPln, int firerTeam)
{
    int allied = false;
    
    if (myPln >= 0 && firerPln >= 0)
    {
        if (GameType() == GAME_NET_DEATHMATCH)
        {
            // we don't need to check the no-team index twice
            //  if they have the same team, then a == b == c
            //  if they don't have the same team, it doesn't matter
            allied = (myTeam == firerTeam)
                  && (myTeam != cond(IsZandronum, 4, 255));
        }
        else
        {
            // If your friends can be hurt, they *will* be hurt
            allied = (GetCVar("teamdamage") <= 0);
        }
    }
    
    SetResultValue(allied);
}


script "ProjCheck_ShortestDist" (int projX, int projY, int projZ)
{
    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);
    int myRadius = GetActorProperty(0, APROP_Radius);
    int myHeight = GetActorProperty(0, APROP_Height);

    int myX_closest = middle(projX, safeAdd(myX, myRadius), safeAdd(myX, -myRadius));
    int myY_closest = middle(projY, safeAdd(myY, myRadius), safeAdd(myY, -myRadius));
    int myZ_closest = middle(projZ, safeAdd(myZ, myHeight), myZ);
    
    int dx   = myX_closest - projX;
    int dy   = myY_closest - projY;
    int dz   = myZ_closest - projZ;
    
    SetResultValue(VectorLength(VectorLength(dx, dy), dz));
}