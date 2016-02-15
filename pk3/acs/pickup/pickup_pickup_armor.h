// ARMOR PICKUP - BECAUSE BEING BUTT NAKED IS WHAT BARBARIANS DO



// Look up a given armor in PKP_KnownArmor and return its index.
//  If we can't find it, return -1.
function int Armor_ArmorIndex(int armor)
{
    int i;

    for (i = 0; i < ARMORCOUNT; i++)
    {
        if (!strcmp_i(PKP_KnownArmors[i], armor))
        {
            return i;
        }
    }

    return -1;
}


// Look up the player's current armor in PKP_KnownArmor and return its index.
//  If we can't find it, return -1.
function int Armor_CurrentArmorIndex(void)
{
    int i;
    int pln = PlayerNumber();

    for (i = 0; i < ARMORCOUNT; i++)
    {
        if (GetArmorType(PKP_KnownArmors[i], pln))
        {
            return i;
        }
    }

    return -1;
}



// Weigh given armor value and protection ratios.
function int Armor_WeighArmor(int points, int protect, int method)
{
    switch (method)
    {
      case AMODE_POINTS:
        return points;

      case AMODE_PROTECTION:
        return points * protect;
    }

    return points;
}


function void Armor_PickupArmorName(int armor, int count)
{
    int index = Armor_ArmorIndex(armor);

    if (index != -1)
    {
        Armor_PickupArmor(index, count);
    }
}

function void Armor_PickupArmor(int armorTo_index, int count)
{
    int armorTo_name    = PKP_KnownArmors[armorTo_index];
    int armorTo_compare = PKP_ArmorData[armorTo_index][ARM_COMPAREMODE];
    int armorTo_type    = PKP_ArmorData[armorTo_index][ARM_TYPE];
    int armorTo_points  = PKP_ArmorData[armorTo_index][ARM_PICKUPPOINTS];
    int armorTo_max     = PKP_ArmorData[armorTo_index][ARM_MAXPOINTS];
    int armorTo_protect = PKP_ArmorData[armorTo_index][ARM_PROTECTION];


    // Don't grab comparison since we're using the compare method from the
    //  armor being given.
    int armorFrom_index     = Armor_CurrentArmorIndex();
    int armorFrom_points    = CheckInventory("Armor");
    int armorFrom_protect   = GetArmorInfo(ARMORINFO_SAVEPERCENT);
    int armorFrom_name      = GetArmorInfo(ARMORINFO_CLASSNAME);
    int armorFrom_max       = GetArmorInfo(ARMORINFO_SAVEAMOUNT);

    // If we know about the armor, use the info we have in the ACS. We can store
    //  more info in ACS than we can in the armor itself.

    if (armorFrom_index != -1)
    {
        armorFrom_protect = PKP_ArmorData[armorFrom_index][ARM_PROTECTION];
        armorFrom_max     = PKP_ArmorData[armorFrom_index][ARM_MAXPOINTS];
    }

    // Special case for the armor given by idfa and idkfa.
    if (armorFrom_protect == 0.5 && !strcmp_i(armorFrom_name, "BasicArmorPickup"))
    {
        armorFrom_name = "BlueArmor";
    }

    // Right now, we're just determining what the end armor values will be if
    //  we decide to pick this up.
    int armorEnd_points; 
    int armorEnd_name, armorEnd_max, armorEnd_protect;

    switch (armorTo_type)
    {
      case ATYPE_REPLACE:
        armorEnd_points  = armorFrom_points + (armorTo_points * count);
        armorEnd_name    = armorTo_name;
        armorEnd_max     = armorTo_max;
        armorEnd_protect = armorTo_protect;
        break;

      case ATYPE_BONUS:
        armorEnd_points  = armorFrom_points + (armorTo_points * count);

        if (armorFrom_points == 0)
        {
            armorEnd_name    = armorTo_name;
            armorEnd_protect = armorTo_protect;
            armorEnd_max     = armorTo_max;
        }
        else
        {
            armorEnd_name    = armorFrom_name;
            armorEnd_protect = armorFrom_protect;
            armorEnd_max     = armorFrom_max;
        }
        break;

      case ATYPE_ADDTOHIGHEST:
        armorEnd_points  = armorFrom_points + (armorTo_points * count);
        armorEnd_max     = max(armorTo_max, armorFrom_max);

        if (armorFrom_points == 0)
        {
            armorEnd_name    = armorTo_name;
            armorEnd_protect = armorTo_protect;
        }
        else
        {
            armorEnd_name    = armorFrom_name;
            armorEnd_protect = armorFrom_protect;
        }
        break;

      case ATYPE_QUAKE2:
        int betterRatio;
        int armorFrom_adjusted;
        int armorTo_adjusted;

        // If the protection ratios are equal, default to the armor getting picked up.
        if (armorFrom_protect > armorTo_protect)
        {
            betterRatio = armorFrom_protect;

            armorEnd_name    = armorFrom_name;
            armorEnd_protect = armorFrom_protect;
            armorEnd_max     = armorFrom_max;
        }
        else
        {
            betterRatio = armorTo_protect;

            armorEnd_name    = armorTo_name;
            armorEnd_protect = armorTo_protect;
            armorEnd_max     = armorTo_max;
        }

        armorFrom_adjusted = armorFrom_points * FixedDiv(armorFrom_protect, betterRatio);
        armorTo_adjusted   = (armorTo_points * count) * FixedDiv(armorTo_protect,   betterRatio);

        armorEnd_points = round(armorFrom_adjusted + armorTo_adjusted);
        break;
    }

    armorEnd_points = min(armorEnd_max, armorEnd_points);

    // Now we weigh the start and end armor values.

    int armorFrom_weight = Armor_WeighArmor(armorFrom_points, armorFrom_protect, armorTo_compare);
    int armorEnd_weight  = Armor_WeighArmor(armorEnd_points,  armorEnd_protect,  armorTo_compare);

    
    // Now pick up if we'd gain armor.
    if (armorEnd_weight > armorFrom_weight)
    {
        // Unknown armor? Just give bonuses.
        // (Note: armorEnd_name can't be "" if you have no armor, both armor
        //  types change the armor name to armorTo_name in that case)

        if (StrLen(armorEnd_name) == 0)
        {
            GiveInventory("Pickup_OneArmor", armorEnd_points - armorFrom_points);
        }
        else
        {
            TakeInventory("BasicArmor", 0x7FFFFFFF);
            GiveInventory(armorEnd_name, 1);
            
            int curArmor = CheckInventory("Armor");
            
            if (curArmor < armorEnd_points)
            {
                GiveInventory("Pickup_OneArmor", armorEnd_points - curArmor);
            }
            else
            {
                TakeInventory("BasicArmor", curArmor - armorEnd_points);
            }
        }

        PKP_ReturnArray[PARRAY_SUCCEEDED] = true;
        PKP_ReturnArray[PARRAY_CONSUME]   = true;
    }
}
