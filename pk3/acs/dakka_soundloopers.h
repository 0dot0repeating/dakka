#define SLOOP_COUNT         2
#define SLOOP_ID_REGEN      0
#define SLOOP_ID_VAMPIRE    1

#define SACTION_SILENCE         0
#define SACTION_DEACTIVATE      1
#define SACTION_KEEPACTIVE      2
#define SACTION_ACTIVATE        3

int SoundLooperTIDs[PLAYERMAX][SLOOP_COUNT];

script "Dakka_SoundLooper_WhatDo" (int which)
{
    int ret = SACTION_SILENCE;

    switch (which)
    {
      case SLOOP_ID_REGEN:
        if (CheckInventory("DakkaExitedAmmoRegen"))
        {
            ret = SACTION_DEACTIVATE;
        }
        else if (CheckInventory("DakkaEnteredAmmoRegen"))
        {
            ret = SACTION_ACTIVATE;
        }
        else if (CheckInventory("DakkaInAmmoRegen"))
        {
            ret = SACTION_KEEPACTIVE;
        }
        break;

      case SLOOP_ID_VAMPIRE:
        if (CheckInventory("DakkaVampireDone"))
        {
            ret = SACTION_DEACTIVATE;
        }
        else if (CheckInventory("DakkaVampireRenewed"))
        {
            ret = SACTION_ACTIVATE;
        }
        else if (CheckInventory("DakkaVampireTimer"))
        {
            ret = SACTION_KEEPACTIVE;
        }
        break;
    }

    SetResultValue(ret);
}


script "Dakka_SoundLooper_Create" (int which)
{
    str looperType;

    switch (which)
    {
        case SLOOP_ID_REGEN:    looperType = "RegenSoundLooper"; break;
        case SLOOP_ID_VAMPIRE:  looperType = "VampireSoundLooper"; break;
    }

    if (!looperType) { terminate; }

    int looperTID = UniqueTID();
    int x = GetActorX(0);
    int y = GetActorY(0);
    int z = GetActorZ(0);
    SpawnForced(looperType, x,y,z, looperTID);

    int myTID_old = ActivatorTID();
    int myTID     = UniqueTID();
    Thing_ChangeTID(0, myTID);

    SetActivator(looperTID);
    SetPointer(AAPTR_TARGET, myTID);
    Warp(myTID, 0,0,0, 0, WARPF_NOCHECKPOSITION | WARPF_COPYINTERPOLATION | WARPF_COPYVELOCITY);
    ACS_NamedExecuteWithResult("Dakka_SoundLooper_Follow");

    Thing_ChangeTID(myTID, myTID_old);
    SetResultValue(looperTID);
}


function void Dakka_SoundLoopers(void)
{
    int pln = PlayerNumber();

    for (int i = 0; i < SLOOP_COUNT; i++)
    {
        int soundTID    = SoundLooperTIDs[pln][i];
        int soundAction = ACS_NamedExecuteWIthResult("Dakka_SoundLooper_WhatDo", i);

        if ((soundTID == 0) || !IsTIDUsed(soundTID))
        {
            soundTID = ACS_NamedExecuteWIthResult("Dakka_SoundLooper_Create", i);
            SoundLooperTIDs[pln][i] = soundTID;
        }

        int active = GetUserVariable(soundTID, "user_active");

        switch (soundAction)
        {
          case SACTION_SILENCE:
            if (active)
            {
                SetActorState(soundTID, "Silenced");
                SetUserVariable(soundTID, "user_active", false);
            }
            break;

          case SACTION_DEACTIVATE:
            if (active)
            {
                SetActorState(soundTID, "Silenced");
                SetUserVariable(soundTID, "user_active", false);
            }

            SetActorState(soundTID, "Deactivate");
            break;

          case SACTION_KEEPACTIVE:
            if (!active)
            {
                SetActorState(soundTID, "Active");
                SetUserVariable(soundTID, "user_active", true);
            }
            break;

          case SACTION_ACTIVATE:
            if (!active)
            {
                SetActorState(soundTID, "Active");
                SetUserVariable(soundTID, "user_active", true);
            }

            SetActorState(soundTID, "Activate");
            break;
        }
    }
}






script "Dakka_PlayerNumber" (int ptr)
{
    SetActivator(0, ptr);
    SetResultValue(PlayerNumber());
}

script "Dakka_SoundLooper_Follow" (int tid, int syncPln)
{
    if (tid) { SetActivator(tid); }

    int targetPln = ACS_NamedExecuteWithResult("Dakka_PlayerNumber", AAPTR_TARGET);
    int justOnce  = false;

    if (IsZandronum && ConsolePlayerNumber() == -1 && targetPln != -1)
    {
        ACS_NamedExecuteAlways("Dakka_SoundLooper_FollowPlayer", 0, targetPln);
        terminate;
    }

    while (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        int myTID_old = ActivatorTID();
        int myTID     = UniqueTID();
        Thing_ChangeTID(0, myTID);

        SetActivator(0, AAPTR_TARGET);

        int pln = PlayerNumber();
        if ((pln > -1 && !PlayerInGame(pln)) || (ClassifyActor(0) & ACTOR_WORLD))
        {
            StopSound(myTID, CHAN_BODY);
            SetActorState(myTID, "Silenced");
            Thing_ChangeTID(myTID, myTID_old);
            break;
        }

        int firerTID_old = ActivatorTID();
        int firerTID     = UniqueTID();
        Thing_ChangeTID(0, firerTID);

        SetActivator(myTID);
        Warp(firerTID, 0,0,0, 0, WARPF_NOCHECKPOSITION | WARPF_COPYINTERPOLATION | WARPF_COPYVELOCITY);

        Thing_ChangeTID(0, myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        Delay(1);
    }
}


script "Dakka_SoundLooper_FollowSync" (int targetPln, int syncPln)
{
    if (ConsolePlayerNumber() != -1) { terminate; }
    
    for (int i = 0; i < SLOOP_COUNT; i++)
    {
        int tid = SoundLooperTIDs[targetPln][i];
        if (tid != 0 && IsTIDUsed(tid))
        {
            SetActivator(tid);
            ACS_NamedExecuteAlways("Dakka_SoundLooper_FollowPlayer", 0, targetPln, syncPln+1);
        }
    }
}


script "Dakka_SoundLooper_FollowPlayer" (int pln, int syncPln) clientside
{
    if (!IsZandronum) { terminate; }

    syncPln--;
    if (syncPln > -1 && syncPln != ConsolePlayerNumber()) { terminate; }
    

    while (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        int myTID_old = ActivatorTID();
        int myTID     = UniqueTID();
        Thing_ChangeTID(0, myTID);

        SetActivatorToPlayer(pln);

        if (!PlayerInGame(pln) || (ClassifyActor(0) & ACTOR_WORLD))
        {
            StopSound(myTID, CHAN_BODY);
            SetActorState(myTID, "Silenced");
            Thing_ChangeTID(myTID, myTID_old);
            break;
        }

        int firerTID_old = ActivatorTID();
        int firerTID     = UniqueTID();
        Thing_ChangeTID(0, firerTID);

        SetActivator(myTID);
        Warp(firerTID, 0,0,0, 0, WARPF_NOCHECKPOSITION | WARPF_COPYINTERPOLATION | WARPF_COPYVELOCITY);

        Thing_ChangeTID(0, myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        Delay(1);
    }
}