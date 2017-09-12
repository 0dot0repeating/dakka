// Gotta do this because ZDoom only has 8 sound slots, because fuck you.
int AmmoRegen_SoundLooperTIDs[PLAYERMAX];

script "Score_Award" (int scoreHeals)
{
    int i;
    int pln = PlayerNumber();
    int newLives = 0;

    for (i = 0; i < scoreHeals; i++)
    {
        LocalAmbientSound("dakka/pointreward", 127);
        
        int curRewards = Score_GetRewardCount(pln);

        if (curRewards % 2)
        {
            Score_ModExtraLives(pln, 1);
            newLives += 1;
        }
        else
        {
            Score_ModRegenTimer(pln, (36 * 20) + 1);
        }

        Score_ModRewardCount(pln, 1);
    }

    FadeRange(255, 234, 0, 1.0 - powFloat(0.65, scoreHeals), 255, 234, 0, 0.0, 0.75);

    if (newLives > 0)
    {
        Delay(24);

        for (i = 0; i < newLives; i++)
        {
            LocalAmbientSound("dakka/fullheal", 127);
        }

        FadeRange(192, 224, 255, 1.0 - powFloat(0.5, newLives), 192, 124, 255, 0.0, 0.75);
    }
}




function void Score_DoRewards(int lastScore, int curScore)
{
    int pln = PlayerNumber();
    if (CToS_ServerData[pln][C2S_D_NOSCOREREWARDS]) { return; }

    // Update this because yeah
    Score_CalcScorePercent(pln);

    int scoreThreshold = MapStart_FullHealPoints;

    if (scoreThreshold == 0)
    {
        Score_CalcMapPoints();
        scoreThreshold = MapStart_FullHealPoints;
    }

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
    int myTID = defaultTID(-1);

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
            Pickup_DoPickup(It_Soulsphere, classNum, false);
            Pickup_DoPickup(It_BlueArmor,  classNum, false);
            break;

          case 4:
            TakeInventory("BasicArmor", 0x7FFFFFFF);
            Pickup_DoPickup(It_Megasphere, classNum, false);
            break;
        }

        Score_ModExtraLives(pln, -1);

        int revivalTID = UniqueTID();

        SpawnForced("DakkaReviveExplosion", GetActorX(0), GetActorY(0), GetActorZ(0) + 32.0, revivalTID);
        GiveInventory("RevivalIntervention", 1);

        SetActivator(revivalTID);
        SetPointer(AAPTR_TARGET, myTID);
        SetActorAngle(0, GetActorAngle(myTID));
        SetActivator(myTID);

        FadeRange(255, 223, 155, 0.75,    255, 79, 0, 0.0, 1.0);

        // we took out 1, so now it's actually 0 in the array
        if (extraLives == 1)
        {
            SetPlayerProperty(false, false, PROP_BUDDHA);
        }
    }
}



#define AMMOREGENCOUNT  10

int AmmoRegen_AmmoTypes[AMMOREGENCOUNT] =
{
    "DakkaBullet_10mm",
    "DakkaShells",
    "DakkaShotGrenades",
    "DakkaGyroShells",
    
    "DakkaBullet_5mm",
    "DakkaRainGrenades",
    "DakkaFlamerFuel",
    "DakkaRockets",
    
    "DakkaCells",
    "DakkaScrap",
};

int AmmoRegen_PerSecond[AMMOREGENCOUNT] = 
{
    15.0,
    3.0,
    0.75,
    1.1,

    20.0,
    0.75,
    2.5,
    2.0,

    5.0,
    4.0,
};

int AmmoRegen_RegenCounters[PLAYERMAX][AMMOREGENCOUNT];


function void Score_ProcessRewards(void)
{
    int pln = PlayerNumber();
    int myTID = defaultTID(-1);

    int regenTime = Score_GetRegenTimer(pln);
    int inRegen   = Score_GetRegenSpent(pln);

    int looperTID = AmmoRegen_SoundLooperTIDs[pln];
    int i;

    if (regenTime > 0)
    {
        if (inRegen == 0)
        {
            for (i = 0; i < AMMOREGENCOUNT; i++)
            {
                AmmoRegen_RegenCounters[pln][i] = 0;
            }
        }

        if (looperTID == 0)
        {
            looperTID = UniqueTID();
            Spawn("RegenSoundLooper", GetActorX(0), GetActorY(0), GetActorZ(0), looperTID);
            AmmoRegen_SoundLooperTIDs[pln] = looperTID;

            SetActivator(looperTID);
            SetPointer(AAPTR_TARGET, myTID);
            SetActivator(myTID);
        }

        int  curTic_bySecond = itof( Timer() % 36);
        int lastTic_bySecond = itof((Timer() % 36) - 1);
        
        for (i = 0; i < AMMOREGENCOUNT; i++)
        {
            int ammoName   = AmmoRegen_AmmoTypes[i];
            int ammoPerSec = AmmoRegen_PerSecond[i];

            int  curTic_ammostep = FixedMul(ammoPerSec,  curTic_bySecond / 36);
            int lastTic_ammostep = FixedMul(ammoPerSec, lastTic_bySecond / 36);

            // We get the ammo step in a convoluted way so as to minimize error
            //  over time due to fixed-point imprecision.
            int ammoStep = curTic_ammostep - lastTic_ammostep;

            int curRegen = AmmoRegen_RegenCounters[pln][i];

            curRegen += ammoStep;

            int ammoToGive = ftoi(curRegen);
            if (ammoToGive > 0) { GiveAmmo(ammoName, ammoToGive); }

            AmmoRegen_RegenCounters[pln][i] = curRegen % 1.0;
        }

        if ((inRegen > 0) && (inRegen % 24 == 0))
        {
            FadeRange(128, 192, 255, random(0.1, 0.15), 128, 192, 255, 0, 0.4);
        }

        Score_ModRegenSpent(pln, 1);
    }


    int newRegenTime = max(0, regenTime - 1);

    Score_SetRegenTimer(pln, newRegenTime);

    if (inRegen && newRegenTime == 0)
    {
        Score_SetRegenSpent(pln, 0);
        if (looperTID != 0) { SetActorState(looperTID, "RegenDone"); }
        AmmoRegen_SoundLooperTIDs[pln] = 0;
    }



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
