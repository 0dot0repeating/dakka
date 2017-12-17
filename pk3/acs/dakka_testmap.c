#library "dakka_testmap"
#include "zcommon.acs"

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