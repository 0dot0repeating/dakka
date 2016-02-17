// This stuff's used in dakka_translateguns.h and dakka_levelstart.h.
//
// dakka_startmode_weapons (see dakka_levelstart.h) has modes that automatically
//  sets your weapon loadout based off of it. See the corresponding files for
//  details (or the wiki eventually). The stuff for it is stored here.
//
// PKP_ClassWeapons is also used by Dakka_TranslateGuns. (dakka_translateguns.h)

#define CLASSWEAPONS    13

// We can omit empty weapon slots due to commonFuncs.h holding a string
//  explicitly to fill string slot 0

// First col is Doomguy
// Second col is Dakkaguy
int PKP_ClassWeapons[CLASSWEAPONS][CLASSCOUNT] =
{
    {"Fist",            "DWep_Fist"},
    {"Chainsaw",        ""}, // Scrappers excluded - see dakka_scrapperstart
    {"Pistol",          "DWep_Pistol"},
    {"",                "DWep_Pistols"},
    {"Shotgun",         "DWep_Shotgun"},
    {"SuperShotgun",    "DWep_SuperShotgun"},
    {"Chaingun",        "DWep_Chaingun"},
    {"",                "DWep_Minigun"},
    {"RocketLauncher",  "DWep_RocketLauncher"},
    {"PlasmaRifle",     "DWep_PlasmaRifle"},
    {"",                "DWep_Channeler"},
    {"BFG9000",         "DWep_BFG9000"},
    {"",                "DWep_Impaler"},
};

// This is used to discriminate which weapons we should give. It doesn't
//  correspond with weapon slot because that doesn't necessarily indicate
//  the power of a weapon.
int PKP_ClassWeaponPowers[CLASSWEAPONS][CLASSCOUNT] =
{
    { 1,  1}, // fist
    { 1, -1}, // chainsaw
    { 1,  1}, // pistol
    {-1,  1}, // pistols
    { 3,  3}, // shotgun
    { 5,  5}, // SSG
    { 4,  4}, // chaingun
    {-1,  6}, // minigun
    { 5,  5}, // rocket launcher
    { 6,  6}, // plasma rifle
    {-1,  7}, // channeler
    { 7,  7}, // BFG
    {-1,  7}, // impaler
};

