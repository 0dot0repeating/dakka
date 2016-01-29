// Used in pickup_daemon.h
#define PICKUP_MAINLOOP     281
#define PICKUP_ENTER        282
#define PICKUP_DISCONNECT   283
#define PICKUP_OPEN_CLIENT  284
#define PICKUP_WEAPONSWITCH 285

// Used in pickup_client.h
#define PICKUP_SENDTOCLIENT 286
#define PICKUP_PINGSERVER   287

// Used in pickup_display.h
#define PICKUP_DISPLAY      288

// Used in pickup_pickup.h
#define PICKUP_PICKUP       289
#define PICKUP_SHOWMESSAGE  290

// Used in pickup.c because of how small a script it is
#define PICKUP_DROPPED      291



// There's not much we can do to make client-specific display work in ZDoom
//  multiplayer. But at least we can *try* to not have things break horribly,
//  at least in single player.
int IsZandronum = -1;


// Also for Zandronum.
int IsServer;


// For weapon pickup.
// - 0 means "never switch".
// - 1 means "switch on better".
// - 2 means "always switch".
int C2S_WeaponSwitchState[PLAYERMAX];
