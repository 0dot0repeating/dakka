// I could've kept this stuff in pickup_client.h, but I'd rather have clearer
//  delineation between "stuff that's meant to be tinkered with" and "system
//  stuff that shouldn't be tinkered with".


// This defines how many different data points we should sync between the server
//  and the clients.
#define S2C_DATACOUNT   6


// Constants for identifying which data point we want to modify.

#define S2C_D_CLASSNUM      0
#define S2C_D_HEALTH        1
#define S2C_D_CLIP          2
#define S2C_D_SHELL         3
#define S2C_D_ROCKET        4
#define S2C_D_CELL          5

