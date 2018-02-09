// Uses indexes from pickup/pickup_items_weapons.h.

script "Dakka_SwitchWeps" (int wep1, int wep2) net clientside
{
    str wepname;

    if (wep1 >= 0 && wep1 < WEAPONCOUNT)
    {
        wepname = PKP_KnownGuns[wep1][0];

        if (CheckInventory(wepname) && !CheckWeapon(wepname))
        {
            if (SetWeapon(wepname))
            {
                NamedRequestScriptPuke("Dakka_SwitchingTo", wep1);
                terminate;
            }
        }
    }

    if (wep1 >= 0 && wep1 < WEAPONCOUNT)
    {
        wepname = PKP_KnownGuns[wep2][0];

        if (CheckInventory(wepname) && !CheckWeapon(wepname))
        {
            if (SetWeapon(wepname))
            {
                NamedRequestScriptPuke("Dakka_SwitchingTo", wep2);
                terminate;
            }
        }
    }
}

script "Dakka_SwitchingTo" (int wep, int t) net
{
    Delay(t);
    
    if (wep >= 0 && wep < WEAPONCOUNT)
    {
        str curwep     = GetWeapon();
        str newwep     = PKP_KnownGuns[wep][0];
        if (stricmp(curwep, newwep)) { SetWeapon(newwep); }
    }
}