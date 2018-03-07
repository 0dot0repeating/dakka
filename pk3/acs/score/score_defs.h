#define SCOREWEPS           9
#define SCOREFIREMODES      3

#define S_FIST              0
#define S_SCRAPPER          1
#define S_PISTOL            2
#define S_SHOTGUN           3
#define S_SUPERSHOTGUN      4
#define S_MINIGUN           5
#define S_IMPALER           6
#define S_LANCER            7
#define S_BFG9000           8

#define SWITCHAROO_TICSPER100HP 24
#define SWITCHAROO_MINTICS      72
#define SWITCHAROO_MAXTICS      144

#define KILLSTREAK_TICSPER100HP 24
#define KILLSTREAK_MINTICS      72
#define KILLSTREAK_MAXTICS      360
#define KILLSTREAK_MULTMAX      0.25
#define KILLSTREAK_MULTINC      0.025
#define KS_COUNT        0
#define KS_TIME         1
#define KS_TIMEADD      2

#define KS_KILLS_SINCELASTHIT 3
#define KS_HP_SINCELASTHIT    4
#define KS_KILLS_UNTOUCHABLE  5

#define UNTOUCHABLE_MINKILLS    24    // so it triggers on the 25th kill
#define UNTOUCHABLE_MINHEALTH   2000

// for every X hp they have, add one to the untouchable kill streak when calculating the score multiplier
// (see SMult_Untouchable in score/score_mults.h, it's hard to explain in words)
#define UNTOUCHABLE_KILLBOOST_HPDIVISOR   400

#define UNTOUCHABLE_MAXKILLS    20
#define UNTOUCHABLE_MULT        0.025

#define EFFICIENCY_MULTINC      0.1
#define EFFICIENCY_MULTMAX      0.5

#define BONEDRY_MAXHP           25
#define BONEDRY_MULTPERHP       0.02

#define AIR_MINHEIGHT           64.0
#define AIR_MULT                0.005
#define AIR_MAXMULT             2.0

#define CURVEBALL_MULT          0.2

#define SCRAPPING_MULT          0.125

#define POINTBLANK_DIST         64.0
#define POINTBLANK_MULT         0.1

#define INFIGHTER_MULT          0.2
#define DARWIN_MULT             2.0

// Allowing three fire modes.
//  0 - primary   (+attack)
//  1 - secondary (+altattack)
//  2 - tertiary  (some combo of the two, probably won't be used)
int PlayerWeapons_LastFired[PLAYERMAX][SCOREWEPS][SCOREFIREMODES];
int PlayerKillStreaks[PLAYERMAX][6];


// Air_JumpHeight stores the max height the player's current jump either has
//  reached, or is predicted to reach
int Air_JumpHeight[PLAYERMAX];

// Air_FloorHeight stores his floor height the last time he was on the ground
int Air_FloorHeight[PLAYERMAX];



// This shit's used in score_levelstart.h, to calculate the points needed for
//  a full heal.
#define P_FULLHEAL_MIN      50
#define P_FULLHEAL_MAX      250
#define P_FULLHEAL_PERCENT  0.7

// This is used to cap how much you need to be considered untouchable.
#define P_UTKILLS_MIN       20
#define P_UTKILLS_MAX       100
#define P_UTHP_MIN          2000
#define P_UTHP_MAX          10000

#define ST_LASTSET      0
#define ST_WORLDTIMER   1
#define ST_FULLHEAL     2
#define ST_UT_KILLS     3
#define ST_UT_HP        4

world int 22:Score_Thresholds[];


// CLASSCOUNT_DEFINED is defined in pickup/pickup_classes.h
//  This is used for Switcharoo and One Stone: Switcharoo checks it to see if
//  it applies at all, and One Stone uses it to see if we can rely on last
//  weapon fire time to actually be updated
//
int SCORE_UpdatesSwitcharoo[CLASSCOUNT_DEFINED] =
{
    true,
};

#define KNOWNMELEE 13

// first value is weapon name, second is damage type

