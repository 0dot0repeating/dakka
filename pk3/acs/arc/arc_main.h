script "Arc_Main" (int arcType)
{
    // sanity check
    if (arcType < 0 || arcType >= ARCTYPES) { terminate; }
    
    // step 1 already finished
    int arcerTID = defaultTID(0);
    
    // sanity check
    SetActivator(0, AAPTR_TARGET);
    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }
    SetActivator(arcerTID);
    
    // steps 2 to 4
    SetActorState(0, "Arc_Query", false);
    
    if (!GetUserVariable(0, "user_keeparcing"))
    {
        ACS_NamedExecuteWithResult("Arc_InheritTracer");
        SetActorState(0, "Arc_EndArc", false);
        terminate;
    }
    
    // steps 5 to 9
    SetActorState(0, "Arc_Look", false);
    
    // step 10
    SetActivator(0, AAPTR_TRACER);
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        SetActivator(arcerTID);
        ACS_NamedExecuteWithResult("Arc_InheritTracer");
        SetActorState(0, "Arc_NoTarget", false);
        terminate;
    }
    
    // step 11
    SetActorState(arcerTID, "Arc_FoundTarget", false);
    
    // step 12
    int tracerX      = GetActorX(0);
    int tracerY      = GetActorY(0);
    int tracerZ      = GetActorZ(0);
    int tracerHeight = GetActorProperty(0, APROP_Height);
    int tracerRadius = GetActorProperty(0, APROP_Height);
    
    SetActivator(arcerTID, AAPTR_TARGET);
    int arcX     = tracerX;
    int arcY     = tracerY;
    int arcZ     = tracerZ + (tracerHeight / 2);
    int arcAngle = GetActorAngle(0);
    int arcPitch = GetActorPitch(0);
    
    str arcActor = STR_ArcData[arcType][ARC_S_NEXT];
    int arcTID   = UniqueTID();
    
    SpawnForced(arcActor, arcX, arcY, arcZ, arcTID);
    SetActorAngle(arcTID, arcAngle);
    SetActorPitch(arcTID, arcPitch);
    
    // step 13
    int targetTID_old = ActivatorTID();
    int targetTID_new = UniqueTID();
    Thing_ChangeTID(0, targetTID_new);
    
    SetActivator(arcTID);
    SetPointer(AAPTR_TARGET, targetTID_new);
    SetPointer(AAPTR_MASTER, arcerTID);
    Thing_ChangeTID(targetTID_new, targetTID_old);
    
    // step 14
    SetActorState(0, "Arc_Spawn", false);
}





script "Arc_InheritTracer" (void)
{
    int arcerTID = defaultTID(0);
    
    // step to previous arcer
    SetActivator(0, AAPTR_MASTER);
    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }
    
    // step to its tracer
    SetActivator(0, AAPTR_TRACER);
    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }
    
    // temporary unique TID
    int tracerTID_old = ActivatorTID();
    int tracerTID_new = UniqueTID();
    Thing_ChangeTID(0, tracerTID_new);
    
    // set up tracer in current arcer
    SetActivator(arcerTID);
    SetPointer(AAPTR_TRACER, tracerTID_new);
    
    // un-clobber TID
    Thing_ChangeTID(tracerTID_new, tracerTID_old);
}