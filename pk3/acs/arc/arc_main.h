// ========================= REALLY IMPORTANT GOD DAMN =========================
//
// (this is in decorate/core.dec as well because it's THAT IMPORTANT)
//
// You REALLY REALLY should understand the process that arcing takes, so here it is,
//  in very brief detail.
// 
// Pointers are in order {target, master, tracer}. "????" means "unset so far".
//
// "(D)" means we're running in DECORATE. "(A)" means we're running in ACS.
//
// Step 1  (D/A): call arcing script
//  Pointers: {firer, ????, ????}
//
// Step 2  (A): ACS sends arcer to Arc_Query
//  Pointers: {firer, ????, ????}
//
// Step 3  (D): Arc_Query's run, gives ArcItem_DoArc to itself if it wants to arc
//  Pointers: {firer, ????, ????}
//
// Step 4  (A): If no ArcItem_DoArc, clear target data, send arcer to Arc_EndArc, and end
//  Pointers: {firer, ????, ????}
//
// - IF NOT ENDED -
//
//      --- LOOP ---
//
//      Step 5 (A): Send arcer to Arc_Look
//       Pointers: {firer, ????, ????}
//
//      Step 6 (D): Look around
//       Pointers: {firer, ????, potential target}
//
//      Step 7 (A): Nothing in tracer pointer? Break loop
//       Pointers: {firer, ????, potential target}
//
//      Step 8 (A): Something in tracer pointer? Process it, add T_CHECKED target slot to it
//       Pointers: {firer, ????, potential target}
//
//      Step 9 (A): New target processed, decide if it's better than current best, if so keep it
//       Pointers: {firer, ????, potential target}
//
//      --- END LOOP ---
//
// Step 10 (A): Clear all T_CHECKED target slots
//  Pointers: {firer, ????, now-irrelevant target}
//
// Step 11 (A): No target? Clear target data, Stick last target (if any) in tracer field,
//              send arcer to Arc_NoTarget, end
//  Pointers: {firer, ????, last target (if any)}
//
// - IF NOT ENDED -
//
// Step 12 (A): Stick best target in tracer field
//  Pointers: {firer, ????, best target}
//
// Step 13 (D): Do Arc_FoundTarget (now's the time to do damage)
//  Pointers: {firer, ????, best target}
//  
// Step 14 (A): Stick T_HIT target slot on calculated best target (it is now +FRIENDLY)
//  Pointers: {firer, ????, best target}
//
// Step 15 (A): Spawn next arcer object on best target, focus on that one now
//  Pointers: {empty, empty, empty}
//
// Step 16 (A): Set up pointers on new arcer object
//  Pointers: {firer, current arcer, current target}
//
// Step 17 (A): Send new arcer object to Arc_Spawn

