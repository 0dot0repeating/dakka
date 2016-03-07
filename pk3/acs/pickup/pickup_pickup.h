// Lovely name, isn't it? This is where picking up shit actually happens.



// This is here because in Dakka (which had a similar pickup system to this),
//  when you ran over a stack of 500 chainguns and didn't pick up any of them,
//  things started lagging really fucking badly because it was running the
//  pickup script 500 times a tic, for every tic you were moving over them.
//
// With this, if the pickup failed to do something, any future pickups on that
//  slot for that player on that tic will just fail instantly, to save a damn
//  lot of processor cycles.
//
// Dakka now uses this system, by the way.

int PKP_RefusePickups[PLAYERMAX][PICKUPCOUNT];




function int Pickup_IsScripted(int index, int classNum)
{
    int i;

    for (i = 0; i < PK_SCRIPTEDCOUNT; i++)
    {
        int checkIndex = PKP_ScriptedPickups[i][PK_S_ITEMNUM];
        int checkClass = PKP_ScriptedPickups[i][PK_S_CLASSNUM];

        if (index == checkIndex && classNum == checkClass)
        {
            return i;
        }
    }

    return -1;
}



function void Pickup_ItemPickup(int item, int count, int dropped)
{
    int oldcount = CheckInventory(item);
    GiveInventory(item, count);
    int newcount = CheckInventory(item);

    if (newcount > oldcount)
    {
        PKP_ReturnArray[PARRAY_SUCCEEDED]   = true;
        PKP_ReturnArray[PARRAY_CONSUME]     = true;
    }
}



// We don't pass arguments to this because they're supplied through
//  PKP_MessageData.
function void Pickup_SendMessage(void)
{
    int i, arg1, arg2;

    for (i = 0; i < MDATA_SLOTS; i += 2)
    {
        arg1 = PKP_MessageData[i];

        if (i + 1 < MDATA_SLOTS) { arg2 = PKP_MessageData[i+1]; }
        else { arg2 = 0; }

        if (IsZandronum && ConsolePlayerNumber() == -1)
        {
            ACS_NamedExecuteAlways("Pickup_ShowMessage", 0, i, arg1, arg2);
        }
        else
        {
            // I don't like latency.
            ACS_NamedExecuteWithResult("Pickup_ShowMessage", i, arg1, arg2);
        }
    }
}



function void Pickup_HandlePickups(int index, int classNum, int dropped)
{
    int i;

    for (i = 0; i < PK_RECEIVECOUNT; i++)
    {
        int item  = PKP_ReceiveItems[index][classNum+1][i];
        int count = PKP_ReceiveCount[index][classNum+1][i];

        if (stringBlank(item)) { continue; }

        int healIndex = Heal_HealthIndex(item);

        if (healIndex != -1)
        {
            Heal_PickupHealth(healIndex, count);
            continue;
        }

        int wepIndex = Weapon_WeaponIndex(item);

        if (wepIndex != -1)
        {
            Weapon_PickupWeapon(wepIndex, count, dropped);
            continue;
        }

        int armorIndex = Armor_ArmorIndex(item);

        if (armorIndex != -1)
        {
            Armor_PickupArmor(armorIndex, count);
            continue;
        }

        int ammoIndex = Ammo_AmmoIndex(item);

        if (ammoIndex != -1)
        {
            Ammo_PickupAmmo(ammoIndex, count, dropped);
            continue;
        }

        Pickup_ItemPickup(item, count, dropped);
    }
}


