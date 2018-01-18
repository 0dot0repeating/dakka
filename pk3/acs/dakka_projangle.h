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