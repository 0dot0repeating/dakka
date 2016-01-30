// Used in pickup_daemon.h
#define PICKUP_MAINLOOP     281
#define PICKUP_ENTER        282
#define PICKUP_DISCONNECT   283
#define PICKUP_OPEN_CLIENT  284

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

// Used in pickup_server.h
#define PICKUP_SENDTOSERVER 292
#define PICKUP_PINGCLIENT   293



// There's not much we can do to make client-specific display work in ZDoom
//  multiplayer. But at least we can *try* to not have things break horribly,
//  at least in single player.
int IsZandronum = -1;


// Also for Zandronum.
int IsServer;
