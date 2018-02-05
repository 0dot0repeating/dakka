int TMP_HealthPoints[HPOINT_COUNT][2];



function int Heal_HealthIndex(int item)
{
    int i;

    for (i = 0; i < HEALTHCOUNT; i++)
    {
        if (stricmp(item, PKP_HealthTypes[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}


// Insertion sort of health points. Highest to lowest.
//
// This is called after TMP_HealthPoints has been filled in with points
//  calculated from PKP_HealingPoints.
function void Heal_SortHealthPoints(int pointCount)
{
    int i, j;

    for (i = 0; i < pointCount; i++)
    {
        int base = TMP_HealthPoints[i][0];
        int heal = TMP_HealthPoints[i][1];

        for (j = i; j > 0; j--)
        {
            int base_comp = TMP_HealthPoints[j-1][0];
            int heal_comp = TMP_HealthPoints[j-1][1];

            if (base > base_comp)
            {
                TMP_HealthPoints[j][0] = base_comp;
                TMP_HealthPoints[j][1] = heal_comp;
            }
            else
            {
                TMP_HealthPoints[j][0] = base;
                TMP_HealthPoints[j][1] = heal;
                break;
            }
        }
    }
}

function int Heal_AdjustHealth(int hp, int maxHealth, int type)
{
    switch (type)
    {
      case HSCALE_NONE:
      default:
        break;

      case HSCALE_ADD:
        hp += maxHealth;
        break;

      case HSCALE_MULT:
        hp = FixedMul(maxHealth, hp);
        break;
    }

    return hp;
}



// Frontend to Heal_PickupHealth that gets the name of the health item
//  rather than the index.
function void Heal_PickupHealthName(int hpName, int amount)
{
    int index = Heal_HealthIndex(hpName);
    if (index != -1) { Heal_PickupHealth(index, amount); }
}



function void Heal_PickupHealth(int index, int amount)
{
    int startHealth = GetActorProperty(0, APROP_Health);
    int maxHealth   = getMaxHealth();

    // Check for low HP, if this pickup even has low health data.
    int p_lowScalar = PKP_LowHealthValues[index][HLOW_SCALAR];

    if (p_lowScalar != HSCALE_BLANK)
    {
        int p_lowHealth = PKP_LowHealthValues[index][HLOW_BASE];
        int lowHealth = Heal_AdjustHealth(p_lowHealth, maxHealth, p_lowScalar);

        if (startHealth <= lowHealth)
        {
            // We pass index along through this so that the message script
            //  knows which health message to display
            PKP_ReturnArray[PARRAY_LOWHEALTH] = index+1;
        }
    }

    // since the amount of health given can vary based off of how much health
    //  you have, we need to figure out where the breakpoints are for how much
    //  you should be healed
    //
    // and since health points can be based off of max health, we need to
    //  calculate and sort them to make sure they're right

    int healthPointCount = 0;

    for (int i = 0; i < HPOINT_COUNT; i++)
    {
        int h_basescale = PKP_HealingPoints[index][i][HPOINT_BASESCALAR];
        int h_base      = PKP_HealingPoints[index][i][HPOINT_BASE];
        int h_healscale = PKP_HealingPoints[index][i][HPOINT_HEALSCALAR];
        int h_heal      = PKP_HealingPoints[index][i][HPOINT_HEAL];


        // every health entry has to be the same length, so we use HSCALE_BLANK
        //  to mark empty entries
        if (h_basescale == HSCALE_BLANK) { continue; }

        int adjust_base = Heal_AdjustHealth(h_base, maxHealth, h_basescale);
        int adjust_heal = h_heal;

        switch (h_healscale)
        {
          case HTYPE_CONSTANT:
          default:
            break;

          case HTYPE_PERCENT:
            adjust_heal = FixedMul(adjust_heal, maxHealth);
            break;
        }

        int point = healthPointCount++;
        TMP_HealthPoints[point][0] = adjust_base;
        TMP_HealthPoints[point][1] = adjust_heal;
    }


    // well shit we ain't got anything to do
    if (healthPointCount == 0) { return; }

    Heal_SortHealthPoints(healthPointCount);

    // give health

    int pointsLeft  = amount;

    while (pointsLeft > 0)
    {
        int curHealth = GetActorProperty(0, APROP_Health);

        int g_healindex = -1;

        // figure out what health breakpoint we fall below
        //
        // since it's sorted top to bottom, we know if we're above the first
        //  breakpoint that we're outside the valid healing range

        for (i = 0; i < healthPointCount; i++)
        {
            int c_health = TMP_HealthPoints[i][0];

            if (curHealth >= c_health) { break; }

            g_healindex = i;
        }

        // hit the max health allowed for this pickup
        if (g_healindex == -1) { break; }

        int g_base = TMP_HealthPoints[g_healindex][0];
        int g_heal = TMP_HealthPoints[g_healindex][1];

        int g_maxheal    = g_base - curHealth;

        // (g_maxheal + (g_heal - 1)) / g_heal
        //
        // that describes how many times we'd need to give g_heal in health to
        //  reach the healing breakpoint and get to the next one
        //
        // eg: we have a 25 health pickup, the next healing breakpoint is at 75,
        //     and we have 50 health
        //
        // g_heal    = 25 hp
        // g_maxheal = 25 hp
        //
        // 25hp + (25hp - 1hp) = 49 hp
        // 49hp / 25hp = 1 (remember, integer division)
        //
        // therefore we only need one point
        //
        // if, however, we were at 49 health,
        //
        // g_maxheal = 26hp
        // 25hp + 25hp - 1hp = 50hp
        // 50hp / 25hp = 2
        //
        // so we need two points

        int g_pointspend = min(pointsLeft, (g_maxheal + g_heal - 1) / g_heal);

        int g_pointheal  = g_pointspend * g_heal;

        // never break out of the total healing range
        // eg. given a range of 0 to 300, and healing in 5 point increments, you
        //     can go from 199 to 204 even if 200 is a midpoint, but you can't
        //     go from 299 to 304, only to 300
        if (g_healindex == 0)
        {
            g_pointheal = min(g_maxheal, g_pointheal);
        }

        SetActorProperty(0, APROP_Health, curHealth + g_pointheal);
        pointsLeft -= g_pointspend;
    }

    int endHealth = GetActorProperty(0, APROP_Health);

    if (startHealth < endHealth)
    {
        PKP_ReturnArray[PARRAY_SUCCEEDED]   = true;
        PKP_ReturnArray[PARRAY_CONSUME]     = true;
    }
}
