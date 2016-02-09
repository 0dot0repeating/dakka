#define TMPITEM_COUNT       26

#define TMP_LEFTFIRE        0
#define TMP_RIGHTFIRE       1
#define TMP_LEFTCLICK       2
#define TMP_RIGHTCLICK      3
#define TMP_INFINITEAMMO    4
#define TMP_CLASSITEM       5

#define TMP_ABNORMALHEALTH  6
#define TMP_HEALTH300       7
#define TMP_HEALTH200       8
#define TMP_HEALTH50        9
#define TMP_HEALTH25        10
#define TMP_HEALTH0         11

#define TMP_DOOMKEYS        12
#define TMP_CARDKEYS        13

#define TMP_AMMO1_ABNORMAL  14
#define TMP_AMMO1_25        15
#define TMP_AMMO1_0         16

#define TMP_AMMO2_ABNORMAL  17
#define TMP_AMMO2_25        18
#define TMP_AMMO2_0         19

#define TMP_KNOWNARMOR      20
#define TMP_ARMOR100        21
#define TMP_ARMOR75         22
#define TMP_ARMOR50         23
#define TMP_ARMOR25         24

#define TMP_GOTBACKPACK     25

int TempChecks[TMPITEM_COUNT];

int TempItems[TMPITEM_COUNT] =
{
    "MainFired",
    "AltFired",
    "MainClicked",
    "AltClicked",
    "DakkaInfiniteAmmo",
    "HUD_IsDakkaguy",
    "AbnormalHealth",
    "HealthOver200",
    "HealthOver100",
    "HealthUnder50",
    "HealthUnder25",
    "HealthUnder0",
    "HUD_AnyDoomKeys",
    "HUD_AnyCardKeys",
    "AbnormalAmmo1",
    "Ammo1Under25",
    "Ammo1Empty",
    "AbnormalAmmo2",
    "Ammo2Under25",
    "Ammo2Empty",
    "HUD_KnownArmor",
    "HUD_Armor100",
    "HUD_Armor75",
    "HUD_Armor50",
    "HUD_Armor25",
    "GotBackpackItem",
};


// Keep this in sync with armor items in pickup/pickup_items_armor.h.
int TempArmorItems[ARMORCOUNT] =
{
    "HUD_GreenArmor",
    "HUD_GreenArmor",
    "HUD_BlueArmor",
    "HUD_BlueArmor",
    "HUD_SilverArmor",
    "HUD_GoldArmor",
    "HUD_TanArmor",
    "HUD_SilverArmor",
    "HUD_RedArmor",
};



