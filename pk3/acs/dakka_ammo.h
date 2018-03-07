#define SCRAPTYPES 4

int Scrap_Items[SCRAPTYPES] =
{
    "ScrapCounter",
    "ScrapCounter_Minigun",
    "ScrapCounter_Flamer",
    "ScrapCounter_Cells",
};

#define SCRAP_NEEDED    0
#define SCRAP_GIVE      1

int Scrap_Batches[SCRAPTYPES][2] =
{
    { 100,  1}, // regular
    { 200,  1}, // minigun
    { 400, 16}, // flamer
    {2000, 20}, // cells
};

script "Dakka_GiveScrap" (int amount, int type)
{
    if (amount <= 0) { terminate; }
    if (type < 0 || type >= SCRAPTYPES) { terminate; }
    
    str scrapType  = Scrap_Items[type];
    int batchSize  = Scrap_Batches[type][SCRAP_NEEDED];
    
    int giveAmount = amount * middle(0, GetCVar("dakka_scrapfactor"), 1000);
    int newAmount  = CheckInventory(scrapType) + giveAmount;
    int batches    = newAmount / batchSize;
    
    GiveAmmo("DakkaScrap", batches * Scrap_Batches[type][SCRAP_GIVE]);
    SetInventory(scrapType, newAmount - (batches * batchSize));
}



// This is mainly to avoid ammo desyncs online, but also to make sure no scrap is
// given when you have infinite ammo.
//
// Zandronum clients predict when they're going to use ammo, and take it away
// themselves. The server therefore never bothers to tell them they used ammo,
// which usually works out... but with Dakka, it doesn't. At all. So we gotta
// handle this manually.

script "Dakka_UseAmmo" (int ammoindex, int count, int scrapgive, int scraptype)
{
    if (!IsServer) { terminate; }
    if (ammoindex < 0 || ammoindex >= AMMOCOUNT) { terminate; }
    if (HasInfiniteAmmo()) { terminate; }
    
    if (ammoindex == AMMO_PISTOL && GetCVar("dakka_pickedupaclip") == 3) { terminate; }

    ACS_NamedExecuteWithResult("Dakka_GiveScrap", scrapgive, scraptype);
    TakeInventory(PKP_KnownAmmo[ammoindex], count);
}