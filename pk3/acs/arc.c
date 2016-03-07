#library "arc"

#include "zcommon.acs"
#include "commonFuncs.h"

// Concepts you should be aware of:
//
// Targeting data:
//  Without special handling, A_LookEx will always find the same target, and it'll
//   be the first thing that a counterclockwise fan starting at due east hits,
//   damn how far away it is (as long as it's within range at all). So this ACS
//   has a system dedicated towards flipping the +FRIENDLY/-FRIENDLY flag on
//   any targets found, so that A_LookEx will actually look past them.
//
//  The first iteration of this arc code handled them... haphazardly. It took a
//   while to actually get it so that all the flags would be flipped properly,
//   and oftentimes one or two targets would slip by, causing, say, demons to
//   turn against their allies the moment you hit them with an arc.
//
//  The second iteration handled it slowly, which
//
//  I intend to avoid all of that this time.


// arc_const.h:
//   Stores constant definitions that you're meant to mess with.
//   Script numbers, arc definitions, stuff like that.
//   You can modify anything in this file, but NOT delete it.
//
// arc_user_const.h:
//   It's like arc_const.h, but meant for stuff in arc_user.h.
//   You can delete everything in here and the arc code will still work.
//   Just make sure to delete everything in arc_user.h as well.
//
// arc_target.h:
//   This defines the core functions for managing targeting data. Don't use
//   these directly.
//
// (arc_target_core.h and arc_target_user.h were the old system, the slow one)
//
// arc_look.h:
//   The nitty-gritty of how the arc acquires targets. It jumps between ACS and
//   DECORATE to get the job done, a messy but unavoidable fact.
//
// arc_angle.h:
//   This entire file is dedicated to calculating the angle of new arcers, as
//   well as the angle between an arcer and its target.
//
// arc_main.h:
//   Where most of it happens. This is where the main arc script is, the one
//   that handles shuffling pointers around, keeping the arc chain going, and
//   all that stuff.
//
// arc_user.h:
//   None of this is necessary for the arc stuff to run. Stick your auxillary
//   scripts and functions and whatnot in here.

#include "arc/arc_const.h"
#include "arc/arc_user_const.h"
#include "arc/arc_target.h"
#include "arc/arc_look.h"
#include "arc/arc_angle.h"
#include "arc/arc_main.h"
#include "arc/arc_user.h"


script "Arc_Open" open
{
    SetDBEntry("pickup", "client_iszand", true);
    IsZandronum = GetDBEntry("pickup", "client_iszand") == 1;
}
