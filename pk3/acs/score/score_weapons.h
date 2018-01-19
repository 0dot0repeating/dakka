script "Dakka_Switcharoo" (int slot, int firemode)
{
    int pln = PlayerNumber();
    if (pln == -1) // called from projectile
    {
        SetActivatorToTarget(0);
        pln = PlayerNumber();
    }

    if (pln == -1
     || slot < 0 || slot >= SCOREWEPS
     || firemode < 0 || firemode >= SCOREFIREMODES) { terminate; }

    PlayerWeapons_LastFired[pln][slot][firemode] = Timer() + 1;
}
