// Defining pickups themselves is handled here.


// This is, quite simply, the amount of pickup types that this ACS is equipped
//  to handle.
#define PICKUPCOUNT     14


// How many items can you get from each pickup?
#define PK_RECEIVECOUNT 1


// Define item constants here. Because "item 0" doesn't mean shit.
#define It_Clip         0
#define It_ClipBox      1
#define It_Shell        2
#define It_ShellBox     3
#define It_Rocket       4
#define It_RocketBox    5
#define It_Cell         6
#define It_CellPack     7
#define It_Stim         8
#define It_HealthBonus  9
#define It_Shotgun      10
#define It_GreenArmor   11
#define It_BlueArmor    12
#define It_ArmorBonus   13


// Here, you have the items that each pickup will give you. Each pickup can give
//  as many as PK_RECEIVECOUNT items.
//
// Note that index 0 is not for class 0 in pickup_classes.h, but actually for
//  Cl_Unknown.
//
// And we group by pickup and not by class because it's more useful contextually
//  to see "oh, this is the place where everyone gets health bonuses" than "oh,
//  he gets a soulsphere up there and a stimpack up there so this is the health
//  bonus index I guess... or is it megasphere?".
//
// Talking from experience.

int PKP_ReceiveItems[PICKUPCOUNT][CLASSCOUNT][PK_RECEIVECOUNT] =
{
    {
        {"Clip"},
        {"Clip"},
    },
    {
        {"Clip"},
        {"Clip"},
    },
    {
        {"Shell"},
        {"Clip"},
    },
    {
        {"Shell"},
        {"Clip"},
    },
    {
        {"RocketAmmo"},
        {"Clip"},
    },
    {
        {"RocketAmmo"},
        {"Clip"},
    },
    {
        {"Cell"},
        {"Clip"},
    },
    {
        {"Cell"},
        {"Clip"},
    },
    {
        {"BaseHealth"},
        {"BaseHealth"},
    },
    {
        {"HealthBonus"},
        {"DakkaHealthBonus"},
    },
    {
        {"Shotgun"},
        {"Shotgun"},
    },
    {
        {"GreenArmor"},
        {"GreenArmor"},
    },
    {
        {"BlueArmor"},
        {"BlueArmor"},
    },
    {
        {"ArmorBonus"},
        {"ArmorBonus"},
    },
};



// How many of those items are we going to get?
int PKP_ReceiveCount[PICKUPCOUNT][CLASSCOUNT][PK_RECEIVECOUNT] =
{
    {
        {10},
        {10},
    },
    {
        {50},
        {50},
    },
    {
        {4},
        {10},
    },
    {
        {20},
        {50},
    },
    {
        {1},
        {10},
    },
    {
        {5},
        {50},
    },
    {
        {20},
        {10},
    },
    {
        {100},
        {50},
    },
    {
        {10},
        {10},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
};



// And should this item *always* be picked up?
// This not only forces a pickup message, but also forces consumption
//  of the pickup. PARRAY_NOCONSUME will still block consumption, however.
int PKP_AlwaysPickup[PICKUPCOUNT][CLASSCOUNT] =
{
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        true,
        true,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        true,
        true,
    },
};
