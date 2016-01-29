script DAKKA_UPDATE_SWITCHAROO (int slot, int firemode, int noUpdateEfficiency)
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

    if (!noUpdateEfficiency)
    {
        PlayerKills_LastFired[pln][0] = Timer() + 1;
        PlayerKills_LastFired[pln][1] = 0;
    }
}
