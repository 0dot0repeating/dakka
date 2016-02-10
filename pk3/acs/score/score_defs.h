#define SCOREWEPS           32
#define SCOREFIREMODES      3

#define S_FIST              0
#define S_SCRAPPER          1
#define S_PISTOL            2
#define S_SHOTGUN           3
#define S_SUPERSHOTGUN      4
#define S_CHAINGUN          5
#define S_MINIGUN           6
#define S_ROCKETLAUNCHER    7
#define S_PLASMARIFLE       8
#define S_CHANNELER         9
#define S_BFG9000           10

#define SWITCHAROO_TICSPER100HP 24
#define SWITCHAROO_MINTICS      72
#define SWITCHAROO_MAXTICS      144

#define KILLSTREAK_TICSPER100HP 24
#define KILLSTREAK_MINTICS      72
#define KILLSTREAK_MAXTICS      360
#define KILLSTREAK_MULTMAX      0.25
#define KILLSTREAK_MULTINC      0.025
#define KS_COUNT    0
#define KS_TIME     1
#define KS_TIMEADD  2

#define EFFICIENCY_MULTINC      0.1
#define EFFICIENCY_MULTMAX      0.5

#define BONEDRY_MAXHP           25
#define BONEDRY_MULTPERHP       0.02

#define AIR_MINHEIGHT           64.0
#define AIR_MULT                0.005
#define AIR_MAXMULT             2.0

#define CURVEBALL_MULT          0.2

#define SCRAPPING_MULT          0.125

#define POINTBLANK_DIST         128.0
#define POINTBLANK_MULT         0.1

#define INFIGHTER_MULT          0.2

world int 54:PlayerMapScores[];

// Allowing three fire modes.
//  0 - primary   (+attack)
//  1 - secondary (+altattack)
//  2 - tertiary  (some combo of the two, probably won't be used)
int PlayerWeapons_LastFired[PLAYERMAX][SCOREWEPS][SCOREFIREMODES];
int PlayerKills_LastFired[PLAYERMAX][2];
int PlayerKillStreaks[PLAYERMAX][3];


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

int MapStart_BasePoints     = 0;
int MapStart_BaseMonsters   = 0;
int MapStart_AveragePoints  = 0;
int MapStart_FullHealPoints = 0;


// CLASSCOUNT_DEFINED is defined in pickup/pickup_classes.h
//  This is used for Switcharoo and One Stone: Switcharoo checks it to see if
//  it applies at all, and One Stone uses it to see if we can rely on last
//  weapon fire time to actually be updated
//  
int SCORE_UpdatesSwitcharoo[CLASSCOUNT_DEFINED] =
{
    true,
};

#define KNOWNMELEE 15

int Brawler_KnownMelee[KNOWNMELEE] =
{
    "Fist",
    "Chainsaw",
    "DWep_Fist",
    "DWep_Chainsaw",
    "PunchDagger",
    "FWeapFist",
    "FWeapAxe",
    "FWeapHammer",
    "CWeapMace",
    "Staff",
    "Gauntlets",
    "Fight",
    "Knife",
    "Fist~",
    "Chainsaw~",
};

int Brawler_MeleeMults[KNOWNMELEE] = 
{
    0.5,
    0.25,
    0.25,
    0.15,
    0.5,
    0.25,
    0.15,
    0.1,
    0.3,
    0.5,
    0.25,
    0.5,
    0.25,
    0.5,
    0.25,
};


#define BONUSCOUNT  12

#define BS_BASE         0
#define BS_SPREE        1
#define BS_EFFICIENT    2
#define BS_INFIGHTER    3
#define BS_SWITCHAROO   4
#define BS_SCRAPPING    5
#define BS_CURVEBALL    6
#define BS_AIR          7
#define BS_BONEDRY      8
#define BS_SORELOSER    9
#define BS_BRAWLER      10
#define BS_POINTBLANK   11

// CR_BRICK  is efficiency
// CR_GREEN  is style
// CR_ORANGE is daring

int BonusNames[BONUSCOUNT] =
{
    "Base",
    "Spree",
    "One Stone",
    "Infighter",
    "Switcharoo",
    "Scrapping",
    "Curveball",
    "Air",
    "Bone Dry",
    "Sore Loser",
    "Brawler",
    "Point-Blank",
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