script "Arc_Main" (int arcType)
{
    int manualClear = INT_ArcData[arcType][ADATA_INT_MANUALCLEAR];

    // Step 1 already done
    int arcerTID   = defaultTID(-1);
    SetActivatorToTarget(arcerTID);

    int firerTID = defaultTID(-1);

    SetActivator(arcerTID, AAPTR_TRACER);
    int oldTargetTID;
    
    if (ClassifyActor(0) & ACTOR_WORLD) { oldTargetTID = 0; }
    else                                { oldTargetTID = defaultTID(-1); }

    SetActivator(arcerTID);

    // Steps 2 and 3
    TakeInventory("ArcItem_DoArc", 0x7FFFFFFF);
    SetActorState(0, "Arc_Query");
    int doArc = CheckInventory("ArcItem_DoArc");

    // Step 4
    if (!doArc)
    {
        if (!manualClear) { T_Clear(); }
        SetActorState(0, "Arc_EndArc");
        terminate;
    }

    // Steps 5 through 10
    int targetTID = Look_Loop(arcType, arcerTID);

    // Step 11
    if (targetTID == 0)
    {
        if (!manualClear) { T_Clear(); }

        SetPointer(AAPTR_TRACER, oldTargetTID);
        SetActorState(0, "Arc_NoTarget");
        terminate;
    }

    // Step 12
    SetPointer(AAPTR_TRACER, targetTID);

    // We actually need to get these *before* FoundTarget runs, because
    //  if the target dies, his camera height goes to 7.
    int spawnX = GetActorX(targetTID);
    int spawnY = GetActorY(targetTID);
    int spawnZ = GetActorZ(targetTID) + GetActorViewHeight(targetTID);

    // Being in the exact center of the target gets boring. Let's vary it up.

    int randomOffset = INT_ArcData[arctype][ADATA_INT_RANDOMOFFSET];

    spawnX += random(-randomOffset, randomOffset);
    spawnY += random(-randomOffset, randomOffset);
    spawnZ += random(-randomOffset, randomOffset);

    // Step 13, with an addition:
    //  - We also set the arcer's angle now that we have a target. It's in
    //    between the firer's angle and the angle pointing towards the new
    //    target.

    Arc_CalcNewAngle(arcType, arcerTID, targetTID);
    SetActorAngle(arcerTID, Arc_NewAngles[ANG_TARGET][ANG_ANGLE]);
    SetActorPitch(arcerTID, Arc_NewAngles[ANG_TARGET][ANG_PITCH]);

    SetActorState(0, "Arc_FoundTarget");

    // Step 14

    // Looking at the code alone, you don't know how much anguish went into the
    //  four lines below. And it's just four lines. How bad could it be?
    //
    // It was either these four lines or five billion ugly hacks to get around
    //  a useless fucking ID system that does absolutely nothing an ID system
    //  is meant to actually do.
    //
    // So, Thing IDs. Let's run down the list of what IDs tend to do:
    //
    // 1. They identify something.
    //
    // 2. They *uniquely* identify something.
    //
    // 3. They uniquely identify something *immutably*.
    //
    // Lets look at Thing IDs.
    //
    // 1. They may identify something. They may identify a lot of things. Who
    //    knows? You sure as hell don't. ThingCount won't actually tell you.
    //    Dead things don't count. Sometimes.
    //
    // 2. And yeah, they can identify multiple things. Which would be fine,
    //    if it were a group ID. But half the functions are clearly intended
    //    to work with singular entities, and what's the only thing they accept?
    //    Thing IDs.
    //
    // 3. What they identify can change. In fact, it's safer to say what they
    //    identify *will* change. Why? Because anything can change the Thing ID
    //    of something at any time. Anything can be given a given Thing ID at
    //    any time. There's absolutely no guarantees for anything.
    //
    // "But there's gotta be a better system!"
    //
    // Nope. They all got shot down. Some of them were even *coded* for the
    //  ZDoom developers. Fuck that. Graf shot them down anyway.
    //
    // So you're stuck with this ungodly abomination of an ID system that 
    //  breaks if you so much as breath on it.
    //
    // These four lines below take away the TID from the arcer target if it
    //  died. Without them... well, take them out, go fire some arcs into a
    //  crowd for a while, and you'll find out.
    //
    // The moment a target gets their TID assigned to the same TID a dead
    //  monster has... it's over.
    //
    // TIDs are a joke played on every ZDoom modder that's ever had the
    //  misfortune of using them.

    if (GetActorProperty(targetTID, APROP_Health) <= 0)
    {
        Thing_ChangeTID(targetTID, 0);
        targetTID = 0;
    }
    else
    {
        THit_Add(targetTID);
    }

    // Step 15
    int newTID   = unusedTID(6000, 9000);
    int newActor = STR_ArcData[arcType][ADATA_STR_NEXTARCER];

    // See Arc_CalcNewAngles for details as to how the new arcer's angle
    //  is determined.

    SpawnForced(newActor, spawnX, spawnY, spawnZ, newTID);
    SetActorAngle(newTID, Arc_NewAngles[ANG_NEW][ANG_ANGLE]);
    SetActorPitch(newTID, Arc_NewAngles[ANG_NEW][ANG_PITCH]);

    // Step 16
    SetActivator(newTID);

    SetPointer(AAPTR_TARGET, firerTID);
    SetPointer(AAPTR_MASTER, arcerTID);
    SetPointer(AAPTR_TRACER, targetTID);

    // Step 17
    SetActorState(0, "Arc_Spawn");
    
    // This will run after everything in the new arcer is done,
    //  so it's safe to add this here.
    if (!manualClear) { T_Clear(); }
}



// This is for when the arc object doesn't automatically do Target_Clear().

script "Arc_Clear" (void)
{
    T_Clear();
}
