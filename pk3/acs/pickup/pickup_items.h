// Defining pickups themselves is handled here.


// This is, quite simply, the amount of pickup types that this ACS is equipped
//  to handle.
#define PICKUPCOUNT     39


// How many items can you get from each pickup?
#define PK_RECEIVECOUNT 5


// Define item constants here. Because "item 0" doesn't mean shit.
#define It_Fist             0
#define It_Chainsaw         1
#define It_Pistol           2
#define It_Shotgun          3
#define It_SuperShotgun     4
#define It_Chaingun         5
#define It_RocketLauncher   6
#define It_PlasmaRifle      7
#define It_BFG9000          8

#define It_ShotgunDropped   9
#define It_ChaingunDropped  10

#define It_Clip             11
#define It_Shell            12
#define It_RocketAmmo       13
#define It_Cell             14
#define It_ClipBox          15
#define It_ShellBox         16
#define It_RocketBox        17
#define It_CellPack         18

#define It_HealthBonus      19
#define It_Stimpack         20
#define It_Medikit          21
#define It_Soulsphere       22

#define It_ArmorBonus       23
#define It_GreenArmor       24
#define It_BlueArmor        25
#define It_Megasphere       26

#define It_Berserk          27
#define It_Blursphere       28
#define It_InvulnSphere     29
#define It_Backpack         30

#define It_Dakka_Channeler      31
#define It_Dakka_PlasmaBundle   32

#define It_Dakka_Impaler        33
#define It_Dakka_BFGBundle      34

#define It_ST_Minigun           35
#define It_ST_GrenadeLauncher   36
#define It_ST_Railgun           37
#define It_ST_BFG10k            38

// Here, you have the items that each pickup will give you. Each pickup can give
//  as many as PK_RECEIVECOUNT items.
//
// Note that index 0 is not for class 0 in pickup_classes.h, but actually for
//  Cl_Unknown.
//
// And we group by pickup and not by class because it's more useful contextually
//  to see "oh, this is the place where everyone gets health bonuses" than "oh,
//  he gets a soulsphere up there and a stimpack up there so this is the health
//  bonus index I guess... or is it megasphere?".
//
// Talking from experience.

int PKP_ReceiveItems[PICKUPCOUNT][CLASSCOUNT][PK_RECEIVECOUNT] =
{
    {
        {"Fist"},
        {"DWep_Fists"},
    },
    {
        {"Chainsaw"},
        {"DWep_Scrappers"},
    },
    {
        {"Pistol"},
        {"DWep_Pistol", "DWep_Pistols"},
    },
    {
        {"Shotgun"},
        {"DWep_Shotgun"},
    },
    {
        {"SuperShotgun"},
        {"DWep_SuperShotgun"},
    },
    {
        {"Chaingun"},
        {"DWep_Chaingun"},
    },
    {
        {"RocketLauncher"},
        {"DWep_RocketLauncher"},
    },
    {
        {"PlasmaRifle"},
        {"DWep_PlasmaRifle"},
    },
    {
        {"BFG9000"},
        {"DWep_BFG9000"},
    },
    { // Dropped
        {"Shotgun"},
        {"DWep_Shotgun"},
    },
    { // Dropped
        {"Chaingun"},
        {"DWep_Chaingun"},
    },


    {
        {"Clip"},
        {"DakkaBullet_10mm", "DakkaBullet_5mm"},
    },
    {
        {"Shell"},
        {"DakkaShells"},
    },
    {
        {"RocketAmmo"},
        {"DakkaRockets"},
    },
    {
        {"Cell"},
        {"DakkaCells"},
    },

    {
        {"ClipBox"},
        {"DakkaBullet_10mm", "DakkaBullet_5mm", "DakkaRainGrenades", "DakkaFlamerFuel"},
    },
    {
        {"Shell"},
        {"DakkaShells", "DakkaShotGrenades", "DakkaGyroShells"},
    },
    {
        {"RocketAmmo"},
        {"DakkaRockets", "DakkaShotGrenades", "DakkaGyroShells", "DakkaRainGrenades"},
    },
    {
        {"Cell"},
        {"DakkaCells", "DakkaFlamerFuel"},
    },


    {
        {"HealthBonus"},
        {"DakkaHealthBonus"},
    },
    {
        {"BaseHealth"},
        {"BaseHealth"},
    },
    {
        {"Medikit"},
        {"BaseHealth"},
    },
    {
        {"HealthBonus"},
        {"DakkaSoulsphere"},
    },


    {
        {"ArmorBonus"},
        {"Dakka_ArmorBonus"},
    },
    {
        {"GreenArmor"},
        {"Dakka_GreenArmor"},
    },
    {
        {"BlueArmor"},
        {"Dakka_BlueArmor"},
    },
    {
        {"MegasphereHealth", "BlueArmor"},
        {"DakkaMegasphere",  "Dakka_BlueArmor"},
    },


    {
        {"FullHealth", "PowerStrength", "SwitchToFist"},
        {"DWep_Minigun"},
    },
    {
        {"Blursphere"},
        {"Blursphere"},
    },
    {
        {"InvulnerabilitySphere"},
        {"DakkaInvulnerable"},
    },
    {
        {"Backpack"},
        {"Backpack"},
    },


    { // Dakka-specific item: channeler
        {"PlasmaRifle"}, // shouldn't be able to get this
        {"DWep_Channeler"},
    },
    { // Dakka-specific item: channeler+plasma bundle
        {"PlasmaRifle"},
        {"DWep_PlasmaRifle", "DWep_Channeler"},
    },
    { // Dakka-specific item: impaler
        {"BFG9000"},
        {"DWep_Impaler"},
    },
    { // Dakka-specific item: BFG+impaler bundle
        {"BFG9000"},
        {"DWep_BFG9000", "DWep_Impaler"},
    },



    // Skulltag pickups

    { // Minigun
        {"Minigun"},
        {"DWep_Minigun"},
    },
    { // Grenade launcher
        {"GrenadeLauncher"},
        {"DWep_RocketLauncher"},
    },
    { // Railgun
        {"Railgun"},
        {"DWep_Channeler"},
    },
    { // BFG10k
        {"BFG10k"},
        {"DWep_Impaler"},
    },
};



