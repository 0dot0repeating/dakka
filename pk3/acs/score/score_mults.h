function int SMult_Base(void)
{
    return GetActorProperty(0, APROP_SpawnHealth);
}

int Tmp_FireModesUsed[SCOREFIREMODES];

function int SMult_WeaponSwitch(int pln, int myhp)
{
    if (pln < 0 || pln >= PLAYERMAX) { return 0.0; }

    int classNum = Pickup_ClassNumber(0);

    // Can't use Switcharoo if we don't update Switcharoo
    if (classNum == -1 || !SCORE_UpdatesSwitcharoo[classNum]) { return 0.0; }

    int maxtics = round(SWITCHAROO_TICSPER100HP * myhp * 0.01);
    maxtics = middle(SWITCHAROO_MINTICS, maxtics, SWITCHAROO_MAXTICS);
    int cutoff = Timer() - maxtics;
    
    int wepsUsed        = 0;
    int fireModesUsed   = 0;
    int s, f, i;
    int used;

    for (i = 0; i < SCOREFIREMODES; i++)
    {
        Tmp_FireModesUsed[i] = 0;
    }

    for (s = 0; s < SCOREWEPS; s++)
    {
        used  = 0;

        for (f = 0; f < SCOREFIREMODES; f++)
        {
            i = PlayerWeapons_LastFired[pln][s][f];
            if (i == 0) { continue; }

            if (cutoff <= i)
            {
                used = 1;
                Tmp_FireModesUsed[f] = 1;
            }
        }

        if (used) { wepsUsed += 1; }
    }

    for (i = 0; i < SCOREFIREMODES; i++)
    {
        if (Tmp_FireModesUsed[i]) { fireModesUsed++; }
    }

    if (wepsUsed > 1) { return 0.15; }
    if (fireModesUsed > 1) { return 0.075; }
    return 0.0;
}

function int SMult_Killstreak(int pln)
{
    if (pln < 0 || pln >= PLAYERMAX) { return 0.0; }

    int curtime = Timer();
    int neededTime = PlayerKillStreaks[pln][KS_TIME] + PlayerKillStreaks[pln][KS_TIMEADD];

    if (curtime > neededTime)
    {
        PlayerKillStreaks[pln][KS_COUNT] = 0;
        return 0.0;
    }

    return min(KILLSTREAK_MULTMAX, KILLSTREAK_MULTINC * PlayerKillStreaks[pln][KS_COUNT]);
}

function int AddKillstreak(int pln, int myhp)
{
    if (pln < 0 || pln >= PLAYERMAX) { return 0; }

    int timeAdd = round(KILLSTREAK_TICSPER100HP * myhp * 0.01);
    timeAdd = middle(KILLSTREAK_MINTICS, timeAdd, KILLSTREAK_MAXTICS);

    PlayerKillStreaks[pln][KS_COUNT]   += 1;
    PlayerKillStreaks[pln][KS_TIME]     = Timer();
    PlayerKillStreaks[pln][KS_TIMEADD]  = timeAdd;
    return 0;
}


function int SMult_BoneDry(void)
{
    int hp = GetActorProperty(0, APROP_Health);
    if (hp < 0) { return 0; }

    int hpUnder = max(0, (BONEDRY_MAXHP - hp) + 1);

    return BONEDRY_MULTPERHP * hpUnder;
}

function int SMult_SoreLoser(void)
{
    int hp = GetActorProperty(0, APROP_Health);

    if (hp < 0) { return 3.0; }
    return 0;
}

function int SMult_Brawler(void)
{
    int i, wep, mult;

    for (i = 0; i < KNOWNMELEE; i++)
    {
        wep  = Brawler_KnownMelee[i];
        mult = Brawler_MeleeMults[i];
        if (CheckWeapon(wep)) { return mult; }
    }

    return 0;
}

function int SMult_Air(int pln)
{
    if (pln < 0 || pln >= PLAYERMAX) { return 0; }

    int zvel  = PlayerZHeight[pln];
    int zdiff = max(0, zvel - AIR_MINHEIGHT);

    return min(AIR_MAXMULT, FixedMul(zdiff, AIR_MULT));
}

function int SMult_Curveball(int couldsee)
{
    int ret;

    if (couldsee) { ret = 0; }
    else { ret = CURVEBALL_MULT; }
    
    return ret;
}

function int SMult_Scrapping(int pln)
{
    if (pln < 0 || pln >= PLAYERMAX) { return 0.0; }
    
    int s, f, i;

    int latestTic  = -1;
    int latestWep  = -1;
    int latestMode = -1;

    for (i = 0; i < SCOREFIREMODES; i++)
    {
        Tmp_FireModesUsed[i] = 0;
    }

    for (s = 0; s < SCOREWEPS; s++)
    {
        for (f = 0; f < SCOREFIREMODES; f++)
        {
            i = PlayerWeapons_LastFired[pln][s][f];
            
            if (i > latestTic)
            {
                latestTic  = i;
                latestWep  = s;
                latestMode = f;
            }
        }
    }

    if (latestWep == S_SCRAPPER) { return SCRAPPING_MULT; }
    return 0;
}

function int SMult_PointBlank(int mx, int my, int mz, int px, int py, int pz)
{
    int dist = distance(mx, my, mz, px, py, pz);
    if (dist < POINTBLANK_DIST) { return POINTBLANK_MULT; }

    return 0;
}




function int SMult_Efficiency(int pln)
{
    int lastFireTime  = PlayerKills_LastFired[pln][0];
    int lastFireKills = PlayerKills_LastFired[pln][1];

    int classNum = Pickup_ClassNumber(0);

    // Switcharoo's updating script also resets PlayerKills_LastFired. If it's
    //  not being called, we have to assume everything on a different tic does
    //  not qualify for One Stone.
    if (classNum == -1 || !SCORE_UpdatesSwitcharoo[classNum])
    {
        if (lastFireTime != Timer())
        {
            PlayerKills_LastFired[pln][1] = 0;
            lastFireKills = 0;
        }
    }

    int killMult = EFFICIENCY_MULTINC * lastFireKills;

    PlayerKills_LastFired[pln][0]  = Timer();
    PlayerKills_LastFired[pln][1] += 1;

    return min(EFFICIENCY_MULTMAX, killMult);
}



function int SMult_Infighter(void)
{
    return INFIGHTER_MULT;
}
