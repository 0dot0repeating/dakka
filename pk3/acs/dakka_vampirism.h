script "Dakka_Vampire" (int powerupTics)
{
    int curTics = CheckInventory("DakkaVampireTimer");
    SetInventory("DakkaVampireTimer", max(curTics, powerupTics));
    if (curTics) { terminate; }

    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);

    int looperTID = UniqueTID();
    SpawnForced("VampireSoundLooper", GetActorX(0),GetActorY(0),GetActorZ(0), looperTID);

    SetActivator(looperTID);
    SetPointer(AAPTR_TARGET, myTID_new);

    SetActivator(myTID_new);
    Thing_ChangeTID(0, myTID_old);
    GiveInventory("DakkaVampireEffect", 1);
    FadeRange(255, 64, 64, 0.3, 255, 64, 64, 0, 1.0);

    while (CheckInventory("DakkaVampireTimer"))
    {
        if (isDead(0))
        {
            TakeInventory("DakkaVampireTimer", 0x7FFFFFFF);
            break;
        }

        TakeInventory("DakkaVampireTimer", 1);
        Delay(1);
    }

    TakeInventory("DakkaVampireEffect", 0x7FFFFFFF);
    FadeRange(255, 64, 64, 0.2, 255, 64, 64, 0, 1.0);
    SetActorState(looperTID, "VampireDone");
}

script "Dakka_VampireHeal" (int monHealth)
{
    if (!CheckInventory("DakkaVampireTimer")) { terminate; }

    int myHealth    = GetActorProperty(0, APROP_Health);
    int myMaxHealth = getMaxHealth();
    int healthOver  = myHealth - myMaxHealth;
    int baseHeal    = random(3, 6);
    int healDivisor = 75;
    int maxHeal     = 30;


    if (healthOver <= -50)
    {
        if (healthOver <= -75)
        {
            baseHeal    = random(5, 8);
            healDivisor = 50;
            maxHeal     = 50;
        }
        else
        {
            baseHeal    = random(4, 7);
            healDivisor = 60;
            maxHeal     = 40;
        }
    }
    else if (healthOver >= 0)
    {
        if (healthOver >= 100)
        {
            baseHeal    = random(1, 3);
            healDivisor = 150;
            maxHeal     = 10;
        }
        else
        {
            baseHeal    = random(2, 4);
            healDivisor = 100;
            maxHeal     = 20;
        }
    }

    int netHeal   = min(maxHeal, baseHeal + (monHealth / healDivisor));
    int newHealth = min(myMaxHealth + 200, myHealth + netHeal);
    SetActorProperty(0, APROP_Health, newHealth);
    FadeRange(255, 192, 192, 0.08, 255, 64, 64, 0, 0.3);

    int myTID_old = ActivatorTID();
    int myTID_new = UniqueTID();
    Thing_ChangeTID(0, myTID_new);

    int soundTID = UniqueTID();
    SpawnForced("VampireHealSound", GetActorX(0),GetActorY(0),GetActorZ(0), soundTID);

    SetActivator(soundTID);
    SetPointer(AAPTR_TARGET, myTID_new);

    SetActivator(myTID_new);
    Thing_ChangeTID(0, myTID_old);

    GiveInventory("HUD_VampireFlash1", 1);
    Delay(2);
    TakeInventory("HUD_VampireFlash1", 1);
    GiveInventory("HUD_VampireFlash2", 1);
    Delay(2);
    TakeInventory("HUD_VampireFlash2", 1);
    GiveInventory("HUD_VampireFlash3", 1);
    Delay(2);
    TakeInventory("HUD_VampireFlash3", 1);
}