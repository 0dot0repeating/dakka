script "Dakka_AmmoSync" (int ammoindex)
{
    if (!(IsZandronum && ConsolePlayerNumber() == -1)) { terminate; }
    if (ammoindex < 0 || ammoindex >= AMMOCOUNT) { terminate; }

    str ammotype = PKP_KnownAmmo[ammoindex];

    if (CheckInventory(ammotype) == 0)
    {
        GiveAmmo(ammotype, 1);
        TakeInventory(ammotype, 1);
    }
    else
    {
        TakeInventory(ammotype, 1);
        GiveAmmo(ammotype, 1);
    }
}


script "Dakka_PlayerNumber" (int ptr)
{
    SetActivator(0, ptr);
    SetResultValue(PlayerNumber());
}

script "Dakka_SoundLooper" (void)
{
    int targetPln = ACS_NamedExecuteWithResult("Dakka_PlayerNumber", AAPTR_TARGET);

    if (IsZandronum && ConsolePlayerNumber() == -1 && targetPln != -1)
    {
        ACS_NamedExecuteAlways("Dakka_FollowTarget_Player", 0, targetPln);
        terminate;
    }

    int first = true;

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
            SetActorState(myTID, "Abort");
            Thing_ChangeTID(myTID, myTID_old);
            break;
        }

        int firerTID_old = ActivatorTID();
        int firerTID     = UniqueTID();
        Thing_ChangeTID(0, firerTID);

        SetActivator(myTID);

        Warp(firerTID, 0,0,0, 0, WARPF_NOCHECKPOSITION | WARPF_COPYINTERPOLATION | WARPF_COPYVELOCITY);

        if (first)
        {
            PlayActorSound(0, SOUND_See,    CHAN_ITEM, 1.0, false, ATTN_NORM);
            PlayActorSound(0, SOUND_Active, CHAN_BODY, 1.0, true,  ATTN_NORM);
            first = false;
        }

        Thing_ChangeTID(0, myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        Delay(1);
    }
}

script "Dakka_FollowTarget_Player" (int pln) clientside
{
    if (!IsZandronum) { terminate; }

    int first = true;

    while (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        int myTID_old = ActivatorTID();
        int myTID     = UniqueTID();
        Thing_ChangeTID(0, myTID);

        SetActivatorToPlayer(pln);

        if (!PlayerInGame(pln) || (ClassifyActor(0) & ACTOR_WORLD))
        {
            StopSound(myTID, CHAN_BODY);
            SetActorState(myTID, "Abort");
            Thing_ChangeTID(myTID, myTID_old);
            break;
        }

        int firerTID_old = ActivatorTID();
        int firerTID     = UniqueTID();
        Thing_ChangeTID(0, firerTID);

        SetActivator(myTID);

        Warp(firerTID, 0,0,0, 0, WARPF_NOCHECKPOSITION | WARPF_COPYINTERPOLATION | WARPF_COPYVELOCITY);

        if (first)
        {
            PlayActorSound(0, SOUND_See,    CHAN_ITEM, 1.0, false, ATTN_NORM);
            PlayActorSound(0, SOUND_Active, CHAN_BODY, 1.0, true,  ATTN_NORM);
            first = false;
        }

        Thing_ChangeTID(0, myTID_old);
        Thing_ChangeTID(firerTID, firerTID_old);
        Delay(1);
    }
}