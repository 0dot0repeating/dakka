// Again, could've just kept this stuff in pickup_server, but it's nice to have
//  the stuff you're meant to edit be in its own file, easy to distinguish.


// This defines how many different data points we should sync between the client
//  and the server.
#define C2S_DATACOUNT   1


// Constants for identifying which data point we want to modify.

// Tells the server how the client handles weapon switching on pickup.
#define C2S_D_NOSCOREREWARDS    0
