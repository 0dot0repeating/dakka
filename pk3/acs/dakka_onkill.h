script "Dakka_OnKill" kill
{
    if(!CheckFlag(0, "ISMONSTER"))
    {
        terminate;
    }
    
    str monName   = GetActorClass(0);
    str deathType = GetActorProperty(0, APROP_DamageType);
    
    int points  = ACS_NamedExecuteWithResult("Score_Lookup", monName);
    ACS_NamedExecuteWithResult("Dakka_Score", points, deathType);

    int spawnHP = GetActorProperty(0, APROP_SpawnHealth);
    SetActivator(0, AAPTR_TARGET);
    ACS_NamedExecuteWithResult("Dakka_VampireHeal", spawnHP);
}