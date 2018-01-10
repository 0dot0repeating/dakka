script "Dakka_OnKill" (int points)
{
    ACS_NamedExecuteWithResult("Dakka_Score", points);
    
    int spawnHP = GetActorProperty(0, APROP_SpawnHealth);
    SetActivator(0, AAPTR_TARGET);
    Dakka_VampireHeal(spawnHP);
}