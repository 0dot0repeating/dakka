// Pickup messages. Get some.
//
// I'm tempted to add pickup_cl_moremessages to this, but no.
//
// You want that, you can add it yourself. 
// I'll try to make that as easy as possible.

int PKP_Messages[PICKUPCOUNT][CLASSCOUNT] = 
{
    {
        "Picked up a clip.",
        "Picked up a clip.",
    },
    {
        "Picked up a box of bullets.",
        "Picked up a box of bullets.",
    },
    {
        "Picked up 4 shotgun shells.",
        "Picked up a clip.",
    },
    {
        "Picked up a box of shotgun shells.",
        "Picked up a box of bullets.",
    },
    {
        "Picked up a rocket.",
        "Picked up a clip.",
    },
    {
        "Picked up a box of rockets.",
        "Picked up a box of bullets.",
    },
    {
        "Picked up an energy cell.",
        "Picked up a clip.",
    },
    {
        "Picked up an energy cell pack.",
        "Picked up a box of bullets.",
    },
    {
        "$GOTSTIM",
        "$GOTSTIM",
    },
    {
        "$GOTHTHBONUS",
        "$GOTHTHBONUS",
    },
    {
        "$GOTSHOTGUN",
        "$GOTSHOTGUN",
    },
    {
        "$GOTARMOR",
        "$GOTARMOR",
    },
    {
        "$GOTMEGA",
        "$GOTMEGA",
    },
    {
        "$GOTARMBONUS",
        "$GOTARMBONUS",
    },
};



// What pickup flash should the item use? This is only used if the pickup
//  message isn't entirely scripted. Array entries are in the order:
//
// {red [0-255], green [0-255], blue [0-255], alpha [0-1.0], duration [seconds, fixed]}
//
// Normal pickup flash is {255, 255, 64, 0.1, 0.25}.

#define PFLASH_RED      0
#define PFLASH_GREEN    1
#define PFLASH_BLUE     2
#define PFLASH_ALPHA    3
#define PFLASH_TIME     4

int PKP_PickupFlashes[PICKUPCOUNT][CLASSCOUNT][5] =
{
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
};



// Pickup sound? Not much to say here.
int PKP_PickupSounds[PICKUPCOUNT][CLASSCOUNT] =
{
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
};
