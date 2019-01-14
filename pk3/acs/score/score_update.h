function void Air_UpdateZHeight(void)
{
    int pln = PlayerNumber();

    int ground = onGround(0);

    if (ground)
    {
        Air_FloorHeight[pln]    = GetActorZ(0);
        Air_JumpHeight[pln]     = 0;
    }
    else
    {
        int grav    = GetActorProperty(0, APROP_Gravity);
        int jumpVel = GetActorVelZ(0);

        int curMaxHeight = GetActorZ(0) + HeightFromJumpZ(jumpVel, grav);

        int adjustedMax  = min(curMaxHeight, GetActorCeilingZ(0)) - Air_FloorHeight[pln];

        Air_JumpHeight[pln] = max(Air_JumpHeight[pln], adjustedMax);
    }
}


function void Score_UpdateClient(int pln)
{
    Sender_SetData(pln, S2C_D_SCORE,        Score_GetScore(pln));
    Sender_SetData(pln, S2C_D_DISPLAYSCORE, Score_GetDisplayScore(pln));
    Sender_SetData(pln, S2C_D_GOALSCORE,    Score_Thresholds[ST_FULLHEAL]);
    Sender_SetData(pln, S2C_D_LIVESLEFT,    Score_GetExtraLives(pln));
    Sender_SetData(pln, S2C_D_REWARDCOUNT,  Score_GetRewardCount(pln));

    int i;
    for (i = 0; i < BONUSCOUNT; i++)
    {
        Sender_SetData(pln, S2C_D_BONUSSTART + i, BonusValues[pln][i]);
    }
}


function int GetBonusDisplayTime(int pln)
{
    return middle(35, GetUserCVar(pln, "dakka_cl_bonustime"), 193);
}


function void ClearExpiredRewards(int pln, int t)
{
    t -= GetBonusDisplayTime(pln);
    
    for (int i = 0; i < BONUSCOUNT; i++)
    {
        int ct = BonusSetTime[pln][i];
        
        if (ct >= 0 && ct <= t)
        {
            BonusValues[pln][i]  =  0;
            BonusSetTime[pln][i] = -1;
        }
    }
}


function void ClearAllRewards(int pln)
{
    for (int i = 0; i < BONUSCOUNT; i++)
    {
        BonusValues[pln][i]    =  0;
        BonusSetTime[pln][i] = -1;
    }
}



script "Score_ResetUntouchable" (void)
{
    if (!CheckFlag(0, "NODAMAGE"))
    {
        TakeInventory("DakkaUntouchableKills",  0x7FFFFFFF);
        TakeInventory("DakkaUntouchableHealth", 0x7FFFFFFF);
        TakeInventory("DakkaUntouchableStacks", 0x7FFFFFFF);
    }
}