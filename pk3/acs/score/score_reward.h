script "Score_Award" (int scoreHeals)
{
    int i;
    int pln = PlayerNumber();
    int newLives = 0;
    int rewardtypes = GetCVar("dakka_score_rewardtypes");
    
    LocalAmbientSound("dakka/pointreward", 127);

    for (i = 0; i < scoreHeals; i++)
    {
        int curRewards = Score_GetRewardCount(pln);
        int giveLife   = false;
        
        switch (rewardtypes)
        {
          default:
            giveLife = curRewards % 2 == 1;
            break;
          
          case 1:
            giveLife = curRewards % 2 == 0;
            break;
          
          case 2:
            break;
          
          case 3:
            giveLife = true;
            break;
        }

        if (giveLife)
        {
            Score_ModExtraLives(pln, 1);
            newLives += 1;
        }
        else
        {
            Score_ModRegenTimer(pln, (35 * 20) + 1);
        }

        Score_ModRewardCount(pln, 1);
    }

    FadeRange(128, 212, 255, 1.0 - powFloat(0.65, scoreHeals), 128, 212, 255, 0, 0.75);

    if (newLives > 0)
    {
        Delay(24);
        LocalAmbientSound("dakka/fullheal", 127);

        FadeRange(192, 224, 255, 1.0 - powFloat(0.5, newLives), 192, 124, 255, 0.0, 0.75);
    }
}




function void Score_DoRewards(int lastScore, int curScore)
{
    int pln      = PlayerNumber();
    int noReward = (GetCVar("dakka_score_rewardtypes") == 4) || GetUserCVar(pln, "dakka_cl_noscorerewards");
    if (noReward) { return; }

    // Update this because yeah
    Score_CalcScorePercent(pln);

    int scoreThreshold = Score_Thresholds[ST_FULLHEAL];
    if (scoreThreshold == 0) { return; }

    int lastScoreHeals = lastScore / scoreThreshold;
    int  curScoreHeals =  curScore / scoreThreshold;

    int newScoreHeals = curScoreHeals - lastScoreHeals;

    if (newScoreHeals > 0)
    {
        ACS_NamedExecuteWithResult("Score_Award", newScoreHeals);
    }

    Score_ProcessRewards();
}



script "Score_CheckRevival" (void)
{
    int pln = PlayerNumber();

    int extraLives = Score_GetExtraLives(pln);
    int hasLives   = Score_GetHasLives(pln);

    if (!hasLives) { terminate; }

    if (GetActorProperty(0, APROP_Health) <= 0)
    {
        Score_SetExtraLives(pln, 0);
        SetPlayerProperty(false, false, PROP_BUDDHA);
    }
    else if (GetActorProperty(0, APROP_Health) == 1)
    {
        // Read dakka_startmode_health to determine what the 'respawn'
        //  stats should be
        int startmode = GetCVar("dakka_startmode_health");
        int classNum  = Pickup_ClassNumber(0);

        SetActorProperty(0, APROP_Health, getMaxHealth());

        switch (startmode)
        {
          default:
            break;

          case 2:
            TakeInventory("BasicArmor", 0x7FFFFFFF);
            Pickup_DoPickup(It_GreenArmor, classNum, false);
            break;

          case 3:
            TakeInventory("BasicArmor", 0x7FFFFFFF);
            GiveInventory("Pickup_Soulsphere", 1);
            Pickup_DoPickup(It_BlueArmor,  classNum, false);
            break;

          case 4:
            GiveInventory("Pickup_Megasphere", 1);
            break;
        }

        Score_ModExtraLives(pln, -1);

        int revivalTID = UniqueTID();
        int myTID_old  = ActivatorTID();
        int myTID      = UniqueTID();
        Thing_ChangeTID(0, myTID);

        SpawnForced("DakkaReviveExplosion", GetActorX(0), GetActorY(0), GetActorZ(0) + 32.0, revivalTID);
        GiveInventory("RevivalIntervention", 1);

        SetActivator(revivalTID);
        SetPointer(AAPTR_TARGET, myTID);
        SetActorAngle(0, GetActorAngle(myTID));

        SetActivator(myTID);
        Thing_ChangeTID(myTID, myTID_old);

        FadeRange(255, 223, 155, 0.75,    255, 79, 0, 0.0, 1.0);

        // we took out 1, so now it's actually 0 in the array
        if (extraLives == 1)
        {
            SetPlayerProperty(false, false, PROP_BUDDHA);
        }
    }
}



