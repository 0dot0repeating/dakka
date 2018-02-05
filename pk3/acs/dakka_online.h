script "Dakka_AmmoSync" (int ammoindex)
{
    if (!IsServer) { terminate; }
    if (!(IsZandronum && ConsolePlayerNumber() == -1)) { terminate; }
    if (ammoindex < 0 || ammoindex >= AMMOCOUNT) { terminate; }

    str ammotype = PKP_KnownAmmo[ammoindex];

    if (CheckInventory(ammotype) == 0)
    {
        GiveAmmo(ammotype, 1);
        TakeInventory(ammotype, 1);
    }
    else
    {
        TakeInventory(ammotype, 1);
        GiveAmmo(ammotype, 1);
    }
}

// This is purely to avoid ammo desyncs online.
//
// Zandronum clients predict when they're going to use ammo, and take it away
// themselves. The server therefore never bothers to tell them they used ammo,
// which usually works out... but with Dakka, it doesn't. At all. So we gotta
// handle this manually.

script "Dakka_UseAmmo" (int ammoindex, int count)
{
    if (!IsServer) { terminate; }
    if (ammoindex < 0 || ammoindex >= AMMOCOUNT) { terminate; }
    if (GetCVar("sv_infiniteammo") || CheckInventory("PowerInfiniteAmmo")) { terminate; }

    str ammotype = PKP_KnownAmmo[ammoindex];
    TakeInventory(ammotype, count);
}