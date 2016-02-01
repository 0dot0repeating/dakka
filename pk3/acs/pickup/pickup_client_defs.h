// I could've kept this stuff in pickup_client.h, but I'd rather have clearer
//  delineation between "stuff that's meant to be tinkered with" and "system
//  stuff that shouldn't be tinkered with".


// This defines how many different data points we should sync between the server
//  and the clients.
#define S2C_DATACOUNT   16


// Constants for identifying which data point we want to modify.

#define S2C_D_CLASSNUM      0

// For Dakkaguy. If he started the map with a plasma rifle, this is true.
#define S2C_D_PLASMASTART   1


// Send score to client so he can display it himself.
#define S2C_D_SCORE         2

// And the score threshold as well.
#define S2C_D_GOALSCORE     3


// All of these are bonus values. See acs/score/score_defs.h for what they
//  correspond to.
#define S2C_D_BONUSSTART    4

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
