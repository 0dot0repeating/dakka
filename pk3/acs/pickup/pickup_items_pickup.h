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
        "$DAKKA_PK_FISTS",
    },
    {
        "$GOTCHAINSAW",
        "$DAKKA_PK_SCRAPPER",
    },
    {
        "Picked up a pistol.",
        "$DAKKA_PK_PISTOLS",
    },
    {
        "$GOTSHOTGUN",
        "$DAKKA_PK_SHOTGUN",
    },
    {
        "$GOTSHOTGUN2",
        "$DAKKA_PK_SUPERSHOTGUN",
    },
    {
        "$GOTCHAINGUN",
        "$DAKKA_PK_MINIGUN",
    },
    {
        "$GOTLAUNCHER",
        "$DAKKA_PK_IMPALER",
    },
    {
        "$GOTPLASMA",
        "$DAKKA_PK_PLASMALANCE",
    },
    {
        "$GOTBFG9000",
        "$DAKKA_PK_BFG9000",
    },
    {
        "$GOTSHOTGUN",
        "$DAKKA_PK_SHOTGUN",
    },
    {
        "$GOTCHAINGUN",
        "$DAKKA_PK_MINIGUN",
    },
    
    
    {
        "$GOTARMOR",
        "Green armor!",
    },
    {
        "$GOTMEGA",
        "Blue armor!",
    },


    {
        "$PICKUP_MINIGUN",
        "$DAKKA_PK_MINIGUN",
    },
    {
        "$PICKUP_GRENADELAUNCHER",
        "$DAKKA_PK_IMPALER",
    },
    {
        "$PICKUP_RAILGUN",
        "$DAKKA_PK_PLASMALANCE",
    },
    {
        "$PICKUP_BFG10k",
        "$DAKKA_PK_BFG9000",
    },
};



// What pickup flash should the item use? This is only used if the pickup
//  message isn't entirely scripted. Array entries are in the order:
//
// {red [0-255], green [0-255], blue [0-255], alpha [0-1.0], duration [seconds, fixed]}
//
// Normal pickup flash is {255, 255, 64, 0.15, 0.15}.

#define PFLASH_RED      0
#define PFLASH_GREEN    1
#define PFLASH_BLUE     2
#define PFLASH_ALPHA    3
#define PFLASH_TIME     4

int PKP_PickupFlashes[PICKUPCOUNT][CLASSCOUNT][5] =
{
    // Weapons
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    
    // Armor
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    

    // Skulltag pickups
    { // Minigun
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    { // Grenade launcher
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    { // Railgun
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    { // BFG10k
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },  
};



// Pickup sound? Not much to say here.
int PKP_PickupSounds[PICKUPCOUNT][CLASSCOUNT] =
{
    // Weapons
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
    

    // Armor
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },


    // Skulltag pickups
    { // Minigun
        "misc/w_pkup",
        "misc/w_pkup",
    },
    { // Grenade launcher
        "misc/w_pkup",
        "misc/w_pkup",
    },
    { // Railgun
        "misc/w_pkup",
        "misc/w_pkup",
    },
    { // BFG10k
        "misc/w_pkup",
        "misc/w_pkup",
    },
};
