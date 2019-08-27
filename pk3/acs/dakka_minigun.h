#define WRF_NOSECONDARY     8

script "Dakka_MinigunReady" (void)
{
    if (CheckInventory("DakkaFlamerFuel"))
    {
        SetResultValue(0);
    }
    else
    {
        SetResultValue(WRF_NOSECONDARY);
    }
}


script "Dakka_MinigunFireStyle" (void) clientside
{
    int pln;

    if (IsZandronum)
    {
        pln = ConsolePlayerNumber();
    }
    else
    {
        SetActivator(0, AAPTR_TARGET);
        pln = PlayerNumber();
    }

    SetResultValue(GetUserCVar(pln, "dakka_cl_flamethrowerstyle"));
}


script "Dakka_InheritVelocity" (int ptr, int percentForward, int percentSide, int percentBackward)
{
    percentForward  = itof(percentForward)  / 100;
    percentSide     = itof(percentSide)     / 100;
    percentBackward = itof(percentBackward) / 100;

    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);

    SetActivator(0, ptr);
    if (IsWorld())
    {
        Thing_ChangeTID(myTID_new, myTID_old);
        terminate;
    }

    int ptrVelX   = GetActorVelX(0);
    int ptrVelY   = GetActorVelY(0);

    int ptrAng     = GetActorAngle(0);
    int ptrAngX    = cos(ptrAng);
    int ptrAngY    = sin(ptrAng);

    int velForward = dot2(ptrVelX, ptrVelY, ptrAngX, ptrAngY);
    int velForX    = FixedMul(ptrAngX, velForward);
    int velForY    = FixedMul(ptrAngY, velForward);
    int velSideX   = ptrVelX - velForX;
    int velSideY   = ptrVelY - velForY;

    int velForwardPercent = cond(velForward > 0, percentForward, percentBackward);

    int modVelX = FixedMul(velForX, velForwardPercent) + FixedMul(velSideX, percentSide);
    int modVelY = FixedMul(velForY, velForwardPercent) + FixedMul(velSideY, percentSide);

    SetActivator(myTID_new);
    SetActorVelocity(0, modVelX, modVelY, 0, true, false);
    Thing_ChangeTID(myTID_new, myTID_old);
}

#define MB_FIRERTID     0
#define MB_FIRERPLN     1
#define MB_FIRERTEAM    2
#define MB_BURNERX      3
#define MB_BURNERY      4
#define MB_BURNERZ      5
#define MB_BURNDIST     6
#define MB_BURNINITIAL  7
#define MB_BURNTIME     8

int MinigunBurnInfo[10];

script "Dakka_StartMinigunBurn" (int dist, int initial, int duration, int burnTracer)
{
    int myTID = defaultTID(0);

    MinigunBurnInfo[MB_BURNERX]     = GetActorX(0);
    MinigunBurnInfo[MB_BURNERY]     = GetActorY(0);
    MinigunBurnInfo[MB_BURNERZ]     = GetActorZ(0);
    MinigunBurnInfo[MB_BURNDIST]    = itof(dist);
    MinigunBurnInfo[MB_BURNINITIAL] = initial;
    MinigunBurnInfo[MB_BURNTIME]    = duration;

    SetActivator(0, AAPTR_TARGET);
    int firerTID_old = ActivatorTID();
    int firerTID     = UniqueTID();
    Thing_ChangeTID(0, firerTID);

    MinigunBurnInfo[MB_FIRERTID]     = firerTID;
    MinigunBurnInfo[MB_FIRERPLN]     = PlayerNumber();
    MinigunBurnInfo[MB_FIRERTEAM]    = GetPlayerInfo(PlayerNumber(), PLAYERINFO_TEAM);

    SetActivator(myTID);

    if (burnTracer)
    {
        ACS_NamedExecuteWithResult("Dakka_MinigunBurn", AAPTR_TRACER);
    }
    else
    {
        GiveInventory("MinigunBurnPropagator", 1);
    }

    Thing_ChangeTID(firerTID, firerTID_old);
}



