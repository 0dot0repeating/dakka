function void Dakka_FistTap(void)
{
    if (inputPressed(BT_ATTACK)) { SetInventory("DakkaFistTap_Main", 5); }
    else { TakeInventory("DakkaFistTap_Main", 1); }
    
    if (inputPressed(BT_ALTATTACK)) { SetInventory("DakkaFistTap_Alt", 5); }
    else { TakeInventory("DakkaFistTap_Alt", 1); }
}

script "Dakka_FistHit" (void)
{
    int myTID = defaultTID(0);
    
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
    
    int dx = GetActorX(0) - GetActorX(firerTID);
    int dy = GetActorY(0) - GetActorY(firerTID);
    
    int punchAngle = VectorAngle(dx, dy);
    int angleDiff  = middle(-0.02, angleDifference(firerAngle, punchAngle), 0.02);
    
    SetActivator(firerTID);
    ACS_NamedExecuteWithResult("Dakka_FistLockon", angleDiff);
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