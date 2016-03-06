world int 102:PlayerMapScoreRewards[];
world int 103:PlayerAmmoRegenTimers[];
world int 104:PlayerExtraLifeCount[];
world int 105:PlayerAmmoRegenTimeSpent[];
world int 106:PlayerHasExtraLives[];

// Gotta do this because ZDoom only has 8 sound slots, because fuck you.
int AmmoRegen_SoundLooperTIDs[PLAYERMAX];


script "Score_Award" (int scoreHeals)
{
    int i;
    int pln = PlayerNumber();

    for (i = 0; i < scoreHeals; i++)
    {
        LocalAmbientSound("dakka/pointreward", 127);
        
        int curRewards = PlayerMapScoreRewards[pln];

        if (curRewards % 2)
        {
            PlayerExtraLifeCount[pln] += 1;
        }
        else
        {
            PlayerAmmoRegenTimers[pln] += (36 * 20);
        }

        PlayerMapScoreRewards[pln] += 1;
    }

    FadeRange(255, 234, 0, 1.0 - powFloat(0.65, scoreHeals), 255, 234, 0, 0.0, 0.75);
}




function void Score_DoRewards(int lastScore, int curScore)
{
    int pln = PlayerNumber();
    if (CToS_ServerData[pln][C2S_D_NOSCOREREWARDS]) { return; }

    int scoreThreshold = MapStart_FullHealPoints;

    int lastScoreHeals = lastScore / scoreThreshold;
    int  curScoreHeals =  curScore / scoreThreshold;

    int newScoreHeals = curScoreHeals - lastScoreHeals;

    if (newScoreHeals > 0)
    {
        ACS_NamedExecuteWithResult("Score_Award", newScoreHeals);
    }

    Score_ProcessRewards();

    Sender_SetData(pln, S2C_D_LIVESLEFT,   PlayerExtraLifeCount[pln]);
    Sender_SetData(pln, S2C_D_REWARDCOUNT, PlayerMapScoreRewards[pln]);
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
    3.0,
    1.5,

    15.0,
    8.0,
};

int AmmoRegen_RegenCounters[PLAYERMAX][AMMOREGENCOUNT];


function void Score_ProcessRewards(void)
{
    int pln = PlayerNumber();
    int myTID = defaultTID(-1);

    int regenTime = PlayerAmmoRegenTimers[pln];
    int inRegen   = PlayerAmmoRegenTimeSpent[pln];

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

        PlayerAmmoRegenTimeSpent[pln] += 1;
    }

    int newRegenTime = max(0, regenTime - 1);

    PlayerAmmoRegenTimers[pln] = newRegenTime;

    if (inRegen && newRegenTime == 0)
    {
        PlayerAmmoRegenTimeSpent[pln] = 0;

        if (looperTID != 0)
        {
            SetActorState(looperTID, "RegenDone");
        }

        AmmoRegen_SoundLooperTIDs[pln] = 0;
    }



    int extraLives = PlayerExtraLifeCount[pln];
    int hasLives   = PlayerHasExtraLives[pln];

    if (extraLives > 0)
    {
        PlayerHasExtraLives[pln] = true;
        SetPlayerProperty(false, true, PROP_BUDDHA);

        if (GetActorPRoperty(0, APROP_Health) <= 0)
        {
            PlayerExtraLifeCount[pln] = 0;
            SetPlayerProperty(false, false, PROP_BUDDHA);
        }
        else if (GetActorProperty(0, APROP_Health) == 1)
        {
            SetActorProperty(0, APROP_Health, getMaxHealth());
            PlayerExtraLifeCount[pln] -= 1;

            int revivalTID = UniqueTID();

            SetActorVelocity(0, 0,0,0, false, true);
            SpawnForced("DakkaReviveExplosion", GetActorX(0), GetActorY(0), GetActorZ(0) + 32.0, revivalTID);

            SetActivator(revivalTID);
            SetPointer(AAPTR_TARGET, myTID);
            SetActorAngle(0, GetActorAngle(myTID));
            SetActivator(myTID);

            FadeRange(255, 223, 155, 0.75,    255, 79, 0, 0.0, 1.0);

            if (PlayerExtraLifeCount[pln] == 0)
            {
                SetPlayerProperty(false, false, PROP_BUDDHA);
            }
        }
    }
    else if (hasLives)
    {
        PlayerHasExtraLives[pln] = false;
        SetPlayerProperty(false, false, PROP_BUDDHA);
    }
}
