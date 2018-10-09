int Score_OldScores[PLAYERMAX];

function void Score_Update(int pln)
{
    int oldPoints       = Score_OldScores[pln] - 1;
    int points          = SToC_ClientData[pln][S2C_D_SCORE];
    int goalpoints      = SToC_ClientData[pln][S2C_D_GOALSCORE];
    int displayPoints   = SToC_ClientData[pln][S2C_D_DISPLAYSCORE];
    int lives           = SToC_ClientData[pln][S2C_D_LIVESLEFT];
    int hideScore       = GetUserCVar(pln, "dakka_cl_hidescore") | (GetCVar("screenblocks") == 12);
    int hideBonuses     = (GetUserCVar(pln, "dakka_cl_bonustime") == 0) | hideScore;
    
    int rewardTypes = GetCVar("dakka_score_rewardtypes");
    int rewardCount = SToC_ClientData[pln][S2C_D_REWARDCOUNT];
    int nextIsLife  = false;
    int flash       = GetUserCVar(pln, "dakka_cl_flashscore") && (oldPoints >= 0) && (points > oldPoints);
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
    
    Score_Draw(pln, points, goalpoints, displayPoints, hideScore, noReward, nextIsLife, flash);
    Score_DrawLives(pln, lives, hideScore);
    Score_DrawBonuses(pln, hideBonuses);
    
    Score_OldScores[pln] = points + 1;
}


function int Score_ScaleRes(int res, int scale)
{
    // arbitrary numbers, don't really care
    scale = middle(25, scale, 400);
    return oldRound(itofDiv(res * 100, scale));
}


function int Score_ScaledCoord(int pos, int range, int padding, int downshift)
{
    int saferange = range - cond(range > padding, padding, 0);
    return FixedMul(saferange, itofDiv(middle(0, pos, 100), 100)) + ((range - saferange) >> 1) - (downshift >> 1);
}


#define BARSTEPS        120
#define BAROFFSET       ((BARSTEPS / 2) << 16)
#define BARINCREMENT    12

#define SCORE_SCREENX    640
#define SCORE_SCREENY    480

#define BAR_APPROXWIDTH  140
#define BAR_APPROXHEIGHT 40

function void Score_Draw(int pln, int curPoints, int goalPoints, int displayPoints, int hideScore, int noScoreRewards, int nextIsLife, int flash)
{
    int i;
    
    int d = GetUserCVar(pln, "dakka_cl_scorescale");
    int screenwidth  = Score_ScaleRes(SCORE_SCREENX, d);
    int screenheight = Score_ScaleRes(SCORE_SCREENY, d);
    int widewidth    = FixedMul(screenHeight, itofDiv(GetScreenWidth(), GetScreenHeight()));
    
    int centerX  = Score_ScaledCoord(GetUserCVar(pln, "dakka_cl_scorex"), wideWidth,    BAR_APPROXWIDTH,  widewidth - screenWidth);
    int centerY  = Score_ScaledCoord(GetUserCVar(pln, "dakka_cl_scorey"), screenheight, BAR_APPROXHEIGHT, 0);
    int centerXf = setFraction(itof(centerX), 0.4);

    
    if (hideScore)
    {
        HudMessage(s:""; HUDMSG_PLAIN, 24200, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24199, 0,0,0,0);
    }
    else
    {
        int scoreYf = setFraction(itof(centerY + 7), 0.2);
        SetHudSize(screenwidth, screenheight, 1);
        
        SetFont("DAKKAFON");
        HudMessage(s:"Score: \c[DScore_Gold]", d:displayPoints;
                    HUDMSG_PLAIN | HUDMSG_COLORSTRING, 24200, "DScore_White", centerXf, scoreYf, 0);
        
        if (flash)
        {
            HudMessage(s:"Score: \c[DScore_GoldFlash]", d:displayPoints;
                        HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24199, "DScore_WhiteFlash", centerXf, scoreYf, 0.05, 0.15);
        }
    }

    if (noScoreRewards || hideScore)
    {
        HudMessage(s:""; HUDMSG_PLAIN, 24201, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24202, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24203, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24204, 0,0,0,0);
        HudMessage(s:""; HUDMSG_PLAIN, 24205, 0,0,0,0);
    }
    else
    {
        str barBackground, barForeground, flashBackground, flashForeground;

        if (nextIsLife)
        {
            barBackground    = "SCOREBG2";
            flashBackground  = "SCOREBG4";
            barForeground    = "SCOREBR2";
            flashForeground  = "SCOREBR4";
        }
        else
        {
            barBackground    = "SCOREBG1";
            flashBackground  = "SCOREBG3";
            barForeground    = "SCOREBR1";
            flashForeground  = "SCOREBR3";
        }

        
        int barYf = itof(centerY + 13);
        SetHudSize(screenwidth, screenheight, 1);

        SetFont(barBackground);
        HudMessage(s:"A"; HUDMSG_PLAIN, 24205, CR_UNTRANSLATED, centerXf, barYf, 0);
            
        if (flash)
        {
            SetFont(flashBackground);
            HudMessage(s:"A"; HUDMSG_FADEOUT, 24204, CR_UNTRANSLATED, centerXf, barYf, 0.05, 0.15);
        }

        SetFont("SCOREBKT");
        HudMessage(s:"A"; HUDMSG_PLAIN, 24201, CR_UNTRANSLATED, centerXf, barYf, 0);
        
        if (goalPoints > 0)
        {
            int pointstep = goalPoints / BARSTEPS;
            int barpoints = curPoints % goalPoints;
            
            int barLeft = centerX - (BARSTEPS / 2);
            SetHudClipRect(barLeft, 0, barpoints / pointstep, screenheight);
            
            SetFont(barForeground);
            HudMessage(s:"A"; HUDMSG_PLAIN, 24203, CR_UNTRANSLATED, centerXf, barYf, 0);
            
            if (flash)
            {
                SetFont(flashForeground);
                HudMessage(s:"A"; HUDMSG_FADEOUT, 24202, CR_UNTRANSLATED, centerXf, barYf, 0.05, 0.15);
            }
            
            SetHudClipRect(0,0,0,0,0);
        }
    }
}


