#library "arc"
#include "zcommon/zcommon.h"
#include "dakka_commonFuncs.h"

/*
Arc code, take three. This time, I try to minimize TID use as much as I can,
 because the previous arc code irreversibly trashed the TIDs of everything
 it looked at and I can't see any way to fix it without ripping out the logic
 entirely.

Instead of using A_Look and keeping track of what's been seen through TIDs,
 I will use A_RadiusGive and run the target-weighing scripts on them, using
 SetPointer and user variables to keep track of the best target so far.

Instead of keeping track of previously hit targets in a TID array, I will
 iterate through the previous arcers and check their AAPTR_TRACER fields to
 see if we have a match.

Hopefully this works out better. We can only hope.
*/


/* GENERAL ACTOR POINTER SETUP UNLESS OTHERWISE NOTED:
    Target: Firer
    Master: Previous arcer, if any
    Tracer: Arcer's target, if any

Step 1 (D): call arcing script
Step 2 (A): send arcer to Arc_Query state
Step 3 (D): arcer sets "user_keeparcing" to true if it wants to arc
Step 4 (A): if "user_keeparcing" is false, send arcer to Arc_EndArc state, set
            its tracer to the previous arcer's tracer, and end

  --- user_keeparcing is true ---

Step 5 (A): send arcer to Arc_Look
Step 6 (D): arcer gives every possible target around it the ArcLookout item
Step 7 (D): the ArcLookout item runs the Arc_WeighTarget script on the target
Step 8 (A): Arc_WeighTarget checks to see if it's the current best target
Step 9 (A): if it is the best target, store itself in the arcer's tracer pointer
            and update "user_bestweight" (lower is better)

Step 10 (A): if the arcer doesn't have a tracer, set its tracer to the tracer of
             the *previous* arcer, send the arcer to Arc_NoTarget, and end

  --- arcer has a tracer ---

Step 11 (A): send the arcer to Arc_FoundTarget, which is its final state jump
Step 12 (A): if "user_keeparcing" was set to false, end the arc here

  --- user_keeparcing is true ---

Step 13 (A): spawn next arcer on tracer
Step 14 (A): set target and master pointers up on next arcer
Step 15 (A): send next arcer to Arc_Spawn
*/

// ZDoom's no-team index (255) is different than Zandronum's no-team index (4).
int NoTeamIndex;

#include "arc/arc_const.h"
#include "arc/arc_target.h"
#include "arc/arc_main.h"


script "Arc_Open" open
{
    SetDBEntry("dakka_arc", "client_iszand", 789237);
    NoTeamIndex = cond(GetDBEntry("dakka_arc", "client_iszand") == 789237, 4, 255);
}

script "Arc_Damage" (int damage)
{
    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);

    SetActivator(0, AAPTR_TRACER);
    if (IsWorld())
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }

    int hisTID     = ActivatorTID();
    int hisTID_new = defaultTID(-1);

    SetActivator(myTID_new, AAPTR_TARGET);

    Thing_Damage2(hisTID_new, damage, "Normal");
    Thing_ChangeTID(hisTID_new, hisTID);
    Thing_ChangeTID(myTID_new, myTID_old);
}