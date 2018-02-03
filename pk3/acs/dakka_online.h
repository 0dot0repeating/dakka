script "Dakka_AmmoSync" (int ammoindex)
{
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