script "Dakka_MinigunBurn" (int ptr)
{
    if (ptr)
    {
        SetActivator(0, ptr);
        if (IsWorld()) { terminate; }
    }

    if (!CheckFlag(0, "SHOOTABLE"))
    {
        terminate;
    }

    if (ActivatorTID() == MinigunBurnInfo[MB_FIRERTID])
    {
        terminate;
    }

    int myPln     = PlayerNumber();
    int myTeam    = GetPlayerInfo(myPln, PLAYERINFO_TEAM);
    int firerPln  = MinigunBurnInfo[MB_FIRERPLN];
    int firerTeam = MinigunBurnInfo[MB_FIRERTEAM];

    if (ACS_NamedExecuteWithResult("ProjCheck_IsAllied", myPln, myTeam, firerPln, firerTeam))
    {
        terminate;
    }

    int burnX = MinigunBurnInfo[MB_BURNERX];
    int burnY = MinigunBurnInfo[MB_BURNERY];
    int burnZ = MinigunBurnInfo[MB_BURNERZ];

    if (ACS_NamedExecuteWithResult("ProjCheck_ShortestDist", burnX, burnY, burnZ) > MinigunBurnInfo[MB_BURNDIST])
    {
        terminate;
    }

    ACS_NamedExecuteWithResult("Dakka_MinigunAfterburn", MinigunBurnInfo[MB_FIRERTID]);
}



#define AFTERBURNDAMAGE     20

