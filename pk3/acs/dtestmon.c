#library "dtestmon"
#include "zcommon.acs"

#define MONSPAWN_COUNT 19

int CommonFuncs_ZeroString = "[commonFuncs.h: string index #0]";

function int stringBlank(int string)
{
    if (string == CommonFuncs_ZeroString) { return true; }

    int safeString = StrParam(s:string);
    return StrLen(safeString) == 0;
}

str MonSpawnerSlots[MONSPAWN_COUNT][2] =
{
    {"Zombieman",        "NotAZombieman"},
    {"ShotgunGuy",       "NotAShotgunner"},
    {"ChaingunGuy",      "NotAChaingunner"},
    {"DoomImp",          "NotAnImp"},
    {"Demon",            "NotADemon"},
    {"Spectre",          "NotASpectre"},
    {"Cacodemon",        "NotATomato"},
    {"Revenant",         "NotABonehead"},
    {"LostSoul",         "NotASkull"},
    {"PainElemental",    "NotAMeatball"},
    {"HellKnight",       "NotAHellKnight"},
    {"BaronOfHell",      "NotABaron"},
    {"Fatso",            "NotAMancubus"},
    {"Arachnotron",      "NotAnArachnotron"},
    {"Archvile",         "NotAnAlien"},
    {"SpiderMastermind", "NotASpiderdemon"},
    {"Cyberdemon",       "NotACyberdemon"},
    {"WolfensteinSS",    "NotANazi"},
    {"CommanderKeen",    "NotCommanderKeen"},
};

// Used in the monster spawner; externalized so that compat patches can have
//  their monsters tested in DAKKTEST

script "DAKKTEST_GetMonster" (int index, int spawnOrShow)
{
    if (index >= MONSPAWN_COUNT || index < 0)
    {
        SetResultValue("");
        terminate;
    }

    if (spawnOrShow && !stringBlank(MonSpawnerSlots[index][1]))
    {
        SetResultValue(MonSpawnerSlots[index][1]);
    }
    else
    {
        SetResultValue(MonSpawnerSlots[index][0]);
    }
}

script "DAKKTEST_MonsterCount" (void)
{
    SetResultValue(MONSPAWN_COUNT);
}