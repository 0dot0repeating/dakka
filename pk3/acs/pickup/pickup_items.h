// Defining pickups themselves is handled here.


// This is, quite simply, the amount of pickup types that this ACS is equipped
//  to handle.
#define PICKUPCOUNT     1


// How many items can you get from each pickup?
#define PK_RECEIVECOUNT 1


// Define item constants here. Because "item 0" doesn't mean shit.
#define It_Chainsaw     0


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
        {"Chainsaw"},
        {"DWep_Scrappers"},
    },
};



// How many of those items are we going to get?
int PKP_ReceiveCount[PICKUPCOUNT][CLASSCOUNT][PK_RECEIVECOUNT] =
{
    {
        {1},
        {1},
    }
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
};
