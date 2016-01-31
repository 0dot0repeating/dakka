#define ARC_DAMAGE      193
#define ARC_DRAWBEAM    194

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
