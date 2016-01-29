// How many underscores can we fit in a name? I don't know.

// Okay, fuck ACS return values. Fuck script arguments. You don't get nearly
//  enough of them, so we're doing this the hacky way: with ARRAYS.


// PKP_PickupData is used in the PICKUP_PICKUP script. It's filled with relevant
//  data regarding the pickup before any proper pickup code is run.

#define PDATA_SLOTS     3

// Item number for the pickup. If you're using a scripted pickup that returns a
//  new item number, it'll get updated when the new item number is returned.
#define PDATA_ITEMNUM   0

// Class number for the pickup.
#define PDATA_CLASSNUM  1

// Was the item dropped?
#define PDATA_DROPPED   2

int PKP_PickupData[PDATA_SLOTS];



// PKP_ReturnArray is used in the PICKUP_PICKUP script. When the script's called,
//  the entire array is cleared, and whatever pickup behaviour runs, it fills up
//  the return array. Once the pickup scripts or whatever are done doing their
//  thing, PICKUP_PICKUP uses what's in PKP_ReturnArray to determine whether the
//  pickup succeeded, whether the pickup should be consumed, etc.

#define PARRAY_SLOTS    5

// Did the pickup succeed? Should we display a pickup message?
#define PARRAY_SUCCEEDED        0

// Should we consume the pickup? A successful pickup doesn't necessarily imply
//  that we should consume it.
#define PARRAY_CONSUME          1

// If this is set, the pickup will not be consumed, regardless of the status of
//  PARRAY_CONSUME.
#define PARRAY_NOCONSUME        2

// If the pickup didn't succeed but it still did something, set this one to
//  true. Otherwise, all pickups for that pickup type, for that player, on that
//  tic will be disabled, to speed up performance.
#define PARRAY_DIDSOMETHING     3


// This is purely for HEAL_PickupHealth. If we got a low health value, stick
//  the health index for the low health message in here.
#define PARRAY_LOWHEALTH        4

int PKP_ReturnArray[PARRAY_SLOTS];



// Thankfully, ACS_ExecuteAlways calls are contiguous, even online. Otherwise,
//  we wouldn't be able to safely pass arguments to the player.
//
// Anyway, three arguments wasn't enough for me to pass everything in with
//  Dakka, so we're doing it the array way. And since we're going from server
//  to client with this, we need to send the array arguments... two at a time.
//
// Sucks.
//
// Any data about the pickup that all clients *need* to know should be passed
//  through this array. Inventory items are only guaranteed to be known to the
//  player with them in their inventory, and new client data from pickup_client
//  is not at all guaranteed to arrive before the pickup script calls do.

#define MDATA_SLOTS 4

// What class number did the pickup? It might've actually changed before the
//  client's gotten a chance to find out.
#define MDATA_CLASSNUM     0

// Item number?
#define MDATA_ITEMNUM      1

// Was it dropped?
#define MDATA_DROPPED      2

// Should we show a low health message?
#define MDATA_LOWHEALTH    3

int PKP_MessageData[MDATA_SLOTS];

