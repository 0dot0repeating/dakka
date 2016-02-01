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
        "Scrappers! Wait, what?",
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


    {
        "$GOTCLIP",
        "Bullets",
    },
    {
        "$GOTSHELL",
        "Shells",
    },
    {
        "$GOTROCKET",
        "Rockets",
    },
    {
        "$GOTCELL",
        "Cells",
    },

    {
        "$GOTCLIPBOX",
        "Big bullets",
    },
    {
        "$GOTSHELLBOX",
        "Big shells",
    },
    {
        "$GOTROCKBOX",
        "Big rockets",
    },
    {
        "$GOTCELLBOX",
        "Big cells",
    },


    {
        "$GOTHTHBONUS",
        "Health bonus",
    },
    {
        "$GOTSTIM",
        "Stimpack",
    },
    {
        "$GOTMEDIKIT",
        "Medikit",
    },
    {
        "$GOTSUPER",
        "Soulsphere!",
    },


    {
        "$GOTARMBONUS",
        "Armor bonus",
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
        "$GOTMSPHERE",
        "Megasphere!",
    },


    {
        "$GOTBERSERK",
        "Minigun!",
    },
    {
        "$GOTINVIS",
        "Blursphere!",
    },
    {
        "$GOTINVUL",
        "Invulnerability!",
    },
    {
        "$GOTBACKPACK",
        "Backpack!",
    },


    {
        "$GOTPLASMA",
        "Channeler!",
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
    { // 0
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
    { // 4
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
    { // 9
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    {
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },


    // Ammo
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
    { // 14
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },

    // Ammo (big)
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


    // Health
    { // 19
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
    { // 24
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


    { // Berserk
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    { // Blursphere
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    { // (29) Invuln
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
    { // Backpack
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },


    { // Channeler
        {255, 255, 64, 0.15, 0.15},
        {255, 255, 64, 0.15, 0.15},
    },
};



// Pickup sound? Not much to say here.
int PKP_PickupSounds[PICKUPCOUNT][CLASSCOUNT] =
{
    // Weapons
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

    
    // Ammo
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    { // 14
        "misc/i_pkup",
        "misc/i_pkup",
    },

    // Ammo (big)
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },


    // Health
    { // 19
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/p_pkup",
        "misc/p_pkup",
    },

    // Armor
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    { // 24
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/i_pkup",
        "misc/i_pkup",
    },
    {
        "misc/p_pkup",
        "misc/p_pkup",
    },


    { // Berserk
        "misc/p_pkup",
        "misc/w_pkup",
    },
    { // Blursphere
        "misc/p_pkup",
        "misc/p_pkup",
    },
    { // (29) Blursphere
        "misc/p_pkup",
        "misc/p_pkup",
    },
    { // Backpack
        "misc/i_pkup",
        "misc/i_pkup",
    },


    { // Channeler
        "misc/w_pkup",
        "misc/w_pkup",
    },
};
