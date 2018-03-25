script "Arc_Main" (int arcType)
{
    // sanity check
    if (arcType < 0 || arcType >= ARCTYPES) { terminate; }

    // step 1 already finished
    int arcerTID_old = ActivatorTID();
    int arcerTID     = UniqueTID();
    Thing_ChangeTID(0, arcerTID);


    // steps 2 to 4
    SetActorState(0, "Arc_Query");

    if (!GetUserVariable(0, "user_keeparcing"))
    {
        Thing_ChangeTID(0, arcerTID_old);
        ACS_NamedExecuteWithResult("Arc_InheritTracer");
        SetActorState(0, "Arc_EndArc");
        terminate;
    }

    // steps 5 to 9
    Arc_GetTarget(arcType, arcerTID);


    // step 10
    SetActivator(0, AAPTR_TRACER);
    if (IsWorld())
    {
        SetActivator(arcerTID);
        Thing_ChangeTID(0, arcerTID_old);
        ACS_NamedExecuteWithResult("Arc_InheritTracer");
        SetActorState(0, "Arc_NoTarget");
        terminate;
    }

    // get this stuff now, because they might die and their height might change
    int tracerHeight = GetActorProperty(0, APROP_Height);
    int tracerRadius = GetActorProperty(0, APROP_Radius);


    // step 11
    SetActivator(arcerTID);

    // set TID back in case the arcing actor needs its old TID for some reason
    Thing_ChangeTID(0, arcerTID_old);
    SetActorState(0, "Arc_FoundTarget");

    // step 12
    if (!GetUserVariable(0, "user_keeparcing"))
    {
        terminate;
    }

    // refresh TIDs as they might have changed
    arcerTID_old = ActivatorTID();
    arcerTID     = UniqueTID();
    Thing_ChangeTID(0, arcerTID);


    // step 13
    SetActivator(0, AAPTR_TRACER);

    int tracerX  = GetActorX(0);
    int tracerY  = GetActorY(0);
    int tracerZ  = GetActorZ(0);

    int arcX     = tracerX;
    int arcY     = tracerY;
    int arcZ     = tracerZ + (tracerHeight / 2);

    int randomOffset = abs(INT_ArcData[arcType][ARC_I_RANDOMOFFSET]);

    arcX += FixedMul(tracerRadius, random(-randomOffset, randomOffset));
    arcY += FixedMul(tracerRadius, random(-randomOffset, randomOffset));
    arcZ += FixedMul(tracerHeight, random(-randomOffset / 2, randomOffset / 2));

    int dx = arcX - GetActorX(arcerTID);
    int dy = arcY - GetActorY(arcerTID);
    int dz = arcZ - GetActorZ(arcerTID);

    // angle/pitch from arcer to tracer
    int tracerAngle = VectorAngle(dx, dy);
    int tracerPitch = -VectorAngle(VectorLength(dx, dy), dz);

    SetActivator(arcerTID, AAPTR_TARGET);

    int firerAngle = GetActorAngle(0);
    int firerPitch = GetActorPitch(0);

    int headingScalar = INT_ArcData[arcType][ARC_I_KEEPHEADING];
    int arcAngle = tracerAngle + FixedMul(headingScalar, angleDifference(tracerAngle, firerAngle));
    int arcPitch = tracerPitch + FixedMul(headingScalar, angleDifference(tracerPitch, firerPitch));

    str arcActor = STR_ArcData[arcType][ARC_S_NEXT];
    int arcTID   = UniqueTID();

    SpawnForced(arcActor, arcX, arcY, arcZ, arcTID);
    SetActorAngle(arcTID, arcAngle);
    SetActorPitch(arcTID, arcPitch);


    // step 14
    int targetTID_old = ActivatorTID();
    int targetTID_new = UniqueTID();
    Thing_ChangeTID(0, targetTID_new);

    SetActivator(arcTID);
    SetPointer(AAPTR_TARGET, targetTID_new);
    SetPointer(AAPTR_MASTER, arcerTID);
    SetUserVariable(0, "user_depth", GetUserVariable(arcerTID, "user_depth") + 1);
    Thing_ChangeTID(targetTID_new, targetTID_old);
    Thing_ChangeTID(arcerTID,      arcerTID_old);


    // step 15
    SetActorState(0, "Arc_Spawn");
}





script "Arc_InheritTracer" (void)
{
    int arcerTID_old = ActivatorTID();
    int arcerTID_new = UniqueTID();
    Thing_ChangeTID(0, arcerTID_new);

    // step to previous arcer
    SetActivator(0, AAPTR_MASTER);
    if (IsWorld())
    {
        Thing_ChangeTID(arcerTID_new,  arcerTID_old);
        terminate;
    }

    // step to its tracer
    SetActivator(0, AAPTR_TRACER);
    if (IsWorld())
    {
        Thing_ChangeTID(arcerTID_new,  arcerTID_old);
        terminate;
    }

    // temporary unique TID
    int tracerTID_old = ActivatorTID();
    int tracerTID_new = UniqueTID();
    Thing_ChangeTID(0, tracerTID_new);

    // set up tracer in current arcer
    SetActivator(arcerTID_new);
    SetPointer(AAPTR_TRACER, tracerTID_new);

    // un-clobber TIDs
    Thing_ChangeTID(tracerTID_new, tracerTID_old);
    Thing_ChangeTID(arcerTID_new,  arcerTID_old);
}