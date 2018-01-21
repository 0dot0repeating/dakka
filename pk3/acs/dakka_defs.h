#define SSG_SHOTMAX     2

// Absorbed from pickup/pickup_const.h
//
// There's not much we can do to make client-specific display work in ZDoom
//  multiplayer. But at least we can *try* to not have things break horribly,
//  at least in single player.
int IsZandronum = -1;

// Also for Zandronum.
int IsServer;



int SSG_AutoReloadCooldown[PLAYERMAX];


#define BACKPACKCOUNT   5

int Dakka_BackpackItems[BACKPACKCOUNT] =
{
    "BackpackItem",
    "DakkaBackpackItem",
    "Backpack",
    "BagOfHolding",
    "AmmoSatchel",
};
