#define SCRAPTYPES 4

int Scrap_Items[SCRAPTYPES] =
{
    "ScrapCounter",
    "ScrapCounter_Minigun",
    "ScrapCounter_Flamer",
    "ScrapCounter_Cells",
};

#define SCRAP_NEEDED    0
#define SCRAP_GIVE      1

int Scrap_Batches[SCRAPTYPES][2] =
{
    { 100,  1}, // regular
    { 200,  1}, // minigun
    { 400, 16}, // flamer
    {2000, 20}, // cells
};

function void Dakka_GiveScrap(int amount, int type)
{
    if (amount <= 0) { return; }
    if (type < 0 || type >= SCRAPTYPES)
    {
        Log(s:"\caDakka_GiveScrap\cg was called with out-of-bounds type index ", d:type);
        return;
    }
    
    str scrapType  = Scrap_Items[type];
    int batchSize  = Scrap_Batches[type][SCRAP_NEEDED];
    
    int giveAmount = amount * middle(0, GetCVar("dakka_scrapfactor"), 1000);
    int newAmount  = CheckInventory(scrapType) + giveAmount;
    int batches    = newAmount / batchSize;
    
    GiveAmmo("DakkaScrap", batches * Scrap_Batches[type][SCRAP_GIVE]);
    SetInventory(scrapType, newAmount - (batches * batchSize));
}


// This is mainly to avoid ammo desyncs online, but also to make sure no scrap is
// given when you have infinite ammo.
//
// Zandronum clients predict when they're going to use ammo, and take it away
// themselves. The server therefore never bothers to tell them they used ammo,
// which usually works out... but with Dakka, it doesn't. At all. So we gotta
// handle this manually.

script "Dakka_UseAmmo" (int ammoindex, int count, int scrapgive, int scraptype)
{
    if (!IsServer) { terminate; }
    if (ammoindex < 0 || ammoindex >= AMMOCOUNT)
    { 
        Log(s:"\caDDakka_UseAmmo\cg was called with out-of-bounds ammo index ", d:ammoindex);
        terminate;
    }
    if (HasInfiniteAmmo()) { terminate; }
    
    if (ammoindex == AMMO_PISTOL && GetCVar("dakka_infinitepistol")) { terminate; }

    str ammotype   = PKP_KnownAmmo[ammoindex];
    int ammoBefore = CheckInventory(ammotype);
    
    Dakka_GiveScrap(scrapgive, scraptype);
    TakeInventory(ammotype, count);
    
    int ammoLeft = CheckInventory(ammotype);
    int lowcount = PKP_DefaultAmmoCount[ammoindex][2];
    
    if (ammoLeft == 0)
    {
        ACS_NamedExecuteWithResult("Dakka_OutOfAmmo", -1, false);
    }
    else if (ammoLeft <= lowcount && ammoBefore > lowCount)
    {
        ACS_NamedExecuteWithResult("Dakka_OutOfAmmo", -1, true);
    }
}

script "Dakka_OutOfAmmo" (int trigger, int justlow)
{
    int pln = PlayerNumber();
    if (pln == -1) { terminate; }
    
    int mode = GetUserCVar(pln, "dakka_cl_noammoclick");
    if (mode <= 0 || (mode <= 1 && justlow)) { terminate; }
    
    str checkitem;
    
    switch (trigger)
    {
        default: checkitem = "OutOfAmmoTimer_Pri"; break;
        case 1:  checkitem = "OutOfAmmoTimer_Alt"; break;
    }
    
    int t = Timer();
    
    if (trigger == -1 || CheckInventory(checkitem) < t)
    {
        SetInventory(checkitem, t + 16);
        
        // this looks dumb, but I want to avoid sound slot issues
        int myTID_old = ActivatorTID();
        int myTID_new = UniqueTID();
        Thing_ChangeTID(0, myTID_new);
        
        int newTID = UniqueTID();
        SpawnForced("OutOfAmmoSound", 0,0,0, newTID);
        
        SetActivator(newTID);
        Warp(myTID_new, 0,0,24.0, 0, WARPF_NOCHECKPOSITION | WARPF_COPYINTERPOLATION);
        SetPointer(AAPTR_TARGET, myTID_new);
        
        if (justlow) { SetActorState(0, "LowAmmo"); }
        
        Thing_ChangeTID(myTID_new, myTID_old);
    }
}