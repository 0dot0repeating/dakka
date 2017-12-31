script "Arc_Main" (int arcType)
{
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