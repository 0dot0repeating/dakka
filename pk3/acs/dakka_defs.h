#define SSG_SHOTMAX     2

// Absorbed from pickup/pickup_const.h
//
// There's not much we can do to make client-specific display work in ZDoom
//  multiplayer. But at least we can *try* to not have things break horribly,
//  at least in single player.
int IsZandronum;

// Also for Zandronum.
int IsServer;


// 0 is SSG, 1 is BFG
int AutoReloadCooldowns[PLAYERMAX][2];


#define BACKPACKCOUNT   5

int Dakka_BackpackItems[BACKPACKCOUNT] =
{
    "BackpackItem",
    "DakkaBackpackItem",
    "Backpack",
    "BagOfHolding",
    "AmmoSatchel",
};
