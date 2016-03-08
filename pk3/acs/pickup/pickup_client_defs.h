// I could've kept this stuff in pickup_client.h, but I'd rather have clearer
//  delineation between "stuff that's meant to be tinkered with" and "system
//  stuff that shouldn't be tinkered with".


// This defines how many different data points we should sync between the server
//  and the clients.
#define S2C_DATACOUNT   24


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

// This one's independent from the score rewards.
#define S2C_D_DISPLAYSCORE  8

// And the score threshold as well.
#define S2C_D_GOALSCORE     9

// How many extra lives do we have?
#define S2C_D_LIVESLEFT     10

// Which reward number are we on?
#define S2C_D_REWARDCOUNT   11


// All of these are bonus values. See acs/score/score_defs.h for what they
//  correspond to.
#define S2C_D_BONUSSTART    12

#define S2C_D_BS_BASE         12
#define S2C_D_BS_SPREE        13
#define S2C_D_BS_EFFICIENT    14
#define S2C_D_BS_INFIGHTER    15
#define S2C_D_BS_SWITCHAROO   16
#define S2C_D_BS_SCRAPPING    17
#define S2C_D_BS_CURVEBALL    18
#define S2C_D_BS_AIR          19
#define S2C_D_BS_BONEDRY      20
#define S2C_D_BS_SORELOSER    21
#define S2C_D_BS_BRAWLER      22
#define S2C_D_BS_POINTBLANK   23
