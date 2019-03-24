// Weapon functions son


// Look up the activator's current weapon in PKP_KnownGuns and return its index.
//  If we can't find it, return -1.
function int Weapon_CurrentWeaponIndex(void)
{
    int i;

    for (i = 0; i < WEAPONCOUNT; i++)
    {
        if (CheckWeapon(PKP_KnownGuns[i][WEP_NAME]))
        {
            return i;
        }
    }

    return -1;
}



// Look up a given weapon in PKP_KnownGuns and return its index.
//  If we can't find it, return -1.
function int Weapon_WeaponIndex(int weapon)
{
    int i;

    for (i = 0; i < WEAPONCOUNT; i++)
    {
        if (!stricmp(PKP_KnownGuns[i][WEP_NAME], weapon))
        {
            return i;
        }
    }

    return -1;
}



// Frontend to Weapon_PickupWeapon that gets the name of the weapon
//  rather than the index.
function void Weapon_PickupWeaponName(int wepName, int amount, int dropped)
{
    int index = Weapon_WeaponIndex(wepName);
    if (index != -1) { Weapon_PickupWeapon(index, amount, dropped); }
}



function void Weapon_PickupWeapon(int wepIndex, int count, int dropped)
{
    int weapon    = PKP_KnownGuns[wepIndex][WEP_NAME];
    int hasWeapon = CheckInventory(weapon) > 0;

    int ammo1 = PKP_KnownGuns[wepIndex][WEP_AMMO1];
    int ammo2 = PKP_KnownGuns[wepIndex][WEP_AMMO2];

    int usesAmmo1  = !stringBlank(ammo1);
    int usesAmmo2  = !stringBlank(ammo2);

    // Dropped weapons never use sv_weaponstay
    int weaponstay = GetCVar("sv_weaponstay") && !dropped;

    // If the weapon stays on the ground, we only pick it up once
    // Otherwise we can still pick it up if it'd give us ammo
    if (weaponstay && hasWeapon) { return; }

    int ammo1_startcount = 0, ammo1_endcount = 0, ammo1_cap = 0;
    int ammo2_startcount = 0, ammo2_endcount = 0, ammo2_cap = 0;

    // Uncapped ammo pickup
    int unlimitedPickup = GetCVar("sv_unlimited_pickup");

    if (usesAmmo1)
    {
        ammo1_startcount = CheckInventory(ammo1);
        ammo1_cap        = GetAmmoCapacity(ammo1);

        // Doubling ammo capacity when dropped means that we'll
        // never have issues when the ammo you get hits the ammo cap
        if (dropped) { SetAmmoCapacity(ammo1, ammo1_cap * 2); }
    }

    if (usesAmmo2)
    {
        ammo2_startcount = CheckInventory(ammo2);
        ammo2_cap        = GetAmmoCapacity(ammo2);
        if (dropped) { SetAmmoCapacity(ammo2, ammo2_cap * 2); }
    }

    int i;

    // This is retarded.
    // Giving more than one of a weapon doesn't give you proportionate ammo;
    //  it gives you the same amount of ammo as if you only gave one. Hence
    //  this stupid-ass for loop.
    //
    // Why is everything in ZDoom retarded?
    for (i = 0; i < count; i++)
    {
        GiveInventory(weapon, 1);
    }

    int gotWeapon = CheckInventory(weapon);

    int pickupDidSomething = gotWeapon && !hasWeapon;
    int changed;

    if (usesAmmo1)
    {
        if (dropped)
        {
            ammo1_endcount = CheckInventory(ammo1);

            // cut off either half the ammo gained, or cap it at the actual max
            int a1diff;

            // Half of the ammo given from the weapon. Rounds up.
            a1diff = ((ammo1_endcount - ammo1_startcount) + 1) / 2;

            if (!unlimitedPickup)
            {
                a1diff = max(a1diff, ammo1_endcount - ammo1_cap);
            }

            TakeInventory(ammo1, a1diff);
            SetAmmoCapacity(ammo1, ammo1_cap);
        }

        // refresh since it could've changed
        ammo1_endcount = CheckInventory(ammo1);
        changed = (ammo1_endcount != ammo1_startcount);
        pickupDidSomething |= changed;
    }

    if (usesAmmo2)
    {
        if (dropped)
        {
            ammo2_endcount = CheckInventory(ammo2);

            int a2diff;

            // Half of the ammo given from the weapon. Rounds up.
            a2diff = ((ammo2_endcount - ammo2_startcount) + 1) / 2;

            if (!unlimitedPickup)
            {
                a2diff = max(a2diff, ammo2_endcount - ammo2_cap);
            }

            TakeInventory(ammo2, a2diff);
            SetAmmoCapacity(ammo2, ammo2_cap);
        }

        ammo2_endcount = CheckInventory(ammo2);
        changed = (ammo2_endcount != ammo2_startcount);
        pickupDidSomething |= changed;
    }

    // Handle weapon switch behaviour. Normally the client handles this,
    //  but GiveInventory doesn't do weapon switching on its own,
    //  so we have to.
    if (pickupDidSomething && !hasWeapon)
    {
        if (IsServer)
        {
            ACS_NamedExecuteWithResult("PWeapon_Switch", wepIndex);
        }
        else
        {
            ACS_NamedExecuteAlways("PWeapon_Switch", 0, wepIndex);
        }
    }

    if (pickupDidSomething)
    {
        PKP_ReturnArray[PARRAY_SUCCEEDED]   = true;
        PKP_ReturnArray[PARRAY_CONSUME]    |= !weaponstay;
    }
}

script "PWeapon_Switch" (int wepIndex) clientside
{
    int doswitch = false;

    int weapon    = PKP_KnownGuns[wepIndex][WEP_NAME];
    int pickupState;

    if (Pickup_IsZandronum())
    {
        pickupState = GetCVar("switchonpickup");
    }
    else
    {
        pickupState = cond(GetCvar("neverswitchonpickup"), 0, 2);
    }

    switch (pickupState)
    {
      case 0:
        // Never switch
        break;

      case 1:
        // Switch only on higher priority
        // If the old gun isn't in the weapon index, switch anyway

        int oldPriority;
        int newPriority = PKP_WeaponPriorities[wepIndex];
        int curGun = Weapon_CurrentWeaponIndex();

        if (curGun == -1) { oldPriority = 0x7FFFFFFF; }
        else { oldPriority = PKP_WeaponPriorities[curGun]; }

        if (oldPriority >= newPriority) { doswitch = true; }
        break;

      case 2:
        // Always switch
        doswitch = true;
        break;
    }

    if (doswitch)
    { 
        if (Pickup_IsZandronum() && !IsServer)
        {
            NamedRequestScriptPuke("Dakka_SwitchingTo", wepIndex);
        }
        else
        {
            SetWeapon(weapon);
        }
    }
}
