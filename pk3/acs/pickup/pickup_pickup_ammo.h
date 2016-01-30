function int Ammo_AmmoIndex(int ammo)
{
    int i;

    for (i = 0; i < AMMOCOUNT; i++)
    {
        if (!strcmp_i(PKP_KnownAmmo[i], ammo))
        {
            return i;
        }
    }

    return -1;
}

function void Ammo_PickupAmmoName(int ammo, int count, int dropped)
{
    int index = Ammo_AmmoIndex(ammo);

    if (index != -1)
    {
        Ammo_PickupAmmo(index, count, dropped);
    }
}

function void Ammo_PickupAmmo(int ammoIndex, int count, int dropped)
{
    int ammo    = PKP_KnownAmmo[ammoIndex];
    int ammoMax = GetAmmoCapacity(ammo);

    if (dropped)
    {
        SetAmmoCapacity(ammo, ammoMax*2);
    }

    // Can't just use ammoCount because ACS is case-insensitive for some
    //  stupid dumbass reason.
    int oldAmmoCount = CheckInventory(ammo);
    GiveInventory(ammo, count);

    int halfAmmo = (CheckInventory(ammo) - oldAmmoCount + 1) / 2;

    // Special case so we get at least one ammo from a dropped pickup.
    if (count == 1) { halfAmmo = 0; }

    if (dropped)
    {
        if (GetCVar("sv_unlimited_pickup"))
        {
            TakeInventory(ammo, halfAmmo);
        }
        else
        {
            TakeInventory(ammo, max(halfAmmo, CheckInventory(ammo) - ammoMax));
        }
        SetAmmoCapacity(ammo, ammoMax);
    }

    int newAmmoCount = CheckInventory(ammo);

    if (newAmmoCount > oldAmmoCount)
    {
        PKP_ReturnArray[PARRAY_SUCCEEDED] = true;
        PKP_ReturnArray[PARRAY_CONSUME]   = true;
    }
}
