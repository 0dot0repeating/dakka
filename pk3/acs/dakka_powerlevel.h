#define POWERLEVEL_COUNT    23
#define POWERLEVEL_CENTER   11

int DakkaPowerLevels[POWERLEVEL_COUNT] =
{
    "DakkaPowerLevel_-11",
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
    "DakkaPowerLevel_11",
};

int DakkaPowerMults[POWERLEVEL_COUNT] =
{
    0.1,
    0.2,
    0.25,
    0.3333,
    0.4,
    0.5,
    0.6,
    0.6667,
    0.75,
    0.8,
    0.909,
    1.0,
    1.1,
    1.25,
    1.3333,
    1.5,
    1.6667,
    1.75,
    2.0,
    2.5,
    3.0,
    4.0,
    5.0,
};

#define DEFENSELEVEL_COUNT  23
#define DEFENSELEVEL_CENTER 12

int DakkaDefenseLevels[POWERLEVEL_COUNT] =
{
    "DakkaDefenseLevel_-12",
    "DakkaDefenseLevel_-11",
    "DakkaDefenseLevel_-10",
    "DakkaDefenseLevel_-9",
    "DakkaDefenseLevel_-8",
    "DakkaDefenseLevel_-7",
    "DakkaDefenseLevel_-6",
    "DakkaDefenseLevel_-5",
    "DakkaDefenseLevel_-4",
    "DakkaDefenseLevel_-3",
    "DakkaDefenseLevel_-2",
    "DakkaDefenseLevel_-1",
    "THIS ISN'T A DEFENSE LEVEL TELL IJON HE FUCKED UP",
    "DakkaDefenseLevel_1",
    "DakkaDefenseLevel_2",
    "DakkaDefenseLevel_3",
    "DakkaDefenseLevel_4",
    "DakkaDefenseLevel_5",
    "DakkaDefenseLevel_6",
    "DakkaDefenseLevel_7",
    "DakkaDefenseLevel_8",
    "DakkaDefenseLevel_9",
    "DakkaDefenseLevel_10",
};

function void Dakka_PowerLevel(void)
{
    int powerLevel   = middle(0, GetCVar("dakka_powerlevel")   + POWERLEVEL_CENTER,   POWERLEVEL_COUNT  -1);
    int defenseLevel = middle(0, GetCVar("dakka_defenselevel") + DEFENSELEVEL_CENTER, DEFENSELEVEL_COUNT-1);

    int i;

    for (i = 0; i < POWERLEVEL_COUNT; i++)
    {
        if (i == POWERLEVEL_CENTER) { continue; }
        SetInventory(DakkaPowerLevels[i], i == powerLevel);
    }

    for (i = 0; i < DEFENSELEVEL_COUNT; i++)
    {
        if (i == DEFENSELEVEL_CENTER) { continue; }
        SetInventory(DakkaDefenseLevels[i], i == defenseLevel);
    }
}