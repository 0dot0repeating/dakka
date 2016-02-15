function void Dakka_ProcessIDFAArmor(void)
{
    int armorName    = GetArmorInfo(ARMORINFO_CLASSNAME);
    int armorProtect = GetArmorInfo(ARMORINFO_SAVEPERCENT);

    if (armorProtect == 0.5 && !strcmp_i(armorName, "BasicArmorPickup"))
    {
        int armorInv = CheckInventory("BasicArmor");
        TakeInventory("BasicArmor", 0x7FFFFFFF);

        GiveInventory("BlueArmor", 1);

        int armorDiff = armorInv - CheckInventory("BasicArmor");

        if (armorDiff < 0) { TakeInventory("BasicArmor", -armorDiff); }
        else               { GiveInventory("Pickup_OneArmor", armorDiff); }
    }
}