// Separated this out so that you can give class-based items in scripts without
//  showing a pickup message, flash, whatever.
function int Pickup_DoPickup(int index, int classNum, int dropped)
{
    // Clear the pickup return array.
    int i;

    for (i = 0; i < PARRAY_SLOTS; i++)
    {
        PKP_ReturnArray[i] = 0;
    }

    // Clear the message data array as well.
    for (i = 0; i < MDATA_SLOTS; i++)
    {
        PKP_MessageData[i] = 0;
    }

    // Little extra convenience: if the first item entry in the pickup is
    //  "Default", it'll use the default (unknown class) item entry instead.
    if (!stricmp(PKP_ReceiveItems[index][classNum+1][0], "Default"))
    {
        classNum = -1;
    }

    // So with the addition of pickup scripts being able to return a new index,
    //  we need to loop this until the chain of scripts stops returning new
    //  item numbers.
    
    int keepLooping = true;


    // The only instance in which we want to do another loop around is:
    //  - The current item index has a pickup script attached to it
    //  - The script is set to return a new item index
    //  - The new item index is not the same as the current item index

    PKP_PickupData[PDATA_CLASSNUM]  = classNum;
    PKP_PickupData[PDATA_DROPPED]   = dropped;

    while (keepLooping)
    {
        keepLooping = false;

        int scriptIndex = Pickup_IsScripted(index, classNum);
        int gotNewIndex = false;

        PKP_PickupData[PDATA_ITEMNUM]   = index;

        if (scriptIndex != -1)
        {
            int snum = PKP_ScriptedPickups[scriptIndex][PK_S_SCRIPTNUM];
            int arg1 = PKP_ScriptedPickups[scriptIndex][PK_S_ARG1];
            int arg2 = PKP_ScriptedPickups[scriptIndex][PK_S_ARG2];
            int arg3 = PKP_ScriptedPickups[scriptIndex][PK_S_ARG3];

            int named = PKP_ScriptedPickups[scriptIndex][PK_S_NAMEDSCRIPT];
            int name  = PKP_PickupNamed[scriptIndex];

            int getIndex = PKP_ScriptedPickups[scriptIndex][PK_S_RETURNINDEX];

            int newIndex;
            
            if (named)
            {
                newIndex = ACS_NamedExecuteWithResult(name, arg1, arg2, arg3);
            }
            else
            {
                newIndex = ACS_ExecuteWithResult(snum, arg1, arg2, arg3);
            }

            // Maybe we just wanted to determine which pickup index to use.
            //
            // You can just return the current item index, and that'll end
            //  the item number update loop.
            if (getIndex)
            {
                // If the new index is different, do another loop in case the
                //  new pickup has its own script.
                if (index != newIndex) { keepLooping = true; }

                index = newIndex;
                gotNewIndex = true;
            }
        }
    }

    // In case it changed.
    PKP_PickupData[PDATA_ITEMNUM] = index;

    if (scriptIndex == -1 || gotNewIndex)
    {
        Pickup_HandlePickups(index, classNum, dropped);
    }

    // If the index changed, the "Pickup_Pickup" script needs to know.
    return index;
}



script "Pickup_Pickup" (int index, int dropped)
{
    // For some reason the pickup code likes to run on the client,
    //  even when it shouldn't.
    if (!IsServer) { terminate; }

    // Has picking this item up this tic not done anything? Trying to pick it
    //  up again won't change anything, so give up now.
    int checkTimer  = Timer() + 1;
    int pln         = PlayerNumber();
    int classNum    = Pickup_ClassNumber(0);

    if (PKP_RefusePickups[pln][index] == checkTimer) { SetResultValue(0); terminate; }

    // This handles the actual inventory-changing part of the pickup.
    //  The stuff below this handles showing pickup messages, item flash, 
    //  removing the pickup from the map, etc.
    //
    // If you want a script to give a class-based item, use Pickup_DoPickup
    //  instead.

    int newIndex = Pickup_DoPickup(index, classNum, dropped);

    // Get result values.

    int didPickup       = PKP_ReturnArray[PARRAY_SUCCEEDED];
    int consume         = PKP_ReturnArray[PARRAY_CONSUME];
    int noconsume       = PKP_ReturnArray[PARRAY_NOCONSUME];
    int didSomething    = PKP_ReturnArray[PARRAY_DIDSOMETHING];

    int forcePickup = PKP_AlwaysPickup[newIndex][classNum+1];

    // What, the pickup did nothing? Don't care. Pick it up anyway.
    if (forcePickup)
    {
        didPickup = true;
        consume   = true;
    }


    // Well this didn't do anything. Refuse to do it again this tic.
    if (!(didPickup || didSomething))
    {
        PKP_RefusePickups[pln][index] = checkTimer;
    }
    
    // So we did the pickup; time to display the pickup message.
    //  Go to pickup_clientmessage.h for details.
    if (didPickup)
    {
        PKP_MessageData[MDATA_CLASSNUM]     = classNum;
        PKP_MessageData[MDATA_ITEMNUM]      = newIndex;
        PKP_MessageData[MDATA_DROPPED]      = dropped;
        PKP_MessageData[MDATA_LOWHEALTH]    = PKP_ReturnArray[PARRAY_LOWHEALTH];

        Pickup_SendMessage();
    }

    SetResultValue(consume && !noconsume);
}
