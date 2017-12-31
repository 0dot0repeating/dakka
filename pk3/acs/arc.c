#library "arc"
#include "zcommon.acs"
#include "dakka_commonFuncs.h"

// Arc code, take three. This time, I try to minimize TID use as much as I can,
//  because the previous arc code irreversibly trashed the TIDs of everything it
//  looked at and I can't see any way to fix it without ripping out the logic
//  entirely.
//
// Instead of using A_Look and keeping track of what's been seen through TIDs,
//  I will use A_RadiusGive and run the target-weighing scripts on them, using
//  SetPointer and user variables to keep track of the best target so far.
// 
// Instead of keeping track of previously hit targets in a TID array, I will
//  iterate through the previous arcers and check their AAPTR_TRACER fields to
//  see if we have a match.
//
// Hopefully this works out better. We can only hope.


// ZDoom's no-team index (255) is different than Zandronum's no-team index (4).
int NoTeamIndex;

script "Arc_Open" open
{   
    SetDBEntry("dakka_arc", "client_iszand", 789237);
    NoTeamIndex = cond(GetDBEntry("pickup", "client_iszand") == 789237, 4, 255);
}