#define LIVES_MAXDRAW   10

function void Score_DrawLives(int pln, int lives, int hideScore)
{
    int i;
    
    int d = GetUserCVar(pln, "dakka_cl_scorescale");
    int screenwidth  = Score_ScaleRes(SCORE_SCREENX, d);
    int screenheight = Score_ScaleRes(SCORE_SCREENY, d);
    int widewidth    = FixedMul(screenHeight, itofDiv(GetScreenWidth(), GetScreenHeight()));
    
    int centerX  = Score_ScaledCoord(GetUserCVar(pln, "dakka_cl_scorex"), wideWidth,    BAR_APPROXWIDTH,  widewidth - screenWidth);
    int centerY  = Score_ScaledCoord(GetUserCVar(pln, "dakka_cl_scorey"), screenheight, BAR_APPROXHEIGHT, 0);
    int centerXf = setFraction(itof(centerX), 0.4);
    
    if (hideScore)
    {
        for (i = 0; i < LIVES_MAXDRAW; i++)
        {
            HudMessage(s:""; HUDMSG_PLAIN, 25501 + i, 0,0,0,0);
        }
        
        return;
    }
    
    SetHudSize(screenwidth, screenheight, 1);

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

    int borderLeft = itof(centerX - (6 * (drawLives - 1))) + 0.4;
    int livesTop   = itof(centerY - 12);

    for (i = 0; i < LIVES_MAXDRAW; i++)
    {
        if (i >= drawLives)
        {
            HudMessage(s:""; HUDMSG_PLAIN, 25501 + i, 0,0,0,0);
        }
        else
        {
            HudMessage(s:"A"; HUDMSG_PLAIN, 25501 + i, CR_UNTRANSLATED, setFraction(borderLeft + (12.0 * i), 0.4), livesTop, 0);
        }
    }
}



int Tmp_BonusDisplay[BONUSCOUNT];

function void Score_DrawBonuses(int pln, int hideScore)
{
    if (pln < 0 || pln >= PLAYERMAX) { return; }
    
    int d = GetUserCVar(pln, "dakka_cl_scorescale");
    int screenwidth  = Score_ScaleRes(SCORE_SCREENX, d);
    int screenheight = Score_ScaleRes(SCORE_SCREENY, d);
    int widewidth    = FixedMul(screenHeight, itofDiv(GetScreenWidth(), GetScreenHeight()));
    
    int centerX  = Score_ScaledCoord(GetUserCVar(pln, "dakka_cl_scorex"), wideWidth,    BAR_APPROXWIDTH,  widewidth - screenWidth);
    int centerY  = Score_ScaledCoord(GetUserCVar(pln, "dakka_cl_scorey"), screenheight, BAR_APPROXHEIGHT, 0);
    int centerXf = setFraction(itof(centerX), 0.4);
    int bonusYf  = itof(centerY + 26);
    
    SetHudSize(screenwidth, screenheight, 1);

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
    
    if (hidescore)
    {
        for (i = 0; i < BONUSCOUNT; i++)
        {
            HudMessage(s:""; HUDMSG_PLAIN, 24409 + (i*2), 0,0,0,0);
            HudMessage(s:""; HUDMSG_PLAIN, 24410 + (i*2), 0,0,0,0);
        }
        
        return;
    }

    if (!redisplay) { return; }
    
    int displaytime = itofdiv(GetBonusDisplayTime(pln), 35) - 0.5;
    SetFont("DAKKAFN2");

    for (i = 0; i < BONUSCOUNT; i++)
    {
        bonus = SToC_ClientData[pln][S2C_D_BONUSSTART + i];
        name  = BonusNames[i];
        color = BonusColors[i];

        if (bonus <= 0) { continue; }
        
        offset++;

        if (!Tmp_BonusDisplay[i]) { continue; }

        HudMessage(s:"+", d:bonus; HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24409 + (offset * 2), color,
                        setFraction(centerXf - 55.0, 0.1), bonusYf + (13.0 * (offset-1)), displaytime, 0.5);

        HudMessage(s:name; HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24410 + (offset * 2), "DScore_White",
                        setFraction(centerXf - 12.0, 0.1), bonusYf + (13.0 * (offset-1)), displaytime, 0.5);
    }
}
