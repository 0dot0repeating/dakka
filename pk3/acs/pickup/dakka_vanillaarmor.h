script "Dakka_VanillaHealthArmor" (void)
{
    SetResultValue(GetCVar("dakka_vanillahealtharmor"));
}

script "Dakka_Pickup_VanillaArmor" (int regular, int vanilla)
{
    if (GetCVar("dakka_vanillahealtharmor") > 0)
    {
        SetResultValue(vanilla);
    }
    else
    {
        SetResultValue(regular);
    }
}