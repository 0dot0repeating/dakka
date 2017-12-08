// This stuff's used in dakka_translateguns.h and dakka_levelstart.h.
//
// dakka_startmode_weapons (see dakka_levelstart.h) has modes that automatically
//  sets your weapon loadout based off of it. See the corresponding files for
//  details (or the wiki eventually). The stuff for it is stored here.
//
// Dakka_ClassWeapons is also used by Dakka_TranslateGuns. (dakka_translateguns.h)

#define CLASSWEAPONS    22

#define CWEP_FIST               0
#define CWEP_CHAINSAW           1
#define CWEP_PISTOL             2
#define CWEP_PISTOLS            3
#define CWEP_SHOTGUN            4
#define CWEP_SUPERSHOTGUN       5
#define CWEP_CHAINGUN           6
#define CWEP_MINIGUN            7
#define CWEP_ROCKETLAUNCHER     8
#define CWEP_PLASMALANCE        9
#define CWEP_BFG9000            10


#define CWEP_HERETIC_STAFF      11
#define CWEP_HERETIC_GAUNTLETS  12
#define CWEP_HERETIC_GOLDWAND   13
#define CWEP_HERETIC_GOLDWAND2  14
#define CWEP_HERETIC_CROSSBOW   15
#define CWEP_HERETIC_CROSSBOW2  16
#define CWEP_HERETIC_BLASTER    17
#define CWEP_HERETIC_BLASTER2   18
#define CWEP_HERETIC_SKULLROD   19
#define CWEP_HERETIC_PHOENIXROD 20
#define CWEP_HERETIC_MACE       21

// First col is Doomguy
// Second col is Dakkaguy
//
// Blank strings here means "no weapon in this class weapon slot". For the
//  purposes of translation, a weapon that belongs to a different class that's
//  in a slot you have nothing in simply gets taken away.
//
// You can use CustomInventory here if you want.

int Dakka_ClassWeapons[CLASSWEAPONS][CLASSCOUNT] =
{
    {"Fist",            "DWep_Fist"},
    {"Chainsaw",        "DWep_Scrappers"},
    {"Pistol",          "DWep_Pistol"},
    {"Pistol",          "DWep_Pistols"},
    {"Shotgun",         "DWep_Shotgun"},
    {"SuperShotgun",    "DWep_SuperShotgun"},
    {"Chaingun",        "DWep_Chaingun"},
    {"Chaingun",        "DWep_Minigun"},
    {"RocketLauncher",  "DWep_Impaler"},
    {"PlasmaRifle",     "DWep_PlasmaLance"},
    {"BFG9000",         "DWep_BFG9000"},

    {"Staff",           "DWep_Fist"},
    {"Gauntlets",       "DWep_Scrappers"},
    {"GoldWand",        "DWep_Pistol"},
    {"GoldWand",        "DWep_Pistols"},
    {"Crossbow",        "DWep_Shotgun"},
    {"Crossbow",        "DWep_SuperShotgun"},
    {"Blaster",         "DWep_Chaingun"},
    {"Blaster",         "DWep_Minigun"},
    {"PhoenixRod",      "DWep_Impaler"},
    {"SkullRod",        "DWep_PlasmaLance"},
    {"Mace",            "DWep_BFG9000"},
};

// This is used to discriminate which weapons we should give. It doesn't
//  correspond with weapon slot because that doesn't necessarily indicate
//  the power of a weapon.
int Dakka_ClassWeaponPowers[CLASSWEAPONS][CLASSCOUNT] =
{
    { 1,  1}, // fist            / fists
    { 3,  5}, // chainsaw        / scrappers
    { 2,  2}, // pistol
    {-1,  2}, // none            / pistols
    { 3,  3}, // shotgun
    { 5,  5}, // SSG
    { 4,  4}, // chaingun
    {-1,  6}, // none            / minigun
    { 5,  5}, // rocket launcher / impaler
    { 6,  6}, // plasma rifle    / plasma lance
    { 7,  7}, // BFG

    { 1,  1}, // staff          (fist)
    { 3,  5}, // gauntlets      (scrappers)
    { 2,  2}, // wand 1         (pistol)
    { 2,  2}, // wand 2         (pistols)
    { 3,  3}, // crossbow 1     (shotgun)
    { 3,  5}, // crossbow 2     (ssg)
    { 4,  4}, // blaster 1      (chaingun)
    { 4,  6}, // blaster 2      (minigun)
    { 6,  5}, // phoenix rod    (impaler)
    { 5,  6}, // hellstaff 1    (plasma)
    { 7,  7}, // mace 1         (bfg)
};


// Should this weapon be ignored by dakka_startmode_weapons?
// Include fist/pistol weapons in this so it never tries to take those away.
int Dakka_ClassWep_StartModeIgnore[CLASSWEAPONS][CLASSCOUNT] =
{
    {true,  true},
    {false, false},
    {true,  true},
    {true,  true},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
    {false, false},

    {true,  true},
    {false, false},
    {true,  true},
    {true,  true},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
    {false, false},
};


// How many non-class guns need to be translated?
#define TRANSLATE_GUNS  11

// How many different guns can a non-class gun become?
#define TRANSLATE_MAXTO 2

// Items that aren't class weapons go in here. This simply stores the item to
//  be taken away.

int Dakka_TranslatableGuns_From[TRANSLATE_GUNS] =
{
    "Fist2",
    "Chainsaw2",
    "Pistol2",
    "Shotgun2",
    "SuperShotgun2",
    "Chaingun2",
    "RocketLauncher2",
    "PlasmaRifle2",
    "BFG90002",
    "BFG9001",
    "BFG9002",
};


// What should the guns translate into?
// For the purposes of this, 
int Dakka_TranslatableGuns_To[TRANSLATE_GUNS][CLASSCOUNT][TRANSLATE_MAXTO] =
{
    { // Fist2
        {""},
        {"DWep_Fist"},
    },

    { // Chainsaw2
        {""},
        {"DWep_Scrappers"},
    },

    { // Pistol2
        {""},
        {"DWep_Pistol", "DWep_Pistols"},
    },

    { // Shotgun2
        {""},
        {"DWep_Shotgun"},
    },

    { // SuperShotgun2
        {""},
        {"DWep_SuperShotgun"},
    },

    { // Chaingun2
        {""},
        {"DWep_Chaingun", "DWep_Minigun"},
    },

    { // RocketLauncher2
        {""},
        {"DWep_Impaler"},
    },

    { // PlasmaRifle2
        {""},
        {"DWep_PlasmaLance"},
    },

    { // BFG90002
        {""},
        {"DWep_BFG9000"},
    },

    { // BFG9001
        {""},
        {"DWep_BFG9000"},
    },

    { // BFG9002
        {""},
        {"DWep_BFG9000"},
    },
};
