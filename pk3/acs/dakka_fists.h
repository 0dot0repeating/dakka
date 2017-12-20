function void Dakka_FistTap(void)
{
    if (inputPressed(BT_ATTACK)) { SetInventory("DakkaFistTap_Main", 5); }
    else { TakeInventory("DakkaFistTap_Main", 1); }
    
    if (inputPressed(BT_ALTATTACK)) { SetInventory("DakkaFistTap_Alt", 5); }
    else { TakeInventory("DakkaFistTap_Alt", 1); }
}

script "Dakka_FistHit" (int thrustpower, int lockpower)
{
    int myTID = defaultTID(0);
    if (lockpower == 0) { lockpower = 6; }
    if (lockpower < 0)  { lockpower = 0; }
    lockpower = itof(lockpower) / 360;
    
    SetActivator(0, AAPTR_TARGET);
    if (ClassifyActor(0) & ACTOR_WORLD)
    {
        SetResultValue(false);
        terminate;
    }
    
    int firerTID   = defaultTID(0);
    int firerAngle = GetActorAngle(0);
    
    SetActivator(myTID, AAPTR_TRACER);
    if ((ClassifyActor(0) & ACTOR_WORLD) || (!CheckFlag(0, "SHOOTABLE")))
    {
        SetResultValue(false);
        terminate;
    }
    
    SetResultValue(true);
    
    if (thrustpower > 0)
    {
        ACS_NamedExecuteWithResult("Dakka_FistThrust", firerAngle, thrustpower);
    }
    
    if (lockpower > 0)
    {
        int dx = GetActorX(0) - GetActorX(firerTID);
        int dy = GetActorY(0) - GetActorY(firerTID);
        
        int punchAngle = VectorAngle(dx, dy);
        int angleDiff  = middle(-lockpower, angleDifference(firerAngle, punchAngle), lockpower);
        
        SetActivator(firerTID);
        ACS_NamedExecuteWithResult("Dakka_FistLockon", angleDiff);
    }
}


script "Dakka_FistLockon" (int angDiff) clientside
{
    int left = angDiff;
    for (int i = 0; i < 3; i++)
    {
        int angDelta = angDiff / 3;
        ChangeActorAngle(0, GetActorAngle(0) + angDelta, true);
        left -= angDelta;
        Delay(1);
    }
    
    ChangeActorAngle(0, GetActorAngle(0) + left, true);
}


script "Dakka_FistThrust" (int angle, int power)
{
    int myMass   = GetActorProperty(0, APROP_Mass);
    int adjPower = FixedSqrt((itof(power) / myMass) * 200);
    int thrustX  = FixedMul(cos(angle), adjPower);
    int thrustY  = FixedMul(sin(angle), adjPower);
    
    Log(f:thrustX, s:", ", f:thrustY);
    
    SetActorVelocity(0, thrustX, thrustY, GetActorVelZ(0), false, false);
}