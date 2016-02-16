function void Score_Update(int pln)
{
    int points      = SToC_ClientData[pln][S2C_D_SCORE];
    int goalpoints  = SToC_ClientData[pln][S2C_D_GOALSCORE];

    Score_Draw(points, goalpoints);
    Score_DrawBonuses(pln);
}

function void Score_Draw(int curPoints, int goalPoints)
{
    SetFont("DAKKAFON");
    SetHudSize(560, 420, 1);
    HudMessage(s:"Score: \c[DScore_Gold]", d:curPoints;
                HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24200, "DScore_White", 455.4, 55.2, 0.5, 0.25);

    SetHudSize(480, 360, 1);

    SetFont("SCOREBAR");
    HudMessage(s:"A"; HUDMSG_FADEOUT, 24401, CR_UNTRANSLATED, 390.4, 52.0, 0.5, 0.25);

    if (goalPoints > 0)
    {
        int pointstep = goalPoints / 100;
        int barpoints = curPoints % goalPoints;

        for (int i = 0; i < 100; i++)
        {
            int barGraphic = "POINTBR1";
            int barChar    = "";
            int i10        = i % 10;

            if (i10 == 0 && i != 0)
            {
                barGraphic = "POINTBR3";
            }

            if (i10 == 9 && i != 99)
            {
                barGraphic = "POINTBR2";
            }

            if (barpoints > pointstep * (i + 1))
            {
                SetFont(barGraphic);
                HudMessage(s:"A"; HUDMSG_PLAIN, 24201 + i, CR_UNTRANSLATED, 340.1 + (1.0 * i), 52.0, 1873);
                // 1873 = (1.0 / 35)+1 = 1 tic
            }
        }
    }
}



int Tmp_BonusDisplay[BONUSCOUNT];

function void Score_DrawBonuses(int pln)
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

        if (display) { redisplay = true; }

        Bonus_LastSeen[pln][i] = bonus;
    }

    if (!redisplay) { return; }

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
                        455.1, 82.0 + (13.0 * (offset-1)), 0.5, 0.5);

        HudMessage(s:name; HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 24410 + (offset * 2), "DScore_White", 
                        508.1, 82.0 + (13.0 * (offset-1)), 0.5, 0.5);

    }
}
