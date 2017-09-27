// Uses indexes from pickup/pickup_items_weapons.h.

script "Dakka_SwitchWeps" (int wep1, int wep2) net clientside
{
    int wepname;
    
    if (wep1 >= 0 && wep1 < WEAPONCOUNT)
    {
        wepname = PKP_KnownGuns[wep1][0];
        
        if (CheckInventory(wepname) && !CheckWeapon(wepname))
        {
            if (SetWeapon(wepname))
            {
                NamedRequestScriptPuke("Dakka_SwitchingTo", wep1, 0,0,0);
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
                NamedRequestScriptPuke("Dakka_SwitchingTo", wep2, 0,0,0);
                terminate;
            }
        }
    }
}

script "Dakka_SwitchingTo" (int wep) net
{
    if (wep >= 0 && wep < WEAPONCOUNT)
    {
        SetWeapon(PKP_KnownGuns[wep][0]);
    }
}