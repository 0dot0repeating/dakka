#define WEAPONCOUNT    60
#define REPLCOUNT   3

#define WEP_NAME    0
#define WEP_AMMO1   1
#define WEP_AMMO2   2


// This should be fairly self-explanatory.

int PKP_KnownGuns[WEAPONCOUNT][3] =
{
//  Name                        Ammo 1                      Ammo 2
//
    // DAKKA (12 weapons)
    {"DWep_Fists",              "",                         ""},
    {"DWep_Scrappers",          "DakkaScrap",               ""},
    {"DWep_Pistol",             "DakkaBullet_10mm",         ""},
    {"DWep_Pistols",            "DakkaBullet_10mm",         ""},
    {"DWep_Shotgun",            "DakkaShells",              "DakkaShotGrenades"},
    {"DWep_SuperShotgun",       "DakkaShells",              "DakkaGyroShells"},
    {"DWep_Chaingun",           "DakkaBullet_5mm",          "DakkaRainGrenades"},
    {"DWep_Minigun",            "DakkaBullet_5mm",          "DakkaFlamerFuel"},
    {"DWep_RocketLauncher",     "DakkaRockets",             ""},
    {"DWep_PlasmaRifle",        "DakkaCells",               ""},
    {"DWep_Channeler",          "DakkaCells",               ""},
    {"DWep_BFG9000",            "DakkaCells",               ""},


    // DOOM (9 weapons)
    {"Fist",                    "",                         ""},
    {"Chainsaw",                "",                         ""},
    {"Pistol",                  "Clip",                     ""},
    {"Shotgun",                 "Shell",                    ""},
    {"SuperShotgun",            "Shell",                    ""},
    {"Chaingun",                "Clip",                     ""},
    {"RocketLauncher",          "RocketAmmo",               ""},
    {"PlasmaRifle",             "Cell",                     ""},
    {"BFG9000",                 "Cell",                     ""},

    // CHEX QUEST (9 weapons)
    {"Bootspoon",               "",                         ""},
    {"SuperBootspork",          "",                         ""},
    {"MiniZorcher",             "Clip",                     ""},
    {"LargeZorcher",            "Shell",                    ""},
    {"SuperLargeZorcher",       "Shell",                    ""},
    {"RapidZorcher",            "Clip",                     ""},
    {"ZorchPropulsor",          "RocketAmmo",               ""},
    {"PhasingZorcher",          "Cell",                     ""},
    {"LAZDevice",               "Cell",                     ""},

    // HERETIC (8 weapons)
    {"Staff",                   "",                         ""},
    {"Gauntlets",               "",                         ""},
    {"GoldWand",                "GoldWandAmmo",             ""},
    {"Crossbow",                "CrossbowAmmo",             ""},
    {"Blaster",                 "BlasterAmmo",              ""},
    {"PhoenixRod",              "PhoenixRodAmmo",           ""},
    {"SkullRod",                "SkullRodAmmo",             ""},
    {"Mace",                    "MaceAmmo",                 ""},

    // HEXEN (16 weapons)
    {"FWeapFist",               "",                         ""},
    {"CWeapMace",               "",                         ""},
    {"MWeapWand",               "",                         ""},
    {"FWeapAxe",                "Mana1",                    ""},
    {"CWeapStaff",              "Mana1",                    ""},
    {"MWeapFrost",              "Mana1",                    ""},
    {"FWeapHammer",             "Mana2",                    ""},
    {"CWeapFlame",              "Mana2",                    ""},
    {"MWeapLightning",          "Mana2",                    ""},
    {"FWeapQuietus",            "Mana1",                    "Mana2"},
    {"CWeapWraithverge",        "Mana1",                    "Mana2"},
    {"MWeapBloodscourge",       "Mana1",                    "Mana2"},

    // STRIFE (10 weapons)
    {"PunchDagger",             "",                         ""},
    {"StrifeCrossbow",          "ElectricBolts",            ""},
    {"StrifeCrossbow2",         "PoisonBolts",              ""},
    {"AssaultGun",              "ClipOfBullets",            ""},
    {"MiniMissileLauncher",     "MiniMissiles",             ""},
    {"StrifeGrenadeLauncher",   "HEGrenadeRounds",          ""},
    {"StrifeGrenadeLauncher2",  "PhosphorusGrenadeRounds",  ""},
    {"Flamethrower",            "EnergyPod",                ""},
    {"Mauler",                  "EnergyPod",                ""},
    {"Mauler2",                 "EnergyPod",                ""},
};



// Copy whatever the Weapon.SelectionOrder for the weapon is into here.

int PKP_WeaponPriorities[WEAPONCOUNT] =
{
    3700,   // DWep_Fists
    1100,   // DWep_Scrappers
    1900,   // DWep_Pistol
    1500,   // DWep_Pistols
    1300,   // DWep_Shotgun
    400,    // DWep_SuperShotgun
    700,    // DWep_Chaingun
    350,    // DWep_Minigun
    300,    // DWep_RocketLauncher
    100,    // DWep_PlasmaRifle
    100,    // DWep_Channeler
    1000,   // DWep_BFG9000

    3700,   // Fist
    2200,   // Chainsaw
    1900,   // Pistol
    1300,   // Shotgun
    400,    // SuperShotgun
    700,    // Chaingun
    2500,   // RocketLauncher
    100,    // PlasmaRifle
    2800,   // BFG9000

    3700,   // Bootspoon
    2200,   // SuperBootspork
    1900,   // MiniZorcher
    1300,   // LargeZorcher
    400,    // SuperLargeZorcher
    700,    // RapidZorcher
    2500,   // ZorchPropulsor
    100,    // PhasingZorcher
    2800,   // LAZDevice

    3800,   // Staff
    2300,   // Gauntlets
    2000,   // GoldWand
    800,    // Crossbow
    500,    // Blaster
    2600,   // PhoenixRod
    200,    // SkullRod
    1400,   // Mace

    3400,   // FWeapFist
    3500,   // CWeapMace
    3600,   // MWeapWand
    1500,   // FWeapAxe
    1600,   // CWeapStaff
    1700,   // MWeapFrost
    900,    // FWeapHammer
    1000,   // CWeapFlame
    1100,   // MWeapLightning
    2900,   // FWeapQuietus
    3000,   // CWeapWraithverge
    3100,   // MWeapBloodscourge

    3900,   // PunchDagger
    1200,   // StrifeCrossbow
    2700,   // StrifeCrossbow2
    600,    // AssaultGun
    1800,   // MiniMissileLauncher
    2400,   // StrifeGrenadeLauncher
    3200,   // StrifeGrenadeLauncher2
    2100,   // Flamethrower
    300,    // Mauler
    3300,   // Mauler2
};