#define AMMOREGENCOUNT  8

int AmmoRegen_AmmoTypes[AMMOREGENCOUNT] =
{
    "DakkaBullet_50AE",
    "DakkaShells",
    "DakkaGrenades",

    "DakkaBullet_5mm",
    "DakkaFlamerFuel",
    "DakkaRockets",

    "DakkaCells",
    "DakkaScrap",
};

// This is per 20 seconds because ammo regen lasts 20 seconds

int AmmoRegen_Per20Seconds[AMMOREGENCOUNT] =
{
    210,
    40,
    16,

    500,
    32,
    60,

    160,
    90,
};

int AmmoRegen_RegenCounters[PLAYERMAX][AMMOREGENCOUNT];


function void Score_ProcessRewards(void)
{
    if (isDead(0))
    {
        Score_ProcessRewards_Dead();
        return;
    }
    
    int pln = PlayerNumber();

    int regenTime =  Score_GetRegenTimer(pln);
    int regenSpent = Score_GetRegenSpent(pln);
    int i;

    if (regenTime > 0)
    {
        if (regenSpent == 0)
        {
            for (i = 0; i < AMMOREGENCOUNT; i++)
            {
                AmmoRegen_RegenCounters[pln][i] = 0;
            }
        }

        for (i = 0; i < AMMOREGENCOUNT; i++)
        {
            int ammoName     = AmmoRegen_AmmoTypes[i];
            int ammoPerCycle = AmmoRegen_Per20Seconds[i];
            int regenStep    = AmmoRegen_RegenCounters[pln][i]++ % 700;
            
            int curRegen = ( regenStep      * ammoPerCycle) / 700; // 700 tics = 20 seconds
            int newRegen = ((regenStep + 1) * ammoPerCycle) / 700;
            int ammoStep = newRegen - curRegen;
             
            if (ammoStep > 0) { GiveAmmo(ammoName, ammoStep); }
        }

        Score_ModRegenSpent(pln, 1);
    }
    else if (regenSpent > 0)
    {
        Score_SetRegenSpent(pln, 0);
    }


    int newRegenTime = max(0, regenTime - 1);
    Score_SetRegenTimer(pln, newRegenTime);
    
    int wasInRegen = CheckInventory("DakkaInAmmoRegen");
    int isInRegen  = regenTime > 0;
    int justReset  = Score_GetLastReset(pln) == Timer();

    SetInventory("DakkaInAmmoRegen",       isInRegen);
    SetInventory("DakkaEnteredAmmoRegen",  isInRegen && !wasInRegen);
    SetInventory("DakkaExitedAmmoRegen",  !isInRegen &&  wasInRegen && !justReset);

    int extraLives = Score_GetExtraLives(pln);
    int hasLives   = Score_GetHasLives(pln);

    if (extraLives > 0)
    {
        Score_SetHasLives(pln, true);
        SetPlayerProperty(false, true, PROP_BUDDHA);

        ACS_NamedExecuteWithResult("Score_CheckRevival");

    }
    else if (hasLives)
    {
        Score_SetHasLives(pln, false);
        SetPlayerProperty(false, false, PROP_BUDDHA);
    }
}

function void Score_ProcessRewards_Dead(void)
{
    int wasInRegen = CheckInventory("DakkaInAmmoRegen");
    SetInventory("DakkaInAmmoRegen", 0);
    SetInventory("DakkaEnteredAmmoRegen", 0);
    SetInventory("DakkaExitedAmmoRegen", wasInRegen);
}