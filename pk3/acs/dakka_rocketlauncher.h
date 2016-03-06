script "Dakka_RLFull" (void)
{
    int ret = (CheckInventory("DakkaRockets") - CheckInventory("DakkaRocketsLoaded")) > 0;
    ret |= CheckInventory("DakkaInfiniteAmmo");
    SetResultValue(ret);
}