script "Dakka_MinigunAfterburn" (int firerTID)
{

    int initialBurn = CheckInventory("MinigunNewBurn");
    int burnTimer   = CheckInventory("MinigunBurnTimer");

    SetInventory("MinigunNewBurn",   max(initialBurn,   MinigunBurnInfo[MB_BURNINITIAL]));
    SetInventory("MinigunBurnTimer", max(burnTimer,     MinigunBurnInfo[MB_BURNTIME]));

    if (burnTimer > 0) { terminate; }
    
    int myTID_old = ActivatorTID();
    int myTID     = UniqueTID();
    Thing_ChangeTID(0, myTID);

    int alreadyBright = CheckFlag(myTID, "BRIGHT");
    if (!alreadyBright) { GiveActorInventory(myTID, "DakkaMinigun_Bright", 1); }

    // since TIDs change, keep track of firer and target through actor pointers
    int pointerTID = UniqueTID();
    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);

    SpawnForced("MinigunPointerDummy", myX,myY,myZ, pointerTID);
    SetActivator(pointerTID);
    SetPointer(AAPTR_TARGET, firerTID);
    SetPointer(AAPTR_TRACER, myTID);

    Thing_ChangeTID(myTID, myTID_old);

    // we need to track total burn time to deal accurate afterburn damage
    int timeBurning    = 0;
    int flameTimer     = 0;
    int directBurnTime = 0;
    int flamesPutOut   = false;

    int myXYRadius, myZRadius, randX, randY, randZ, i;

    while (true)
    {
        SetActivator(pointerTID, AAPTR_TRACER);
        if (IsWorld()) { break; }

        if (GetActorProperty(0, APROP_WaterLevel) > 1)
        {
            flamesPutOut = true;

            SetActivator(pointerTID, AAPTR_TRACER);

            myXYRadius = GetActorProperty(0, APROP_Radius);
            myZRadius  = GetActorProperty(0, APROP_Height) / 2;

            for (i = 0; i < 8; i++)
            {
                randX = random( 0.1, 0.6) * randSign();
                randY = random( 0.1, 0.6) * randSign();
                randZ = random(-0.8, 0.4);

                SpawnForced("MinigunSteam", myX + FixedMul(myXYRadius, randX),
                                            myY + FixedMul(myXYRadius, randY),
                                            myZ + FixedMul(myZRadius,  randZ));
            }

            break;
        }

        // If +BRIGHT gets toggled, we want to handle that
        if (!CheckFlag(0, "BRIGHT"))
        {
            GiveInventory("DakkaMinigun_Bright", 1);
            alreadyBright = false;
        }

        // Handle this here or else this CVar will do jack shit
        int powermult = DakkaPowerMults[Dakka_PowerLevel()];

        int burnDamage = 0;

        initialBurn = CheckInventory("MinigunNewBurn");

        if (initialBurn > 0)
        {
            initialBurn += min(directBurnTime / 14, 6);
            directBurnTime = min(directBurnTime + 3, (14*6)+70);
        }
        else
        {
            directBurnTime = max(0, directBurnTime - 1);
        }

        burnDamage += FixedMul(initialBurn, powermult);
        TakeInventory("MinigunNewBurn", 0x7FFFFFFF);

        burnTimer   = CheckInventory("MinigunBurnTimer");

        if (burnTimer > 0)
        {
            int adjustedBurnDmg = FixedMul(AFTERBURNDAMAGE, powermult); // is int, not fixed
            int afterburn_prev  = fractionMult(timeBurning,   adjustedBurnDmg, 35);
            int afterburn_next  = fractionMult(timeBurning+1, adjustedBurnDmg, 35);
            
            burnDamage += afterburn_next - afterburn_prev;
            TakeInventory("MinigunBurnTimer", 1);
        }

        //Log(s:"(", n:0, s:"\c-) Burn damage: ", d:burnDamage, s:" (", d:initialBurn, s:", ", d:burnTimer, s:")");

        myX = GetActorX(0);
        myY = GetActorY(0);
        myZ = GetActorZ(0) + (GetActorProperty(0, APROP_Height) / 2);

        // Do this for sound purposes
        SetActivator(pointerTID);
        Warp(0, myX, myY, myZ, 0, WARPF_ABSOLUTEPOSITION | WARPF_NOCHECKPOSITION | WARPF_INTERPOLATE);
        SetActivator(pointerTID, AAPTR_TRACER);

        if (burnDamage > 0 && !isDead(0))
        {
            SetActivator(pointerTID, AAPTR_TARGET);

            int fireTID = UniqueTID();
            SpawnForced("MinigunBurner", myX,myY,myZ, fireTID);
            SetUserVariable(fireTID, "user_damage",     burnDamage);
            SetUserVariable(fireTID, "user_painfactor", cond(initialBurn, 3, 15));

            if (!IsWorld())
            {
                int firerTID_old = ActivatorTID();
                int firerTID_new = UniqueTID();
                Thing_ChangeTID(0, firerTID_new);

                SetActivator(fireTID);
                SetPointer(AAPTR_TARGET, firerTID_new);
                Thing_ChangeTID(firerTID_new, firerTID_old);
            }

            SetActorState(fireTID, "Burn");
        }

        if (flameTimer == 0)
        {
            SetActivator(pointerTID, AAPTR_TRACER);

            myXYRadius = GetActorProperty(0, APROP_Radius);
            myZRadius  = GetActorProperty(0, APROP_Height) / 2;

            randX = random( 0.1, 0.6) * randSign();
            randY = random( 0.1, 0.6) * randSign();
            randZ = random(-0.8, 0.4);

            SpawnForced("MinigunAfterburnFlame", myX + FixedMul(myXYRadius, randX),
                                                 myY + FixedMul(myXYRadius, randY),
                                                 myZ + FixedMul(myZRadius,  randZ));
            flameTimer = random(3, 7);
        }

        if (burnTimer == 1) { break; }
        timeBurning++;
        flameTimer--;
        Delay(1);
    }

    if (!alreadyBright)
    {
        SetActivator(pointerTID, AAPTR_TRACER);
        GiveInventory("DakkaMinigun_Unbright", 1);
    }

    int decayTime = cond(flamesPutOut, 12, 48);
    int decayInc  = 0.3 / decayTime;

    for (i = 0; i < decayTime; i++)
    {
        int vol = 0.3 - (i + 1) * decayInc;
        SoundVolume(pointerTID, CHAN_BODY, vol);
        Delay(1);
    }

    Thing_Remove(pointerTID);
}