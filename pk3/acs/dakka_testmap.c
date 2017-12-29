#library "dakka_testmap"
#include "zcommon.acs"
#include "dakka_commonFuncs.h"

// superceded in DAKKTEST itself, but here to avoid unknown script errors
script "DAKKTEST_ReportDamage" (void)
{
    int hp    = GetActorProperty(0, APROP_Health);
    int maxhp = GetActorProperty(0, APROP_SpawnHealth);
    
    if (hp < maxhp)
    {
        Log(s:"\cd[", d:Timer(), s:"]\c- Dealt ", d:maxhp - hp, s:" damage to target"); 
    }
    SetActorProperty(0, APROP_Health, maxhp);
}

// do nothing, but avoid unknown script errors
script "DAKKTEST_MonitorTID_Ping" (void)
{
}


// Used in the monster spawner; externalized so that compat patches can have
//  their monsters tested in DAKKTEST

#define MONSPAWN_COUNT 19

str MonSpawnerSlots[MONSPAWN_COUNT][2] =
{
    {"Zombieman",        ""},
    {"ShotgunGuy",       ""},
    {"ChaingunGuy",      ""},
    {"DoomImp",          ""},
    {"Demon",            ""},
    {"Spectre",          ""},
    {"Cacodemon",        ""},
    {"Revenant",         ""},
    {"LostSoul",         ""},
    {"PainElemental",    ""},
    {"HellKnight",       ""},
    {"BaronOfHell",      "NotABaron"},
    {"Fatso",            "NotAMancubus"},
    {"Arachnotron",      "NotAnArachnotron"},
    {"Archvile",         ""},
    {"SpiderMastermind", "NotASpiderdemon"},
    {"Cyberdemon",       "NotACyberdemon"},
    {"WolfensteinSS",    ""},
    {"CommanderKeen",    "NotCommanderKeen"},
};

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