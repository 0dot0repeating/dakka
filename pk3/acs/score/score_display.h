#define OSCORE_POINTS       0
#define OSCORE_GOALPOINTS   1
#define OSCORE_FIRSTDRAW    2
#define OSCORE_NOREWARD     3
#define OSCORE_HIDESCORE    4
#define OSCORE_NEXTISLIFE   5

int Score_OldVals[6][PLAYERMAX];

function void Score_Update(int pln)
{
    int oldPoints       = Score_OldVals[OSCORE_POINTS][pln];
    int oldGoalPoints   = Score_OldVals[OSCORE_GOALPOINTS][pln];
    int oldNoReward     = Score_OldVals[OSCORE_NOREWARD][pln];
    int oldHideScore    = Score_OldVals[OSCORE_HIDESCORE][pln];
    int oldNextIsLife   = Score_OldVals[OSCORE_NEXTISLIFE][pln];
    int first           = Score_OldVals[OSCORE_FIRSTDRAW][pln];

    int points          = SToC_ClientData[pln][S2C_D_SCORE];
    int goalpoints      = SToC_ClientData[pln][S2C_D_GOALSCORE];
    int displayPoints   = SToC_ClientData[pln][S2C_D_DISPLAYSCORE];
    int hideScore       = GetUserCVar(pln, "dakka_cl_hidescore");
    
    int rewardTypes = GetCVar("dakka_score_rewardtypes");
    int rewardCount = SToC_ClientData[pln][S2C_D_REWARDCOUNT];
    int nextIsLife  = false;
    int noReward    = GetUserCVar(pln, "dakka_cl_noscorerewards");
    
    switch (rewardtypes)
    {
      default:
        nextIsLife = rewardCount % 2 == 1;
        break;
      
      case 1:
        nextIsLife = rewardCount % 2 == 0;
        break;
      
      case 2:
        break;
      
      case 3:
        nextIsLife = true;
        break;
      
      case 4:
        noReward = true;
        break;
    }

    if (!first || (points != oldPoints) || (goalPoints != oldGoalPoints)
     || (noReward != oldNoReward) || (oldHideScore != hideScore) || (oldNextIsLife != nextIsLife))
    {
        Score_Draw(points, goalpoints, displayPoints, hideScore, noReward, nextIsLife);
        Score_OldVals[OSCORE_FIRSTDRAW][pln] = true;
    }
    
    int lives = SToC_ClientData[pln][S2C_D_LIVESLEFT];
    Score_DrawBonuses(pln, hideScore);
    Score_DrawLives(lives, hideScore);

    Score_OldVals[OSCORE_POINTS][pln]     = points;
    Score_OldVals[OSCORE_GOALPOINTS][pln] = goalpoints;
    Score_OldVals[OSCORE_NOREWARD][pln]   = noReward;
    Score_OldVals[OSCORE_HIDESCORE][pln]  = hideScore;
    Score_OldVals[OSCORE_NEXTISLIFE][pln] = nextIsLife;
}


#define BARSTEPS        120
#define BAROFFSET       ((BARSTEPS / 2) << 16)
#define BARINCREMENT    12

