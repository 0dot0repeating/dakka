#define TMPITEM_COUNT       8

#define TMP_LEFTFIRE        0
#define TMP_RIGHTFIRE       1
#define TMP_LEFTCLICK       2
#define TMP_RIGHTCLICK      3
#define TMP_SSGAGAIN        4
#define TMP_RELOADSSG       5
#define TMP_INFINITEAMMO    6
#define TMP_CLASSITEM       7

int TempChecks[TMPITEM_COUNT];

int TempItems[TMPITEM_COUNT] =
{
    "MainFired",
    "AltFired",
    "MainClicked",
    "AltClicked",
    "FireSSGAgain",
    "ReloadSSG",
    "DakkaInfiniteAmmo",
    "HUD_IsDakkaguy",
};


function void Dakka_UpdateTemporaryItems(void)
{
    TempChecks[TMP_LEFTFIRE]        = keyDown(BT_ATTACK);
    TempChecks[TMP_RIGHTFIRE]       = keyDown(BT_ALTATTACK);
    TempChecks[TMP_LEFTCLICK]       = keyPressed(BT_ATTACK);
    TempChecks[TMP_RIGHTCLICK]      = keyPressed(BT_ALTATTACK);

    TempChecks[TMP_SSGAGAIN]        = false;
    TempChecks[TMP_RELOADSSG]       = false;

    TempChecks[TMP_INFINITEAMMO]    = GetCVar("sv_infiniteammo") || CheckInventory("PowerInfiniteAmmo");
    TempChecks[TMP_CLASSITEM]       = Pickup_ClassNumber(0) == Cl_Dakkaguy;

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
