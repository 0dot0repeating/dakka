function void Dakka_TranslateGuns(void)
{
    int slot, class;
    int classNum = Pickup_ClassNumber(0);
    int classWepIndex = classNum + 1;

    int hisWeapon, takingSomething;

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

            hisWeapon = PKP_ClassWeapons[slot][class];
            if (hisWeapon == 0 || StrLen(hisWeapon) == 0) { continue; }
            if (!CheckInventory(hisWeapon)) { continue; }

            takingSomething = true;
            break;
        }
        
        if (!takingSomething) { continue; }

        // Now that we know we're taking something, actually do the taking.

        int myWeapon = PKP_ClassWeapons[slot][classWepIndex];
        int justTake = (myWeapon == 0 || StrLen(myWeapon) == 0);

        int myWepIndex, myAmmo1Count, myAmmo2Count;
        int myAmmo1 = "";
        int myAmmo2 = "";

        if (!justTake)
        {
            myWepIndex = Weapon_WeaponIndex(myWeapon);

            if (myWepIndex != -1)
            {
                myAmmo1 = PKP_KnownGuns[myWepIndex][WEP_AMMO1];
                myAmmo2 = PKP_KnownGuns[myWepIndex][WEP_AMMO2];

                if (StrLen(myAmmo1) > 0) { myAmmo1Count = CheckInventory(myAmmo1); }
                if (StrLen(myAmmo2) > 0) { myAmmo2Count = CheckInventory(myAmmo2); }
            }
        }


        for (class = 0; class < CLASSCOUNT; class++)
        {
            if (class == classWepIndex) { continue; }

            hisWeapon = PKP_ClassWeapons[slot][class];
            if (hisWeapon == 0 || StrLen(hisWeapon) == 0) { continue; }

            if (!CheckInventory(hisWeapon)) { continue; }

            TakeInventory(hisWeapon, 0x7FFFFFFF);

            if (!justTake)
            {
                GiveInventory(myWeapon, 1);
                Log(s:"Replaced weapon \"", s:hisWeapon, s:"\" with \"", s:myWeapon, s:"\"");
            }
            else
            {
                Log(s:"Took weapon \"", s:hisWeapon, s:"\"");
            }
        }

        if (StrLen(myAmmo1) > 0) { TakeInventory(myAmmo1, CheckInventory(myAmmo1) - myAmmo1Count); }
        if (StrLen(myAmmo2) > 0) { TakeInventory(myAmmo2, CheckInventory(myAmmo2) - myAmmo2Count); }
    }
}
