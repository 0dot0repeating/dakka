// All constant definitions that can be changed freely are here.
//  Do not *delete* anything, though. All the definitions and arrays are
//  necessary, and deleting them will cause compiling to inevitably fail.
//
// Well, okay, you probably don't need to mess with the ARCDATA_* and ADATA_*
//  stuff. But the STR_ArcData and INT_ArcData stuff is fine.

// Core arc script number. This is also in decorate/core.dec, so be sure
//  to change it there as well.

#define ARC_MAIN        191
#define ARC_CLEAR       192



// Arc types need to be defined in ACS, because there's no wayt to store all
//  the information on DECORATE actors (namely strings), and plus that'd be
//  ripe for breakage.

#define ARCTYPES    2

// Arcer properties. Due to ACS limitations, ints and strings have to be stored
//  in different arrays.

#define ARCDATA_INT 6
#define ARCDATA_STR 1

// Maximum distance arc can look.
#define ADATA_INT_MAXRANGE      0

// How much should the angle between the target and arcer matter?
#define ADATA_INT_ANGLEWEIGHT   1

// Which direction should the new arcer face? Towards the new target (1.0),
//  in the same direction the current arc is facing (0), or in between?
#define ADATA_INT_FACINGWEIGHT  2

// Instead of the current arc, should we use the *firer's* angle?
//  A_BFGSpray antics ahoy!
#define ADATA_INT_FACINGFIRER   3

// Should this arcer not automatically clear target data when it's done arcing?
//      
// This is meant for spawning multiple arcers at once that shouldn't overlap.
//  Make damn sure you call ARC_CLEAR when you're done.
#define ADATA_INT_MANUALCLEAR   4


// Do we *really* need to be in the center of the actor? This adds random variance
//  to where the new arc actor spawns. It's not based off the target's height
//  and width because Zandronum doesn't have that, but once it gets it,
//  it probably will be.
#define ADATA_INT_RANDOMOFFSET  5


// Name of arc actor to be spawned on a found target.
#define ADATA_STR_NEXTARCER 0
str STR_ArcData[ARCTYPES][ARCDATA_STR] =
{
    {"Channeler_PrimaryBase"},
    {"Channeler_SecondaryBase"},
};

int INT_ArcData[ARCTYPES][ARCDATA_INT] = 
{
    {288.0, 0.75,  0.5, false, false, 8.0},
    {192.0, 0.25,  0.0, false, false, 8.0},
};