int Brawler_KnownMelee[KNOWNMELEE][2] =
{
    {"Fist",        ""},    
    {"Chainsaw",    ""},
    {"DWep_Fist",   "FistRapid"},
    {"",            "FistLightStun"},
    {"",            "FistMediumStun"},
    {"",            "FistHeavyStun"},

    {"PunchDagger", ""},

    {"FWeapFist",   ""},
    {"FWeapAxe",    ""},
    {"FWeapHammer", ""},
    {"CWeapMace",   ""},

    {"Staff",       ""},
    {"Gauntlets",   ""},
};

// first value is mult, second is "only check weapon if damagetype is unknown"

int Brawler_MeleeValues[KNOWNMELEE][2] =
{
    {0.5,   false},
    {0.5,   false},
    {0.25,  true},
    {0.25,  false},
    {0.25,  false},
    {0.25,  false},
    
    {0.5,   false},
    
    {0.25,  false},
    {0.15,  false},
    {0.1,   false},
    {0.3,   false},
    
    {0.5,   false},
    {0.25,  false},
};


#define BONUSCOUNT  13

#define BS_BASE         0
#define BS_SPREE        1
#define BS_UNTOUCHABLE  2
#define BS_INFIGHTER    3
#define BS_SWITCHAROO   4
#define BS_SCRAPPING    5
#define BS_CURVEBALL    6
#define BS_DARWIN       7
#define BS_AIR          8
#define BS_BONEDRY      9
#define BS_SORELOSER    10
#define BS_POINTBLANK   11
#define BS_BRAWLER      12

// CR_BRICK  is efficiency
// CR_GREEN  is style
// CR_ORANGE is daring

int BonusNames[BONUSCOUNT] =
{
    "Base",
    "Spree",
    "Untouchable",
    "Infighter",
    "Switcharoo",
    "Scrapping",
    "Curveball",
    "Darwin",
    "Air",
    "Redline",
    "Swan Song",
    "Point-Blank",
    "Brawler",
};

int BonusColors[BONUSCOUNT] =
{
    "DScore_Base",
    "DScore_Efficiency",
    "DScore_Efficiency",
    "DScore_Efficiency",
    "DScore_Style",
    "DScore_Style",
    "DScore_Style",
    "DScore_Style",
    "DScore_Style",
    "DScore_Daring",
    "DScore_Daring",
    "DScore_Daring",
    "DScore_Daring",
};

// Stores actual bonus value.
int BonusValues[PLAYERMAX][BONUSCOUNT];

// Stores time bonus was last set.
int LastBonus[PLAYERMAX][BONUSCOUNT];

// Clientside. Stores the last value a bonus was seen with.
// (also make sure to add score definitions in pickup_client_defs.h
int Bonus_LastSeen[PLAYERMAX][BONUSCOUNT];



global int 22:MapScoreData[];

#define REWARDDATA_SCORE         0
#define REWARDDATA_DISPLAYSCORE (1 * PLAYERMAX)
#define REWARDDATA_REWARDCOUNT  (2 * PLAYERMAX)
#define REWARDDATA_SCOREPERCENT (3 * PLAYERMAX)
#define REWARDDATA_REGENTIMER   (4 * PLAYERMAX)
#define REWARDDATA_REGENSPENT   (5 * PLAYERMAX)
#define REWARDDATA_EXTRALIVES   (6 * PLAYERMAX)
#define REWARDDATA_HASLIVES     (7 * PLAYERMAX)
#define REWARDDATA_LASTRESET    (8 * PLAYERMAX)


#define REWARDMASK_SCORE         (1 << 0)
#define REWARDMASK_DISPLAYSCORE  (1 << 1)
#define REWARDMASK_REWARDCOUNT   (1 << 2)
#define REWARDMASK_SCOREPERCENT  (1 << 3)
#define REWARDMASK_REGENTIMER    (1 << 4)
#define REWARDMASK_REGENSPENT    (1 << 5)
#define REWARDMASK_EXTRALIVES    (1 << 6)
#define REWARDMASK_HASLIVES      (1 << 7)
#define REWARDMASK_LASTRESET     (1 << 8)

#define REWARDMASK_ONLYDISPLAY      (REWARDMASK_DISPLAYSCORE)
#define REWARDMASK_NOTDISPLAY       (~REWARDMASK_ONLYDISPLAY)
#define REWARDMASK_ONLYREWARDS      (REWARDMASK_REGENTIMER | REWARDMASK_REGENSPENT | REWARDMASK_EXTRALIVES | REWARDMASK_HASLIVES)
#define REWARDMASK_NOTREWARDS       (~REWARDMASK_ONLYREWARDS)
#define REWARDMASK_ALL              0xFFFFFFFF

