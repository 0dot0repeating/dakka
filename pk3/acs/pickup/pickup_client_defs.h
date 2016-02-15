// I could've kept this stuff in pickup_client.h, but I'd rather have clearer
//  delineation between "stuff that's meant to be tinkered with" and "system
//  stuff that shouldn't be tinkered with".


// This defines how many different data points we should sync between the server
//  and the clients.
#define S2C_DATACOUNT   21


// Constants for identifying which data point we want to modify.

#define S2C_D_CLASSNUM      0

// For Dakkaguy. If he started the map with a plasma rifle, this is true.
#define S2C_D_PLASMASTART   1

// Tells the clients what dakka_bundleplasma is set to.
#define S2C_D_BUNDLEPLASMA  2

// Also for Dakkaguy. If he started the map with a BFG, this is true.
#define S2C_D_BFGSTART      3

// Tells the clients what dakka_bundlebfg is set to.
#define S2C_D_BUNDLEBFG     4

// Whether shotgun items (that spawned on the map) should show as SSGs instead.
#define S2C_D_SHOT2SSG      5

// Ditto, for the chaingun and minigun.
#define S2C_D_CHAIN2MINI    6

// Send score to client so he can display it himself.
#define S2C_D_SCORE         7

// And the score threshold as well.
#define S2C_D_GOALSCORE     8


// All of these are bonus values. See acs/score/score_defs.h for what they
//  correspond to.
#define S2C_D_BONUSSTART    7

#define S2C_D_BS_BASE         4
#define S2C_D_BS_SPREE        5
#define S2C_D_BS_EFFICIENT    6
#define S2C_D_BS_INFIGHTER    7
#define S2C_D_BS_SWITCHAROO   8
#define S2C_D_BS_SCRAPPING    9
#define S2C_D_BS_CURVEBALL    10
#define S2C_D_BS_AIR          11
#define S2C_D_BS_BONEDRY      12
#define S2C_D_BS_SORELOSER    13
#define S2C_D_BS_BRAWLER      14
#define S2C_D_BS_POINTBLANK   15
