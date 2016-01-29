script DAKKA_OPEN open
{
    SetDBEntry("dakka", "iszand", 1);
    IsZand = GetDBEntry("dakka", "iszand");

    Score_CalcMapPoints();
}
