// Get healed.
// Everything related to defining health pickups is in here. You *must* add
//  any health pickup stuff in here, or the pickup system won't know how to
//  handle it.

#define HEALTHCOUNT 7

// NOTE: These health items don't have to actually exist!
//  So long as they're defined here, the pickup system will know what
//  to do with them.

int PKP_HealthTypes[HEALTHCOUNT] =
{
    "BaseHealth",
    "HealthBonus",
    "Medikit",
    "MegasphereHealth",
    "DakkaHealthBonus",
    "DakkaSoulsphere",
    "DakkaMegasphere",
};

// Now we define how each health pickup actually works. How much health they
//  give, at what health ranges they give them, how much health is low health,
//  so on and so forth.

// Now, since player classes can have any number of health values, there's a
//  number of ways to specify both what health ranges we should work with,
//  and how we should heal.

// "This point does not actually exist."
#define HSCALE_BLANK 0

// Don't do anything to the given value.
#define HSCALE_NONE  1

// Add the player's max health to the given value.
#define HSCALE_ADD   2

// Multiply the given value with the player's max health.
#define HSCALE_MULT  3



// These values are for scaling healing numbers.

// Heal just the specified amount of health.
#define HTYPE_CONSTANT  0

// Multiply the given healing value with the player's max health to get the
//  actual healing value.
#define HTYPE_PERCENT   1



// Healing points define ranges in which the player will get healed for a
//  certain amount. From a given healing points, its healing value will be
//  used until the next highest healing point is reached, at which point its
//  healing will be used.
//
// So for example:
//
//  {HSCALE_NONE,   300,    HTYPE_CONSTANT,     1}, // oh shit this is
//  {HSCALE_NONE,   200,    HTYPE_CONSTANT,     2}, // basically dakka's
//  {HSCALE_NONE,   100,    HTYPE_CONSTANT,     3}, // health bonus values
//  {HSCALE_NONE,    50,    HTYPE_CONSTANT,     4}, // oh noooooooooooooo
//
//  From   0 to  50 HP, you get healed for 4 HP.
//  From  50 to 100 HP, you get healed for 3 HP.
//  From 100 to 200 HP, you get healed for 2 HP.
//  From 200 to 300 HP, you get healed for 1 HP.
//
//  Above 300 HP, you don't get healed.

// How many healing points do we want to work with?
#define HPOINT_COUNT    4

// This defines the point at which its healing applies. 
#define HPOINT_BASE         1

// This defines how to modify that base value.
//  Goes by the HSCALE_* constants.
#define HPOINT_BASESCALAR   0

// This defines how much to heal by...
#define HPOINT_HEAL         3   

// and this defines how to modify that.
//  Goes by the HTYPE_* constants.
#define HPOINT_HEALSCALAR   2

int PKP_HealingPoints[HEALTHCOUNT][HPOINT_COUNT][4] =
{
    // Base health (up to class max health)
    {
        {HSCALE_ADD,    0,      HTYPE_CONSTANT,     1},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
    },

    // Health bonus (up to class max health +100)
    {
        {HSCALE_ADD,    100,    HTYPE_CONSTANT,     1},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
    },

    // Medikit (up to class max health, below 25 gives low message)
    {
        {HSCALE_ADD,    0,      HTYPE_CONSTANT,     1},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
    },

    // Megasphere health (200% class' max health)
    {
        {HSCALE_MULT,   2.0,    HTYPE_PERCENT,      2.0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
    },

    // Dakka health bonus (4hp to 50, 3hp to 100, 2hp to 200, 1hp to 300)
    {
        {HSCALE_MULT,   3.0,    HTYPE_CONSTANT,     1},
        {HSCALE_MULT,   2.0,    HTYPE_CONSTANT,     2},
        {HSCALE_MULT,   1.0,    HTYPE_CONSTANT,     3},
        {HSCALE_MULT,   0.5,    HTYPE_CONSTANT,     4},
    },

    // Dakka soulsphere (2hp to 200, 1hp to 300)
    {
        {HSCALE_MULT,   3.0,    HTYPE_CONSTANT,     1},
        {HSCALE_MULT,   2.0,    HTYPE_CONSTANT,     2},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
    },

    // Dakka megasphere (300% class' max health)
    {
        {HSCALE_MULT,   3.0,    HTYPE_PERCENT,      3.0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
        {HSCALE_BLANK,  0,      0,                  0},
    },
};



// For any health item that defines low health messages, this is where you want
//  to be. The low health value uses the same scaling system as the base health
//  values in HealingPoints.

// Base healing value.
#define HLOW_BASE       0

// Scalar, using HSCALE_* constants.
#define HLOW_SCALAR     1

int PKP_LowHealthValues[HEALTHCOUNT][2] =
{
    {0,  HSCALE_BLANK},
    {0,  HSCALE_BLANK},
    {25, HSCALE_NONE},
    {0,  HSCALE_BLANK},
    {0,  HSCALE_BLANK},
    {0,  HSCALE_BLANK},
};



// Low health messages override any message that would normally be displayed.
//  In case of conflicts, last one evaluated takes precedence.
//
// If picking up health gets you blank messages occasionally, this is
//  probably why.

int PKP_LowHealthMessages[HEALTHCOUNT] =
{
    "",
    "",
    "$GOTMEDINEED",
    "",
    "",
    "",
};
