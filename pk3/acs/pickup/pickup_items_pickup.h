// Pickup messages. Get some.
//
// I'm tempted to add pickup_cl_moremessages to this, but no.
//
// You want that, you can add it yourself. 
// I'll try to make that as easy as possible.

int PKP_Messages[PICKUPCOUNT][CLASSCOUNT] = 
{
    {
        "You got a pair of hands!",
        "Two hands!",
    },
    {
        "$GOTCHAINSAW",
        "Scrappers!",
    },
    {
        "Picked up a pistol.",
        "Pistols!",
    },
    {
        "$GOTSHOTGUN",
        "Shotgun!",
    },
    {
        "$GOTSHOTGUN2",
        "Super shotgun!",
    },
    {
        "$GOTCHAINGUN",
        "Chaingun!",
    },
    {
        "$GOTLAUNCHER",
        "Rocket launcher!",
    },
    {
        "$GOTPLASMA",
        "Plasma rifle!",
    },
    {
        "$GOTBFG9000",
        "\caBig feffin' gun!",
    },
    {
        "$GOTSHOTGUN",
        "Shotgun!",
    },
    {
        "$GOTCHAINGUN",
        "Chaingun!",
    },
};



// What pickup flash should the item use? This is only used if the pickup
//  message isn't entirely scripted. Array entries are in the order:
//
// {red [0-255], green [0-255], blue [0-255], alpha [0-1.0], duration [seconds, fixed]}
//
// Normal pickup flash is {255, 255, 64, 0.1, 0.25}.

#define PFLASH_RED      0
#define PFLASH_GREEN    1
#define PFLASH_BLUE     2
#define PFLASH_ALPHA    3
#define PFLASH_TIME     4

int PKP_PickupFlashes[PICKUPCOUNT][CLASSCOUNT][5] =
{
    { // 0
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    { // 4
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    { // 9
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
    {
        {255, 255, 64, 0.1, 0.25},
        {255, 255, 64, 0.1, 0.25},
    },
};



// Pickup sound? Not much to say here.
int PKP_PickupSounds[PICKUPCOUNT][CLASSCOUNT] =
{
    { // 0
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    { // 4
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
    { // 9
        "misc/w_pkup",
        "misc/w_pkup",
    },
    {
        "misc/w_pkup",
        "misc/w_pkup",
    },
};
