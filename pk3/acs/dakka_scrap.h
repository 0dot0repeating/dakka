// Defines how much of a GiveScrapCounter item is needed to give 1 scrap.
#define SCRAPTYPES 3

int Scrap_Items[SCRAPTYPES] =
{
    "GiveScrapCounter",
    "GiveScrapCounter2",    // For cells
    "GiveScrapCounter3",    // For flamer fuel
};

#define SCRAP_NEEDED    0
#define SCRAP_GIVE      1
int Scrap_Batches[SCRAPTYPES][2] =
{
    {10,  1},
    {200, 20},
    {160, 16},
};

function void Dakka_ProcessScrap(void)
{
    int i, item;
    int scrapFactor = middle(0, GetCVar("dakka_scrapfactor"), 1000);

    if (scrapFactor == 0)
    {
        for (i = 0; i < SCRAPTYPES; i++)
        {
            item = Scrap_Items[i];
            if (CheckInventory(item)) { TakeInventory(item, 0x7FFFFFFF); }
        }

        return;
    }

    // You'd think FixedDiv(100.0, scrapFactor) would be equivalent.
    //  It ain't.
    int neededMult = FixedDiv(1.0, scrapFactor * 0.01);
    int needed, toGive, itemCount, fullBatches;

    for (i = 0; i < SCRAPTYPES; i++)
    {
        item   = Scrap_Items[i];
        needed = FixedMul(Scrap_Batches[i][SCRAP_NEEDED], neededMult);
        toGive = Scrap_Batches[i][SCRAP_GIVE];

        itemCount   = CheckInventory(item);
        fullBatches = itemCount / needed;

        GiveInventory("DakkaScrap", toGive * fullBatches);
        TakeInventory(item,         needed * fullBatches);
    }
}
