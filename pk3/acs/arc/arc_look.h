// Simply looking for shit needed an entire file dedicated to it.

// Copying here for self-reminder:
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

int APos_X, APos_Y, APos_Z;
int ALook_AngX, ALook_AngY, ALook_AngZ;

function int Look_Loop(int arcType, int arcerTID)
{
    int bestTargetTID = 0;
    int bestTargetScore = 0x7FFFFFFF;

    int rangeStep = 0;
    int rangeMax  = INT_ArcData[arcType][ADATA_INT_MAXRANGE];

    // Precalculate to save cycles. It's used in Loop_CalcScore.
    // 
    // It's the vector of the angle the arc is facing.
    
    APos_X = GetActorX(0);
    APos_Y = GetActorY(0);
    APos_Z = GetActorZ(0);
    int arcAngle = GetActorAngle(0);
    int arcPitch = GetActorPitch(0);

    ALook_AngX = FixedMul(cos(arcAngle), cos(arcPitch));
    ALook_AngY = FixedMul(sin(arcAngle), cos(arcPitch));
    ALook_AngZ =          sin(arcPitch);

    while (true)
    {
        rangeStep += 32.0;
        SetUserVariable(0, "user_lookDist", ftoi(min(rangeStep, rangeMax)));

        while (true)
        {
            // Steps 5 and 6
            SetActorState(0, "Arc_Look");

            // Step 7
            if (CheckInventory("ArcItem_NoTarget")) { break; }

            // Step 8
            SetActivator(0, AAPTR_TRACER);

            int targetTID   = defaultTID(-1);
            int targetScore = Loop_CalcScore(arcType, arcerTID, targetTID, rangeMax, bestTargetScore);

            TCheck_Add(targetTID);

            // Step 9
            if ((targetScore != -1) && (targetScore < bestTargetScore))
            {
                bestTargetTID   = targetTID;
                bestTargetScore = targetScore;

                rangeMax  = targetScore;
                rangeStep = min(rangeMax, rangeStep);

                // Saves a few cycles to just break it now.
                //  And goddamn, every instruction saved is important.
                if (rangeMax == rangeStep) { break; }

                SetUserVariable(0, "user_lookDist", ftoi(rangeStep));
            }
        }

        if (rangeStep >= rangeMax)       { break; }
        if (bestTargetScore < rangeStep) { break; }
    }

    // Step 10
    TCheck_Clear();

    SetActivator(arcerTID);
    return bestTargetTID;
}




// Given the TIDs of both the arcer and its potential target, as well as
//  the arc type, calculate its targeting score. Lower is better. If the
//  target's invalid, return -1 instead.
//
// We pass in currentBestScore for some score-calculating optimizations.
//
// Thing is with the score calculation is that it starts at the actor's distance
//  from the arcer, and only goes up from there. So if the distance is higher than
//  the current best score, there is no way it will beat the current best, so we
//  can just return it as-is. Every calculation afterwards is meaningless.

function int Loop_CalcScore(int arcType, int arcerTID, int targetTID, int maxRange, int currentBestScore)
{
    // First off, we check off the quick-to-calculate stuff.
    
    // Gather firer and target info.
    SetActivator(0, AAPTR_TARGET);
    int firerPln  = PlayerNumber();
    int firerTID  = ActivatorTID();

    SetActivator(targetTID);
    int targetPln = PlayerNumber();

    SetActivator(arcerTID);

    // Have we somehow targeted ourself? 
    if (firerTID == targetTID)
    {
        Log(s:"Somehow we targeted ourself: ", d:firerTID, s:" vs. ", d:targetTID);
        return -1;
    }


    // If the target and firer are players, we check player numbers and teams,
    //  because friendliness checks aren't a reliable way of doing it.
    if ((targetPln != -1) && (firerPln != -1))
    {
        int noTeamIndex;

        // ZDoom has the "no team" index at 255. Zandronum has it at 4. DUMB.
        if (IsZand) { noTeamIndex = 4; }
        else        { noTeamIndex = 255; }

        // A -FRIENDLY arc will target the player who fired it. Rule that out.
        if (firerPln == targetPln) { return -1; }

        int firerTeam  = GetPlayerInfo(firerPln,  PLAYERINFO_TEAM);
        int targetTeam = GetPlayerInfo(targetPln, PLAYERINFO_TEAM);

        // And let's make sure we're not team-killing.
        if ((firerTeam != noTeamIndex) && (firerTeam == targetTeam)) { return -1; }
    }
    else
    {
        // Friendliness check - same friendliness flags? Not valid.
        if (GetActorProperty(firerTID,  APROP_Friendly) == GetActorProperty(targetTID, APROP_Friendly))
        {
            return -1;
        }
    }

    // At this point, we're pretty damn sure we have a valid target.
    //  Now only distance checks will rule them out.

    // Any score above this can be ruled out.
    int rangeLimiter = min(maxRange, currentBestScore);

    int targetX = GetActorX(targetTID);
    int targetY = GetActorY(targetTID);
    int targetZ = GetActorZ(targetTID) + GetActorViewHeight(targetTID);

    int dX = targetX - APos_X;
    int dY = targetY - APos_Y;
    int dZ = targetZ - APos_Z;

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

    int angleDiff = acos(FixedDiv(dot3(ALook_AngX, ALook_AngY, ALook_AngZ, dX, dY, dZ), distanceScore));


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
}
