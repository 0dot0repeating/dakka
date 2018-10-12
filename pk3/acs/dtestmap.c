#library "dtestmap"
#include "zcommon.acs"

// superceded in DAKKTEST itself, but here to avoid unknown script errors
script "DAKKTEST_ReportDamage" (void)
{
    int hp    = GetActorProperty(0, APROP_Health);
    int maxhp = GetActorProperty(0, APROP_SpawnHealth);

    if (hp < maxhp)
    {
        Log(s:"\cd[", d:Timer(), s:"]\c- Dealt ", d:maxhp - hp, s:" damage to target");
    }
    SetActorProperty(0, APROP_Health, maxhp);
}

// do nothing, but avoid unknown script errors
script "DAKKTEST_MonitorTID_Ping" (void)
{
}

script "DAKKTEST_KickAsses" (void)
{
    int myTID_old = ActivatorTID();
    int myTID     = UniqueTID();
    Thing_ChangeTID(0, myTID);
    
    for (int i = 0; i < 64; i++)
    {
        SetActivator(0);

        if (i < 8) { SetActivator(0, AAPTR_PLAYER1 << i); }
        else       { SetActivatorToPlayer(i); } // zandronum only

        if (!(ClassifyActor(0) & ACTOR_WORLD))
        {
            int playerTID_old = ActivatorTID();
            int playerTID     = UniqueTID();
            Thing_ChangeTID(0, playerTID);
            
            SetActivator(myTID);
            Thing_Damage(playerTID, 0x7FFFFFFF, "Exit");
            Thing_ChangeTID(playerTID, playerTID_old);
        }
    }
    
    Thing_ChangeTID(myTID, myTID_old);
}