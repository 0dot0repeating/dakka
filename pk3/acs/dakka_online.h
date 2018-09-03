// Sometimes, the client thinks he switched weapons, and the server disagrees.
//  Normally, this only gets resolved when the player tries to shoot, and finds
//  that nothing is actually coming out of the barrel until a short bit later.
//  This resolves the issue as soon as possible.

script "Dakka_SyncHeldWeapon" enter clientside
{
    if (IsServer || !IsZandronum) { terminate; }
    
    int  pln = PlayerNumber();
    int cpln = ConsolePlayerNumber();
    if (pln != cpln) { terminate; }
    
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