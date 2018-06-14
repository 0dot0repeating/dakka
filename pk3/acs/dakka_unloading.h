script "Dakka_Unloading" unloading
{
    for (int i = 0; i < PLAYERMAX; i++)
    {
        if (PlayerInGame(i))
        {
            Score_SetScore_Unloading(i, Score_GetScore(i));
            Score_SetDisplayScore_Unloading(i, Score_GetDisplayScore(i));
        }
    }
}