function void Score_Draw(int curPoints, int goalPoints, int displayPoints, int hideScore, int noScoreRewards, int nextIsLife)
{
    int i;

    if (hideScore)
    {
        HudMessage(s:""; HUDMSG_PLAIN, 24200, 0,0,0,0);
    }
    else
    {
        SetFont("DAKKAFON");
        SetHudSize(640, 480, 1);
        HudMessage(s:"Score: \c[DScore_Gold]", d:displayPoints;
                    HUDMSG_PLAIN | HUDMSG_COLORSTRING, 24200, "DScore_White", 520.4, 64.2, 0);
    }

    if (noScoreRewards || hideScore)
    {
        HudMessage(s:""; HUDMSG_PLAIN, 24201, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24202, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24203, 0,0,0,0);
    }
    else
    {
        str barBackground, barForeground;

        if (nextIsLife)
        {
            barBackground  = "SCOREBG2";
            barForeground  = "SCOREBR2";
        }
        else
        {
            barBackground  = "SCOREBG1";
            barForeground  = "SCOREBR1";
        }

        SetHudSize(640, 480, 1);

        SetFont(barBackground);
        HudMessage(s:"A"; HUDMSG_PLAIN, 24203, CR_UNTRANSLATED, 520.4, 70.0, 0);

        SetFont("SCOREBKT");
        HudMessage(s:"A"; HUDMSG_PLAIN, 24201, CR_UNTRANSLATED, 520.4, 70.0, 0);
        
        if (goalPoints > 0)
        {
            int pointstep = goalPoints / BARSTEPS;
            int barpoints = curPoints % goalPoints;
            
            int barLeft = 520 - (BARSTEPS / 2);
            SetHudClipRect(barLeft, 0, barpoints / pointstep, 480);
            
            SetFont(barForeground);
            HudMessage(s:"A"; HUDMSG_PLAIN, 24202, CR_UNTRANSLATED, 520.4, 70.0, 0);
            SetHudClipRect(0,0,0,0,0);
        }
    }
}


#define LIVES_MAXDRAW   10

function void Score_DrawLives(int lives, int hideScore)
{
    int i;
    
    if (hideScore)
    {
        for (i = 0; i < LIVES_MAXDRAW; i++)
        {
            HudMessage(s:""; HUDMSG_PLAIN, 25501 + i, 0,0,0,0);
        }
        
        return;
    }
    
    SetHudSize(640, 480, 1);

    int lifeFont;

    switch ((Timer() / 4) % 4)
    {
        default: case 0: lifeFont = "HUD_LIF1"; break;
        case 1: lifeFont = "HUD_LIF2"; break;
        case 2: lifeFont = "HUD_LIF3"; break;
        case 3: lifeFont = "HUD_LIF4"; break;
    }

    SetFont(lifeFont);


    int drawLives = min(LIVES_MAXDRAW, lives);

    int borderLeft = itof(520 - (6 * (drawLives - 1)));

    for (i = 0; i < LIVES_MAXDRAW; i++)
    {
        if (i >= drawLives)
        {
            HudMessage(s:""; HUDMSG_PLAIN, 25501 + i, 0,0,0,0);
        }
        else
        {
            HudMessage(s:"A"; HUDMSG_PLAIN, 25501 + i, CR_UNTRANSLATED, 0.4 + borderLeft + (12.0 * i), 50.2, 0);
        }
    }
}



int Tmp_BonusDisplay[BONUSCOUNT];

function void Score_DrawBonuses(int pln, int hideScore)
{
    if (pln < 0 || pln >= PLAYERMAX) { return; }
    SetHudSize(640, 480, 1);

    int i, display, score, name, color, offset = 0;
    int redisplay = false;
    int bonus;

    for (i = 0; i < BONUSCOUNT; i++)
    {
        bonus = SToC_ClientData[pln][S2C_D_BONUSSTART + i];
        display = (bonus > Bonus_LastSeen[pln][i]);

        Tmp_BonusDisplay[i] = display;
        redisplay |= display;

        Bonus_LastSeen[pln][i] = bonus;
    }

    // Check hidescore here so that turning the cvar on mid-game doesn't display
    // any score changes that happened while it was off.
    if (hideScore || !redisplay) { return; }
    
    int displaytime = itofdiv(GetUserCVar(pln, "dakka_cl_bonustime"), 35) - 0.5;

    for (i = 0; i < BONUSCOUNT; i++)
    {
        bonus = SToC_ClientData[pln][S2C_D_BONUSSTART + i];

        name  = BonusNames[i];
        color = BonusColors[i];
        score = bonus;

        if (score <= 0) { continue; }

        offset += 1;

        if (!Tmp_BonusDisplay[i]) { continue; }

        SetFont("DAKKAFN2");

        HudMessage(s:"+", d:score; HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24409 + (offset * 2), color,
                        455.1, 82.0 + (13.0 * (offset-1)), displaytime, 0.5);

        HudMessage(s:name; HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24410 + (offset * 2), "DScore_White",
                        508.1, 82.0 + (13.0 * (offset-1)), displaytime, 0.5);
    }
}