// How many of those items are we going to get?
int PKP_ReceiveCount[PICKUPCOUNT][CLASSCOUNT][PK_RECEIVECOUNT] =
{
    // Weapons
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1,1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },


    // Ammo
    {
        {10},
        {16, 5},
    },
    {
        {4},
        {4},
    },
    {
        {1},
        {2},
    },
    {
        {20},
        {20},
    },
    {
        {50},
        {50, 100, 6, 12},
    },
    {
        {20},
        {20, 4, 8},
    },
    {
        {5},
        {10, 3, 6, 5},
    },
    {
        {100},
        {100, 12},
    },


    // Health
    {
        {1},
        {1},
    },
    {
        {10},
        {10},
    },
    {
        {25},
        {25},
    },
    {
        {100},
        {50},
    },


    // Armor
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    {
        {1},
        {1},
    },
    { // Megasphere
        {1, 1},
        {1, 2},
    },


    { // Berserk
        {1, 1, 1},
        {1},
    },

    { // Blursphere
        {1},
        {1},
    },

    { // Invuln
        {1},
        {1},
    },

    { // Backpack
        {1},
        {1},
    },


    { // Channeler - doesn't give its own cells, gotta do it manually
        {1},
        {2},
    },
    { // Plasma+channeler bundle
        {1},
        {1, 1},
    },
    { // Impaler
        {1},
        {1},
    },
    { // BFG+impaler bundle
        {1},
        {1, 1},
    },

    

    // Skulltag pickups
    { // Minigun
        {1},
        {1},
    },
    { // Grenade launcher
        {1},
        {1},
    },
    { // Railgun
        {1},
        {1},
    },
    { // BFG10k
        {1},
        {1},
    },
};



// And should this item *always* be picked up?
// This not only forces a pickup message, but also forces consumption
//  of the pickup. PARRAY_NOCONSUME will still block consumption, however.
int PKP_AlwaysPickup[PICKUPCOUNT][CLASSCOUNT] =
{
    // Weapons
    { // 0
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    { // 4
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    { // 9
        false,
        false,
    },
    {
        false,
        false,
    },


    // Ammo
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    { // 14
        false,
        false,
    },

    // Ammo (big)
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },


    // Health
    { // 19
        true,
        true,
    },
    {
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        true,
        true,
    },


    // Armor
    {
        true,
        true,
    },
    { // 24
        false,
        false,
    },
    {
        false,
        false,
    },
    {
        true,
        true,
    },


    
    { // Berserk
        true,
        false,
    },

    { // Blursphere
        true,
        true,
    },

    { // (29) Invuln
        true,
        true,
    },

    { // Backpack
        true,
        true,
    },


    { // Channeler
        false,
        false,
    },
    { // Plasma+channeler
        false,
        false,
    },
    { // Impaler
        false,
        false,
    },
    { // (34) BFG+impaler
        false,
        false,
    },



    // Skulltag pickups
    { // Minigun
        false,
        false,
    },
    { // Grenade launcher
        false,
        false,
    },
    { // Railgun
        false,
        false,
    },
    { // BFG10k
        false,
        false,
    },
};
