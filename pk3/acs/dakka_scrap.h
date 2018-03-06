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