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


// Sometimes, the client thinks he switched weapons, and the server disagrees.
//  Normally, this only gets resolved when the player tries to shoot, and finds
//  that nothing is actually coming out of the barrel until a short bit later.
//  This resolves the issue as soon as possible.

script "Dakka_SyncHeldWeapon" enter clientside
{
    if (IsServer) { terminate; }
    SetHudSize(640, 480, true);
    
    int lastWepIndex;
    int curWepIndex;
    int i, first = true;
    
    while (true)
    {
        str wep = GetWeapon();
        curWepIndex = -1;
        
        for (i = 0; i < WEAPONCOUNT; i++)
        {
            if (!stricmp(PKP_KnownGuns[i][0], wep))
            {
                curWepIndex = i;
                break;
            }
        }
        
        if (!first && (curWepIndex != lastWepIndex))
        {
            NamedRequestScriptPuke("Dakka_SwitchingTo", curWepIndex, 1);
        }
        
        lastWepIndex = curWepIndex;
        first = false;
        Delay(1);
    }
}