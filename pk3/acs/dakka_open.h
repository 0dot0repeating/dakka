script DAKKA_OPEN open
{
    SetDBEntry("dakka", "iszand", 1);
    IsZand = GetDBEntry("dakka", "iszand");

    Score_CalcMapPoints();
}


script DAKKA_OPEN_CLIENT open clientside
{
    int cpln = ConsolePlayerNumber();

    while (true)
    {
        // In score/score_display.h
        // TODO: Move this to clientside display,
        //       using the pickup client->server system
        Score_Update(cpln);
        Delay(1);
    }
}
