#define WRF_NOSECONDARY     8

script "Dakka_MinigunReady" (void)
{
    if (CheckInventory("DakkaFlamerFuel"))
    {
        SetResultValue(0);
    }
    else
    {
        SetResultValue(WRF_NOSECONDARY);
    }
}
