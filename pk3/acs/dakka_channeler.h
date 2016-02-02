#define ARC_DAMAGE      193
#define ARC_ALTFIRE     194

script ARC_DAMAGE (int damage)
{
    int myTID = defaultTID(-1);

    SetActivator(0, AAPTR_TRACER);
    if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }

    int hisTID     = ActivatorTID();
    int hisTID_new = defaultTID(-1);

    SetActivator(myTID, AAPTR_TARGET);

    Thing_Damage2(hisTID_new, damage, "Normal");
    Thing_ChangeTID(hisTID_new, hisTID);
}

// This script exists tp ensure that everything related to the arc runs in
//  the right order. All of this is necessary.
script ARC_ALTFIRE (int count, int isFirer)
{
    int myTID = defaultTID(-1);
    int firerTID;
    
    if (isFirer)
    {
        firerTID = defaultTID(-1);
    }
    else
    {
        SetActivator(0, AAPTR_TARGET);
        firerTID = defaultTID(-1);

        SetActivator(myTID);
    }

    int angleIncrement  = 1.0 / count;
    int startAngle      = GetActorAngle(0) + (angleIncrement / 2);

    int arcerTID = unusedTID(12000, 22000);
    int i;

    int myX = GetActorX(0);
    int myY = GetActorY(0);
    int myZ = GetActorZ(0);

    for (i = 0; i < count; i++)
    {
        SpawnForced("Channeler_SecondaryBase", myX, myY, myZ, arcerTID);
        SetActivator(arcerTID);

        SetPointer(AAPTR_TARGET, firerTID);
        SetActorAngle(0, startAngle + (angleIncrement * i));
        SetActorState(0, "Spawn");
        
        // In between this and the Thing_ChangeTID is the arc actor arcing

        Thing_ChangeTID(arcerTID, 0);
    }

    T_Clear();
}
