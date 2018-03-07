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

    int maxtics = oldRound(SWITCHAROO_TICSPER100HP * myhp * 0.01);
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

    int timeAdd = oldROund(KILLSTREAK_TICSPER100HP * myhp * 0.01);
    timeAdd = middle(KILLSTREAK_MINTICS, timeAdd, KILLSTREAK_MAXTICS);

    PlayerKillStreaks[pln][KS_COUNT]        += 1;
    PlayerKillStreaks[pln][KS_TIME]          = Timer();
    PlayerKillStreaks[pln][KS_TIMEADD]       = timeAdd;
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


function int SMult_Brawler(int damagetype)
{
    int i, wep, mult;
    str type;

    for (i = 0; i < KNOWNMELEE; i++)
    {
        mult = Brawler_MeleeValues[i][0];
        
        if (damagetype != 0)
        {
            type = Brawler_KnownMelee[i][1];
            if (!stricmp(type, damagetype)) { return mult; }
        }
        
        if (damagetype == 0 || Brawler_MeleeValues[i][1] == false)
        {
            wep  = Brawler_KnownMelee[i][0];
            if (CheckWeapon(wep)) { return mult; }
        }
    }

    return 0;
}


function int SMult_Air(int pln)
{
    if (pln < 0 || pln >= PLAYERMAX) { return 0; }

    int jumpHeight  = Air_JumpHeight[pln];
    int zdiff       = max(0, jumpHeight - AIR_MINHEIGHT);

    return min(AIR_MAXMULT, FixedMul(zdiff, AIR_MULT));
}


function int SMult_Curveball(int couldsee)
{
    int ret;

    if (couldsee) { ret = 0; }
    else { ret = CURVEBALL_MULT; }

    return ret;
}


function int SMult_Scrapping(int pln, int damagetype)
{
    if (damagetype != 0)
    {
        return cond(!stricmp(damagetype, "Scrap"), SCRAPPING_MULT, 0);
    }
    
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


function int SMult_PointBlank(int mx, int my, int mz, int mradius, int mheight, int px, int py, int pz, int pradius, int pheight)
{
    int mX_closest = middle(px, safeAdd(mx, mradius), safeAdd(mx, -mradius));
    int mY_closest = middle(py, safeAdd(my, mradius), safeAdd(my, -mradius));
    int mZ_closest = middle(pz, safeAdd(mz, mheight), mz);

    int pX_closest = middle(mX_closest, safeAdd(px, pradius), safeAdd(px, -pradius));
    int pY_closest = middle(mY_closest, safeAdd(py, pradius), safeAdd(py, -pradius));
    int pZ_closest = middle(mZ_closest, safeAdd(pz, pheight), pz);

    int dist = distance(mX_closest, mY_closest, mZ_closest, pX_closest, pY_closest, pZ_closest);
    if (dist < POINTBLANK_DIST) { return POINTBLANK_MULT; }

    return 0;
}


function int SMult_Untouchable(int pln, int myhp)
{
    int kills = PlayerKillStreaks[pln][KS_KILLS_SINCELASTHIT];
    int hp    = PlayerKillStreaks[pln][KS_HP_SINCELASTHIT];

    if (kills >= Score_Thresholds[ST_UT_KILLS] || hp >= Score_Thresholds[ST_UT_HP])
    {
        PlayerKillStreaks[pln][KS_KILLS_UNTOUCHABLE] += 1 + (myhp / UNTOUCHABLE_KILLBOOST_HPDIVISOR);
        int multKills = min(PlayerKillStreaks[pln][KS_KILLS_UNTOUCHABLE], UNTOUCHABLE_MAXKILLS);

        return multKills * UNTOUCHABLE_MULT;
    }

    return 0;
}

function void AddUntouchable(int pln, int myhp)
{
    PlayerKillStreaks[pln][KS_KILLS_SINCELASTHIT] += 1;
    PlayerKillStreaks[pln][KS_HP_SINCELASTHIT]    += myhp;
}


function int SMult_Infighter(void)
{
    return INFIGHTER_MULT;
}


function int SMult_Darwin(void)
{
    return DARWIN_MULT;
}