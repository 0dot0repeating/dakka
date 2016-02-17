int Translate_ToTake[65535];


function void Dakka_TranslateGuns(void)
{
    int slot, class;
    int classNum = Pickup_ClassNumber(0);
    int classWepIndex = classNum + 1;

    int hisWeapon, takingSomething;

    int takeCount = 0;

    // Take away based off of Dakka_ClassWeapons (dakka_defs_classweapons.h).
    for (slot = 0; slot < CLASSWEAPONS; slot++)
    {
        // First off, check if we're even going to take anything.
        //  Because Weapon_WeaponIndex's a bit expensive.
        //
        // This is actually a pretty massive optimization.

        takingSomething = false;

        for (class = 0; class < CLASSCOUNT; class++)
        {
            if (class == classWepIndex) { continue; }

            hisWeapon = Dakka_ClassWeapons[slot][class];
            if (stringBlank(hisWeapon)) { continue; }
            if (!CheckInventory(hisWeapon)) { continue; }

            takingSomething = true;
            break;
        }
        
        if (!takingSomething) { continue; }



        // Now that we know we're taking something, actually do the taking.

        int myWeapon = Dakka_ClassWeapons[slot][classWepIndex];
        int justTake = stringBlank(myWeapon);


        // Look at this class weapon slot for everyone, and translate any weapons in this slot.
        for (class = 0; class < CLASSCOUNT; class++)
        {
            if (class == classWepIndex) { continue; }

            hisWeapon = Dakka_ClassWeapons[slot][class];
            if (stringBlank(hisWeapon)) { continue; }
            if (!CheckInventory(hisWeapon)) { continue; }

            // We don't take away the weapons right away, in case one weapon for
            //  class A can translate into multiple weapons for class B.
            Translate_ToTake[takeCount++] = hisWeapon;

            if (!justTake)
            {
                GiveInventory(myWeapon, 1);
                Log(s:"\"", s:hisWeapon, s:"\" -> \"", s:myWeapon, s:"\"");
            }
        }
    }



    // Now take away based off of Dakka_TranslatableGuns_From (dakka_defs_classweapons.h).
    int from, i;

    for (slot = 0; slot < TRANSLATE_GUNS; slot++)
    {
        int checkWep = Dakka_TranslatableGuns_From[slot];
        if (!CheckInventory(checkWep)) { continue; }
        
        // If we're this far, we have checkWep.
        int gaveSomething = false;

        for (i = 0; i < TRANSLATE_MAXTO; i++)
        {
            int itemToGive = Dakka_TranslatableGuns_To[slot][classWepIndex][i];
            if (stringBlank(itemToGive)) { continue; }

            GiveInventory(itemToGive, 1);
            Log(s:"\"", s:checkWep, s:"\" -> \"", s:itemToGive, s:"\" (", d:itemToGive, s:")");
            gaveSomething = true;
        }

        if (gaveSomething)
        {
            Translate_ToTake[takeCount++] = checkWep;
        }
    }

    // Now actually take away weapons.
    for (i = 0; i < takeCount; i++)
    {
        TakeInventory(Translate_ToTake[i], 0x7FFFFFFF);
    }
}
