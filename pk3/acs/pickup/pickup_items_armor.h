#define ARMORCOUNT 13

int PKP_KnownArmors[ARMORCOUNT] =
{
    "ArmorBonus",
    "GreenArmor",
    "BlueArmor",
    "BlueArmorForMegasphere",
    "SilverShield",
    "EnchantedShield",
    "LeatherArmor",
    "MetalArmor",
    "RedArmor",

    "Dakka_GreenArmor",
    "Dakka_BlueArmor",
    "Dakka_ArmorBonus",
    "Dakka_ArmorBonusCapped",
};

// See AMODE_* for comparison modes.
#define ARM_COMPAREMODE     0

// Replace current armor, or add to it? See ATYPE_*.
#define ARM_TYPE            1

// How much armor do we get?
#define ARM_PICKUPPOINTS    2

// How much armor *can* we get?
#define ARM_MAXPOINTS       3

// How much does it protect the wearer?
#define ARM_PROTECTION      4

// When determining the worth of this armor, what method should we use?
//
// Note: The comparison method is taken from the armor being given, with one
//  exception: if the player's current armor type isn't in PKP_KnownArmors,
//  we always use AMODE_POINTS because we don't have enough info for anything
//  else.

// Just use how many armor points you currently have.
#define AMODE_POINTS        0

// If you take just enough damage to lose all your armor, how much health would
//  be protected? Formula's <armor points> * <protection ratio>. For your
//  current armor, the amount of armor points you have is used; for the new
//  armor, the amount of armor points you'd end up with is used.
//
// Examples:
// - Green armor has a protection value of 33.
// - Blue armor has a protection value of 100.
// - Enchanted shield has a protection value of 150.
#define AMODE_PROTECTION    1

// Try to replace current armor if getting the new armor would be better.
#define ATYPE_REPLACE       0

// Add to current armor if there is any.
#define ATYPE_BONUS         1

// Replicate the Quake 2 armor pickup behaviour. It's as such:
//
// Take the armor protection ratios of both the armor you have now, and the
//  armor you intend to grab. Use the better of the two.
//
// Now with the better armor ratio, you recalculate both armors' point values
//  as such:
//
// <new armor points> = <old armor points> * (<armor protect ratio> / <best protect ratio>)
//
// Add the adjusted points together, and that's your new armor value.
//
// So in Quake 2, assuming you had 50 jacket armor (the max) and grabbed a
//  set of combat armor:
//
// Jacket armor (current): 30% protection, 50 armor.
// Combat armor (pickup):  60% protection, 50 armor.
//
// Best armor ratio: 60%.
//
// Jacket armor points, adjusted: 50 * (30% / 60%) = 25
// Combat armor points, adjusted: 50 * (60% / 60%) = 50
//
// End armor result: Combat armor (60% protection) at 75 points
//
// If the armor you currently have is unknown, just do ATYPE_REPLACE instead.
#define ATYPE_QUAKE2        2


// Add to current armor, but use the higher of the two armor maxes.
#define ATYPE_ADDTOHIGHEST  3


int PKP_ArmorData[ARMORCOUNT][5] =
{
    {AMODE_POINTS,  ATYPE_BONUS,    1,      200,    0.33335},
    {AMODE_POINTS,  ATYPE_REPLACE,  100,    100,    0.33335},
    {AMODE_POINTS,  ATYPE_REPLACE,  200,    200,    0.5},
    {AMODE_POINTS,  ATYPE_REPLACE,  200,    200,    0.5},
    {AMODE_POINTS,  ATYPE_REPLACE,  100,    100,    0.5},
    {AMODE_POINTS,  ATYPE_REPLACE,  200,    200,    0.75},
    {AMODE_POINTS,  ATYPE_REPLACE,  100,    100,    0.33335},
    {AMODE_POINTS,  ATYPE_REPLACE,  200,    200,    0.5},
    {AMODE_POINTS,  ATYPE_REPLACE,  200,    200,    0.66666},

    // Dakka armors
    {AMODE_PROTECTION,  ATYPE_QUAKE2,       1,      150,    0.33335},
    {AMODE_PROTECTION,  ATYPE_QUAKE2,       1,      300,    0.5},
    {AMODE_PROTECTION,  ATYPE_ADDTOHIGHEST, 2,      300,    0.33335},
    {AMODE_PROTECTION,  ATYPE_ADDTOHIGHEST, 2,      200,    0.33335},
};