function void Dakka_UpdateTemporaryItems(void)
{
    int i;

    TempChecks[TMP_LEFTFIRE]        = keyDown(BT_ATTACK);
    TempChecks[TMP_RIGHTFIRE]       = keyDown(BT_ALTATTACK);
    TempChecks[TMP_LEFTCLICK]       = keyPressed(BT_ATTACK);
    TempChecks[TMP_RIGHTCLICK]      = keyPressed(BT_ALTATTACK);

    int classNum = Pickup_ClassNumber(0);

    TempChecks[TMP_INFINITEAMMO]    = GetCVar("sv_infiniteammo") || CheckInventory("PowerInfiniteAmmo");
    TempChecks[TMP_CLASSITEM]       = classNum == Cl_Dakkaguy;
    


    int gotCards  = CheckInventory("BlueCard")  || CheckInventory("YellowCard")  || CheckInventory("RedCard");
    int gotSkulls = CheckInventory("BlueSkull") || CheckInventory("YellowSkull") || CheckInventory("RedSkull");

    TempChecks[TMP_DOOMKEYS]        = gotCards || gotSkulls;
    TempChecks[TMP_CARDKEYS]        = gotCards;



    int health = GetActorProperty(0, APROP_Health);

    TempChecks[TMP_ABNORMALHEALTH]  = middle(50, health, 100) != health;
    TempChecks[TMP_HEALTH300]       = health > 200;
    TempChecks[TMP_HEALTH200]       = middle(101, health, 200) == health;
    TempChecks[TMP_HEALTH50]        = middle( 25, health,  49) == health;
    TempChecks[TMP_HEALTH25]        = middle(  1, health,  24) == health;
    TempChecks[TMP_HEALTH0]         = health < 0;



    // pickup/pickup_pickup_armor.h
    int armorIndex  = Armor_CurrentArmorIndex();

    // I'd just use armorCount here, but ACS is case-insensitive...
    //  fucking hate that.
    int armorInv    = CheckInventory("BasicArmor");

    if (armorIndex == -1)
    {
        TempChecks[TMP_KNOWNARMOR]  = false;

        // ARMOR100/75/50/25 isn't used if we have an unknown armor.
        //
        // I'm lazy.

        for (i = 0; i < ARMORCOUNT; i++)
        {
            SetInventory(TempArmorItems[i], 0);
        }
    }
    else
    {
        TempChecks[TMP_KNOWNARMOR]  = true;

        int armorHUDItem = TempArmorItems[armorIndex];

        for (i = 0; i < ARMORCOUNT; i++)
        {
            int curItem = TempArmorItems[i];
            SetInventory(curItem, strcmp_i(curItem, armorHUDItem) == 0);
        }

        int armorMax      = PKP_ArmorData[armorIndex][ARM_MAXPOINTS];
        int armorQuadrant = (armorInv * 4) / armorMax;

        TempChecks[TMP_ARMOR100] = armorQuadrant >= 3;
        TempChecks[TMP_ARMOR75]  = armorQuadrant == 2;
        TempChecks[TMP_ARMOR50]  = armorQuadrant == 1;
        TempChecks[TMP_ARMOR25]  = armorQuadrant <= 0;
    }



    // pickup/pickup_pickup_weapons.h
    int wepIndex    = Weapon_CurrentWeaponIndex();
    int ammo1Name   = PKP_KnownGuns[wepIndex][WEP_AMMO1];
    int ammo2Name   = PKP_KnownGuns[wepIndex][WEP_AMMO2];

    int hasAmmo1;
    int hasAmmo2;

    // ACS does not have operator short-circuiting.
    // It really fucking needs operator short-circuiting.
    if (wepIndex == -1)
    {
        hasAmmo1 = false;
        hasAmmo2 = false;
    }
    else
    {
        hasAmmo1 = StrLen(ammo1Name) != 0;
        hasAmmo2 = StrLen(ammo2Name) != 0;
    }

    if (!hasAmmo1)
    {
        TempChecks[TMP_AMMO1_ABNORMAL]  = false;
        TempChecks[TMP_AMMO1_25]        = false;
        TempChecks[TMP_AMMO1_0]         = false;
    }
    else
    {
        int ammo1Cur = CheckInventory(ammo1Name);
        int ammo1Max = GetAmmoCapacity(ammo1Name);
        int ammo1Percent = itof(ammo1Cur) / ammo1Max;

        // note: 1 is actually 1.0 / 65536, do not get confused
        TempChecks[TMP_AMMO1_ABNORMAL]  = ammo1Percent <= 0.25;
        TempChecks[TMP_AMMO1_25]        = middle(1, ammo1Percent, 0.25) == ammo1Percent;
        TempChecks[TMP_AMMO1_0]         = ammo1Percent <= 0;
    }

    if (!hasAmmo2)
    {
        TempChecks[TMP_AMMO2_ABNORMAL]  = false;
        TempChecks[TMP_AMMO2_25]        = false;
        TempChecks[TMP_AMMO2_0]         = false;
    }
    else
    {
        int ammo2Cur = CheckInventory(ammo2Name);
        int ammo2Max = GetAmmoCapacity(ammo2Name);
        int ammo2Percent = itof(ammo2Cur) / ammo2Max;

        // note: 1 is actually 1.0 / 65536, do not get confused
        TempChecks[TMP_AMMO2_ABNORMAL]  = ammo2Percent <= 0.25;
        TempChecks[TMP_AMMO2_25]        = middle(1, ammo2Percent, 0.25) == ammo2Percent;
        TempChecks[TMP_AMMO2_0]         = ammo2Percent <= 0;
    }


    // Backpack check - backpack items in dakka_const.h

    int hasBackpack = false;

    for (i = 0; i < BACKPACKCOUNT; i++)
    {
        if (CheckInventory(Dakka_BackpackItems[i]))
        {
            hasBackpack = true;
            break;
        }
    }

    TempChecks[TMP_GOTBACKPACK] = hasBackpack;


    for (i = 0; i < TMPITEM_COUNT; i++)
    {
        int item        = TempItems[i];
        int getItem     = TempChecks[i];

        SetInventory(item, getItem);
    }
}
