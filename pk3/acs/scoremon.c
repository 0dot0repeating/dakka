#library "scoremon"
#include "zcommon.acs"
#include "dakka_commonFuncs.h"

#define MONTYPES 162

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
    
    // Colorful hell - zombiemen
    "CommonZombie",
    "GreenZombie",
    "BlueZombie",
    "PurpleZombie",
    "YellowZombie",
    "RedZombie",
    "BlackZombie1",
    "WhiteZombie1",
    
    // Colorful hell - shotgun guys
    "CommonSG",
    "GreenSG",
    "BlueSG",
    "PurpleSG",
    "FirebluSG2",
    "YellowSG",
    "RedSG",
    "BlackSG2",
    "WhiteSG2",
    
    // Colorful hell - chaingun guys
    "CommonCGuy",
    "GreenCGuy",
    "BlueCGuy",
    "PurpleCGuy",
    "GrayCGuy2",
    "YellowCGuy",
    "FirebluCGuy2",
    "RedCGuy",
    "BlackCGuy2",
    "WhiteCGuy2",
    
    // Colorful hell - imps
    "CommonImp",
    "GreenImp",
    "BlueImp",
    "PurpleImp",
    "GrayImp2",
    "YellowImp",
    "RedImp",
    "BlackImp1",
    "WhiteImp2",
    
    // Colorful hell - demons
    "CommonDemon",
    "GreenDemon",
    "BlueDemon",
    "PurpleDemon",
    "YellowDemon",
    "RedDemon",
    "BlackDemon3",
    "WhiteDemon2",
    
    // Colorful hell - spectres
    "CommonSpectre",
    "GreenSpectre",
    "BlueSpectre",
    "PurpleSpectre",
    "YellowSpectre",
    "RedSpectre",
    "BlackSpectre2",
    "WhiteSpectre2",
    
    // Colorful hell - cacodemons
    "CommonCaco",
    "GreenCaco",
    "BlueCaco",
    "PurpleCaco",
    "YellowCaco",
    "FirebluCaco2",
    "RedCaco",
    "BlackCaco2",
    "WhiteCaco2",
    
    // Colorful hell - lost souls
    "CommonLSoul",
    "GreenLSoul",
    "BlueLSoul",
    "FirebluLSoul2",
    "PurpleLSoul",
    "YellowLSoul",
    "RedLSoul",
    "BlackLSoul3",
    "WhiteLSoul2",
    
    // Colorful hell - pain elementals
    "CommonPE",
    "GreenPE",
    "BluePE",
    "PurplePE",
    "YellowPE",
    "RedPE",
    "BlackPE2",
    
    // Colorful hell - skeletons
    "CommonRevenant",
    "GreenRevenant",
    "BlueRevenant",
    "GrayRevenant2",
    "PurpleRevenant",
    "FirebluRevenant2",
    "YellowRevenant",
    "RedRevenant",
    "BlackRevenant3",
    
    // Colorful hell - arachnotrons
    "CommonSP1",
    "GreenSP1",
    "BlueSP1",
    "PurpleSP1",
    "YellowSP1",
    "RedSP1",
    "BlackSP2",
    "WhiteSP11",
    
    // Colorful hell - mancubi
    "CommonFatso",
    "GreenFatso",
    "BlueFatso",
    "PurpleFatso",
    "YellowFatso",
    "RedFatso",
    "BlackFatso2",
    
    // Colorful hell - hell knights
    "CommonHK",
    "GreenHK",
    "BlueHK",
    "PurpleHK",
    "GrayHK2",
    "YellowHK",
    "FirebluHK2",
    "RedHK",
    "BlackHK2",
    "WhiteHK3",
    
    // Colorful hell - barons of hell
    "CommonBaron",
    "GreenBaron",
    "BlueBaron",
    "PurpleBaron",
    "YellowBaron",
    "FirebluBaron2",
    "RedBaron",
    "BlackBaron2",
    
    // Colorful hell - archviles
    "CommonArch",
    "GreenArch",
    "BlueArch",
    "PurpleArch",
    "FirebluArch2",
    "YellowArch",
    "RedArch",
    "BlackVile",
    "WhiteVile",
    
    // Colorful hell - cyberdemons
    "CommonCybie",
    "GreenCybie",
    "BlueCybie",
    "PurpleCybie",
    "FirebluCybie2",
    "YellowCybie",
    "RedCybie",
    "BlackCybie2",
    
    // Colorful hell - spiderdemons
    "CommonMind",
    "GreenMind",
    "BlueMind",
    "PurpleMind",
    "YellowMind",
    "RedMind",
    "BlackMind2",
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
    
    // Colorful hell - zombiemen
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - shotgun guys
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - chaingun guys
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
    
    // Colorful hell - imps
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - demons
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - spectres
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - cacodemons
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - lost souls
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - pain elementals
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - skeletons
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - arachnotrons
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - mancubi
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - hell knights
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
    
    // Colorful hell - barons of hell
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - archviles
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - cyberdemons
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    
    // Colorful hell - spiderdemons
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