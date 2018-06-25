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

int PKP_DefaultAmmoCount[AMMOCOUNT][2] =
{
    // DAKKA (10)
    {60,  120},            // scrap
    {70,  210},            // .50AE
    {16,   60},            // shells
    {8,    18},            // grenades
    {160, 400},            // 5mm
    {15,   24},            // flamer
    {20,   80},            // rockets
    {50,  200},            // cells

    // DOOM (4)
    {100, 300},            // bullets
    {20,  60},             // shells
    {10,  30},             // rockets
    {80,  240},            // cells

    // HERETIC (6)
    {100, 300},            // wand
    {100, 300},            // blaster
    {20,  60},             // crossbow
    {10,  30},             // phoenix
    {100, 250},            // hellstaff
    {50,  100},            // firemace

    // HEXEN (2)
    {100, 200},            // mana 1
    {50,  100},            // mana 2

    // STRIFE (7)
    //  (this will probably *never* come into play, since strife's one big hub)
    {100, 300},            // bullets
    {30,  60},             // electric
    {5,   15},             // poison
    {20,  75},             // missiles
    {6,   24},             // HE
    {2,   8},              // phosphorus
    {120, 320},            // energy
};


// Yeah, that's... basically it.