function int Score_GetScore(int pln)
{
    return MapScoreData[REWARDDATA_SCORE + pln];
}

function void Score_SetScore(int pln, int val)
{
    MapScoreData[REWARDDATA_SCORE + pln] = val;
}

function void Score_ModScore(int pln, int val)
{
    MapScoreData[REWARDDATA_SCORE + pln] += val;
}



function int Score_GetDisplayScore(int pln)
{
    return MapScoreData[REWARDDATA_DISPLAYSCORE + pln];
}

function void Score_SetDisplayScore(int pln, int val)
{
    MapScoreData[REWARDDATA_DISPLAYSCORE + pln] = val;
}

function void Score_ModDisplayScore(int pln, int val)
{
    MapScoreData[REWARDDATA_DISPLAYSCORE + pln] += val;
}


function void Score_ModBothScores(int pln, int val)
{
    MapScoreData[REWARDDATA_SCORE        + pln] += val;
    MapScoreData[REWARDDATA_DISPLAYSCORE + pln] += val;
}




function int Score_GetRewardCount(int pln)
{
    return MapScoreData[REWARDDATA_REWARDCOUNT + pln];
}

function void Score_SetRewardCount(int pln, int val)
{
    MapScoreData[REWARDDATA_REWARDCOUNT + pln] = val;
}

function void Score_ModRewardCount(int pln, int val)
{
    MapScoreData[REWARDDATA_REWARDCOUNT + pln] += val;
}



function int Score_GetScorePercent(int pln)
{
    return MapScoreData[REWARDDATA_SCOREPERCENT + pln];
}

function void Score_CalcScorePercent(int pln)
{
    if (Score_Thresholds[ST_FULLHEAL] <= 0)
    {
        MapScoreData[REWARDDATA_SCOREPERCENT + pln] = 0;
    }
    else
    {
        MapScoreData[REWARDDATA_SCOREPERCENT + pln] = FixedDiv(Score_GetScore(pln), Score_Thresholds[ST_FULLHEAL]) % 1.0;
    }
}




function int Score_GetRegenTimer(int pln)
{
    return MapScoreData[REWARDDATA_REGENTIMER + pln];
}

function void Score_SetRegenTimer(int pln, int val)
{
    MapScoreData[REWARDDATA_REGENTIMER + pln] = val;
}

function void Score_ModRegenTimer(int pln, int val)
{
    MapScoreData[REWARDDATA_REGENTIMER + pln] += val;
}



function int Score_GetRegenSpent(int pln)
{
    return MapScoreData[REWARDDATA_REGENSPENT + pln];
}

function void Score_SetRegenSpent(int pln, int val)
{
    MapScoreData[REWARDDATA_REGENSPENT + pln] = val;
}

function void Score_ModRegenSpent(int pln, int val)
{
    MapScoreData[REWARDDATA_REGENSPENT + pln] += val;
}



function int Score_GetExtraLives(int pln)
{
    return MapScoreData[REWARDDATA_EXTRALIVES + pln];
}

function void Score_SetExtraLives(int pln, int val)
{
    MapScoreData[REWARDDATA_EXTRALIVES + pln] = val;
}

function void Score_ModExtraLives(int pln, int val)
{
    MapScoreData[REWARDDATA_EXTRALIVES + pln] += val;
}



function int Score_GetHasLives(int pln)
{
    return MapScoreData[REWARDDATA_HASLIVES + pln];
}

function void Score_SetHasLives(int pln, int val)
{
    MapScoreData[REWARDDATA_HASLIVES + pln] = val;
}

function void Score_ModHasLives(int pln, int val)
{
    MapScoreData[REWARDDATA_HASLIVES + pln] += val;
}



function int Score_GetLastReset(int pln)
{
    return MapScoreData[REWARDDATA_LASTRESET + pln];
}

function void Score_SetLastReset(int pln, int val)
{
    MapScoreData[REWARDDATA_LASTRESET + pln] = val;
}

function void Score_ModLastReset(int pln, int val)
{
    MapScoreData[REWARDDATA_LASTRESET + pln] += val;
}

