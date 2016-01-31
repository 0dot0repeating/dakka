#define TMPITEM_COUNT       14

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
};



function void Dakka_UpdateTemporaryItems(void)
{
    TempChecks[TMP_LEFTFIRE]        = keyDown(BT_ATTACK);
    TempChecks[TMP_RIGHTFIRE]       = keyDown(BT_ALTATTACK);
    TempChecks[TMP_LEFTCLICK]       = keyPressed(BT_ATTACK);
    TempChecks[TMP_RIGHTCLICK]      = keyPressed(BT_ALTATTACK);

    int classNum = Pickup_ClassNumber(0);

    TempChecks[TMP_INFINITEAMMO]    = GetCVar("sv_infiniteammo") || CheckInventory("PowerInfiniteAmmo");
    TempChecks[TMP_CLASSITEM]       = classNum == Cl_Dakkaguy;

    int health = GetActorProperty(0, APROP_Health);

    TempChecks[TMP_ABNORMALHEALTH]  = middle(50, health, 100) != health;
    TempChecks[TMP_HEALTH300]       = health > 200;
    TempChecks[TMP_HEALTH200]       = middle(101, health, 200) == health;
    TempChecks[TMP_HEALTH50]        = middle( 25, health,  49) == health;
    TempChecks[TMP_HEALTH25]        = middle(  1, health,  24) == health;
    TempChecks[TMP_HEALTH0]         = health < 0;
    
    int gotCards  = CheckInventory("BlueCard")  || CheckInventory("YellowCard")  || CheckInventory("RedCard");
    int gotSkulls = CheckInventory("BlueSkull") || CheckInventory("YellowSkull") || CheckInventory("RedSkull");

    TempChecks[TMP_DOOMKEYS]        = gotCards || gotSkulls;
    TempChecks[TMP_CARDKEYS]        = gotCards;

    int i;

    for (i = 0; i < TMPITEM_COUNT; i++)
    {
        int item        = TempItems[i];
        int getItem     = TempChecks[i];
        int haveItem    = CheckInventory(item);

        if ( getItem && !haveItem) { GiveInventory(item, 1); }
        if (!getItem &&  haveItem) { TakeInventory(item, 0x7FFFFFFF); }
    }
}
