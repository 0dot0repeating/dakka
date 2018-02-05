script "Dakka_OnKill" KILL
{
	if(!CheckFlag(0, "ISMONSTER"))
	{
		terminate;
	}

    str monName = GetActorClass(0);
    int points  = ACS_NamedExecuteWithResult("Score_Lookup", monName);
    ACS_NamedExecuteWithResult("Dakka_Score", points);

    int spawnHP = GetActorProperty(0, APROP_SpawnHealth);
    SetActivator(0, AAPTR_TARGET);
    ACS_NamedExecuteWithResult("Dakka_VampireHeal", spawnHP);
}