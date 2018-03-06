#define AMMOCOUNT   28

#define AMMO_SCRAP      0
#define AMMO_PISTOL     1
#define AMMO_SHELL      2
#define AMMO_GRENADE    3
#define AMMO_MINIGUN    4
#define AMMO_FLAMER     5
#define AMMO_ROCKET     6
#define AMMO_CELL       7

int PKP_KnownAmmo[AMMOCOUNT] =
{
    // DAKKA (10)
    "DakkaScrap",
    "DakkaBullet_50AE",     // if this moves, update Dakka_UseAmmo script
    "DakkaShells",
    "DakkaShotGrenades",
    "DakkaBullet_5mm",
    "DakkaFlamerFuel",
    "DakkaRockets",
    "DakkaCells",

    // DOOM (4)
    "Clip",
    "Shell",
    "RocketAmmo",
    "Cell",

    // HERETIC (6)
    "GoldWandAmmo",
    "BlasterAmmo",
    "CrossbowAmmo",
    "PhoenixRodAmmo",
    "SkullRodAmmo",
    "MaceAmmo",

    // HEXEN (2)
    "Mana1",
    "Mana2",

    // STRIFE (7)
    "ClipOfBullets",
    "ElectricBolts",
    "PoisonBolts",
    "MiniMissiles",
    "HEGrenadeRounds",
    "PhosphorusGrenadeRounds",
    "EnergyPod",
};


// DAKKA EXTENSION
// If dakka_startmode_ammo is set to a mode that resets your ammo to default
//  for weapons you have, it reads the values from here to determine that
//  default ammo count.

int PKP_DefaultAmmoCount[AMMOCOUNT] =
{
    // DAKKA (10)
    60,             // scrap
    70,             // .50AE
    16,             // shells
    6,              // grenades
    160,            // 5mm
    15,             // flamer
    20,             // rockets
    50,             // cells

    // DOOM (4)
    100,            // bullets
    20,             // shells
    10,             // rockets
    80,             // cells

    // HERETIC (6)
    100,            // wand
    100,            // blaster
    20,             // crossbow
    10,             // phoenix
    100,            // hellstaff
    50,             // firemace

    // HEXEN (2)
    100,            // mana 1
    50,             // mana 2

    // STRIFE (7)
    //  (this will probably *never* come into play, since strife's one big hub)
    100,            // bullets
    20,             // electric
    0,              // poison
    20,             // missiles
    6,              // HE
    2,              // phosphorus
    80,             // energy
};


// Yeah, that's... basically it.
