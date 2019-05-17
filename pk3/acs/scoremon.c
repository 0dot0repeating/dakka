#library "scoremon"
#include "zcommon/zcommon.h"

#define MONTYPES 19

str Monster_KnownMons[MONTYPES] =
{
    "Zombieman",
    "ShotgunGuy",
    "ChaingunGuy",
    "DoomImp",
    "Demon",
    "Spectre",
    "Cacodemon",
    "LostSoul",
    "PainElemental",
    "Revenant",
    "Arachnotron",
    "Fatso",
    "HellKnight",
    "BaronOfHell",
    "ArchVile",
    "Cyberdemon",
    "SpiderMastermind",
    "WolfensteinSS",
    "CommanderKeen",
};

int Monster_Points[MONTYPES] =
{
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};


script "Score_MonsterTypeCount" (void)
{
    SetResultValue(MONTYPES);
}

script "Score_MonsterAtIndex" (int i)
{
    if (i < 0 || i >= MONTYPES)
    {
        SetResultValue("");
        terminate;
    }

    SetResultValue(Monster_KnownMons[i]);
}

script "Score_RewardAtIndex" (int i)
{
    if (i < 0 || i >= MONTYPES)
    {
        SetResultValue(-1);
        terminate;
    }

    SetResultValue(Monster_Points[i]);
}

// name is a str but ACC is retarded
script "Score_Lookup" (int name)
{
    for (int i = 0; i < MONTYPES; i++)
    {
        if (!Stricmp(name, Monster_KnownMons[i]))
        {
            SetResultValue(Monster_Points[i]);
            terminate;
        }
    }

    SetResultValue(-1);
}