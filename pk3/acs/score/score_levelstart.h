function void Score_CalcMapPoints(void)
{
    int i;
    int name, count;

    int totalMons   = 0;
    int totalPoints = 0;

    for (i = 0; i < MONTYPES; i++)
    {
        name = Monster_KnownMons[i];
        count = ThingCountName(name, 0);

        totalMons   += count;
        totalPoints += count * Monster_Points[i];
    }

    MapStart_BasePoints     = totalPoints;
    MapStart_BaseMonsters   = totalMons;

    if (MapStart_BaseMonsters == 0)
    {
        MapStart_AveragePoints = 0;
    }
    else
    {
        MapStart_AveragePoints  = totalPoints / totalMons;
    }

    
    int fullHealMult = middle(P_FULLHEAL_MIN, FixedMul(MapStart_BaseMonsters, P_FULLHEAL_PERCENT), P_FULLHEAL_MAX);

    int fullHealPoints = MapStart_AveragePoints * fullHealMult;
    fullHealPoints = ((fullHealPoints + 2500) / 5000) * 5000; // Round to nearest 5000

    MapStart_FullHealPoints = max(5000, fullHealPoints);
}
