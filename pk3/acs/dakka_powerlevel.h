#define POWERLEVEL_COUNT    21
#define POWERLEVEL_CENTER   10

int DakkaPowerLevels[POWERLEVEL_COUNT] =
{
    "DakkaPowerLevel_-10",
    "DakkaPowerLevel_-9",
    "DakkaPowerLevel_-8",
    "DakkaPowerLevel_-7",
    "DakkaPowerLevel_-6",
    "DakkaPowerLevel_-5",
    "DakkaPowerLevel_-4",
    "DakkaPowerLevel_-3",
    "DakkaPowerLevel_-2",
    "DakkaPowerLevel_-1",
    "THIS ISN'T A POWER LEVEL TELL IJON HE FUCKED UP",
    "DakkaPowerLevel_1",
    "DakkaPowerLevel_2",
    "DakkaPowerLevel_3",
    "DakkaPowerLevel_4",
    "DakkaPowerLevel_5",
    "DakkaPowerLevel_6",
    "DakkaPowerLevel_7",
    "DakkaPowerLevel_8",
    "DakkaPowerLevel_9",
    "DakkaPowerLevel_10",
};

function void Dakka_PowerLevel(void)
{
    int powerLevel = middle(0, GetCVar("dakka_powerlevel") + POWERLEVEL_CENTER, POWERLEVEL_COUNT-1);
    
    for (int i = 0; i < POWERLEVEL_COUNT; i++)
    {
        if (i == POWERLEVEL_CENTER) { continue; }
        SetInventory(DakkaPowerLevels[i], i == powerLevel);
    }
}