int IDFAArmors[CLASSCOUNT] =
{
    "BlueArmor",
    "Dakka_BlueArmor",
};

function void Dakka_ProcessIDFAArmor(void)
{
    int armorName    = GetArmorInfo(ARMORINFO_CLASSNAME);
    int armorProtect = GetArmorInfo(ARMORINFO_SAVEPERCENT);

    if (armorProtect == 0.5 && !stricmp(armorName, "BasicArmorPickup"))
    {
        int idfaArmor = IDFAArmors[Pickup_ClassNumber(0) + 1];
        int armorInv = CheckInventory("BasicArmor");
        TakeInventory("BasicArmor", 0x7FFFFFFF);

        GiveInventory(idfaArmor, 1);

        int armorDiff = armorInv - CheckInventory("BasicArmor");

        if (armorDiff < 0) { TakeInventory("BasicArmor", -armorDiff); }
        else               { GiveInventory("Pickup_OneArmor", armorDiff); }
    }
}
