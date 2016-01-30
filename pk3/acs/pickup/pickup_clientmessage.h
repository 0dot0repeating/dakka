// From pickup_pickup.h, the client gets data for a pickup message. This is
//  where it's processed.

// Data that the server sends to the client for pickup messages arrives here.
//  Constant definitions for this are in pickup_pickup.h.

int CMSG_MessageData[PLAYERMAX][MDATA_SLOTS];

// This is so we know what's been sent and what hasn't been.
int CMSG_MessageDefined[PLAYERMAX][MDATA_SLOTS];


// ZDoom has this odd behaviour where pickups of the same type picked up in the
//  same tic only show one message among them... unless there's a pickup in
//  between, in which case it'll display it again.
//
// This replicates that. Entirely scripted pickups ignore it, however. They can
//  do whatever they want.

#define LASTPICKUP_INDEX    0
#define LASTPICKUP_TIME     1

int CMSG_LastPickup[PLAYERMAX][2];




// Add newly-received data to CMSG_MessageData, and return true if every data
//  slot has been filled.
function int CMSG_AddPickupData(int pln, int index, int arg1, int arg2)
{
    // As of Zandronum 2.1.2, Using ACS_ExecuteWithResult with arguments
    //  between -1 and -128 will pass those numbers as 256 + <val> instead.
    //  This is obviously not desirable.
    //
    // We subtracted 128 from it in Pickup_SendMessage, so we add 128 here.
    if (arg1 < 0) { arg1 += 128; }
    if (arg2 < 0) { arg2 += 128; }

    CMSG_MessageData[pln][index]   = arg1;
    CMSG_MessageData[pln][index+1] = arg2;

    CMSG_MessageDefined[pln][index]   = true;
    CMSG_MessageDefined[pln][index+1] = true;

    int i;

    for (i = 0; i < MDATA_SLOTS; i++)
    {
        if (!CMSG_MessageDefined[pln][i]) { return false; }
    }

    return true;
}



// Clear received data on a certain player number, so a new pickup message can
//  come through.
function void CMSG_ClearPickupData(int pln)
{
    int i;

    for (i = 0; i < MDATA_SLOTS; i++)
    {
        CMSG_MessageData[pln][i]    = 0;
        CMSG_MessageDefined[pln][i] = false;
    }
}



// It's the same as the other two IsScripted functions. Returns the script index
//  in CMSG_ScriptedPickups, or -1 if it doesn't exist.
function int CMSG_IsScripted(int index, int classNum)
{
    int i;

    for (i = 0; i < MSG_SCRIPTEDCOUNT; i++)
    {
        int checkIndex = CMSG_ScriptedMessages[i][MSG_S_ITEMNUM];
        int checkClass = CMSG_ScriptedMessages[i][MSG_S_CLASSNUM];

        if (index == checkIndex && classNum == checkClass)
        {
            return i;
        }
    }

    return -1;
}



script PICKUP_SHOWMESSAGE (int mdata_index, int data1, int data2) clientside
{
    // This is the player number of the person picking up the item.
    int pln  = PlayerNumber();

    // This is the client's player number. If it doesn't match pln, don't do
    //  Log messages.
    int cpln = ConsolePlayerNumber();

	int gotAllData = CMSG_AddPickupData(pln, mdata_index, data1, data2);

    // We're still receiving data, don't do anything yet
    if (!gotAllData) { terminate; }

    int classNum = CMSG_MessageData[pln][MDATA_CLASSNUM];
    int itemNum  = CMSG_MessageData[pln][MDATA_ITEMNUM];
    int dropped  = CMSG_MessageData[pln][MDATA_DROPPED];

    int message = "";

    int snum, arg1, arg2, arg3, onlyString;
    int scriptIndex = CMSG_IsScripted(itemNum, classNum);

    if (scriptIndex != -1)
    {
        snum = CMSG_ScriptedMessages[scriptIndex][MSG_S_SCRIPTNUM];
        arg1 = CMSG_ScriptedMessages[scriptIndex][MSG_S_ARG1];
        arg2 = CMSG_ScriptedMessages[scriptIndex][MSG_S_ARG2];
        arg3 = CMSG_ScriptedMessages[scriptIndex][MSG_S_ARG3];

        // If this is true, we just use the script for getting a message.
        onlyString = CMSG_ScriptedMessages[scriptIndex][MSG_S_ONLYSTRING];

        // If we're handling this pickup message entirely through scripted
        //  events, don't bother with the ZDoom pickup message behaviour.
        //
        // Note that CMSG_MessageData is still available for use in the message
        //  script, so we don't need to pass in class number, item number, and
        //  all that.
        //
        // We pass in dropped mainly for consistency. I don't wanna make an
        //  argument array for pickup scripts.
        //
        // If I make one for them anyway, expect to get the
        //  full three arguments.

        if (!onlyString)
        {
            ACS_ExecuteWithResult(snum, arg1, arg2, arg3);
            CMSG_ClearPickupData(pln);
            terminate;
        }
    }

    // Always the "- 1" trick. If Timer() started at 1, things would be nice.
    int lastPickup = CMSG_LastPickup[pln][LASTPICKUP_INDEX];
    int lastTime   = CMSG_LastPickup[pln][LASTPICKUP_TIME]  - 1;

    // Replicate ZDoom pickup message behaviour.
    if (lastTime == Timer() && lastPickup == itemNum)
    {
        terminate;
    }

    if (scriptIndex != -1)
    {
        message = ACS_ExecuteWithResult(snum, arg1, arg2, arg3);

        // If the pickup script doesn't return a message, don't do pickup message behaviour.
        if (message == 0 || StrLen(message) == 0)
        {
            CMSG_ClearPickupData(pln);
            terminate;
        }
    }
    else if (CMSG_MessageData[pln][MDATA_LOWHEALTH] > 0)
    {
        message = PKP_LowHealthMessages[CMSG_MessageData[pln][MDATA_LOWHEALTH] - 1];
    }
    else
    {
        message = PKP_Messages[itemNum][classNum + 1];
    }

    // Whatever the message ended up being, check if it's a LANGUAGE lookup.

    // Explicitly *not* one.
    if (strstr(message, "\\$") == 0)
    {
        message = sliceString(message, 2, StrLen(message));
    }
    else if (GetChar(message, 0) == '$')
    {
        message = StrParam(l:sliceString(message, 1, StrLen(message)));
    }


    // msgColors is in commonFuncs.h.
    // Since Log() prints in white, we need to fix that.
    int messageColor = msgColors[GetCVar("msg0color")];

    // Can't have \c- breaking shit.
    message = strsub(message, "\c-", messageColor);

    if (pln == cpln)
    {
        Log(s:messageColor, s:message);
    }

    // If silent pickup's on, can't let other people hear it.

    int pickupSound = PKP_PickupSounds[itemNum][classNum + 1];
    int silent      = !!(GetCvar("compat_silentpickup"));

    if (silent)
    {
        // Only for the right player.
        if (pln == cpln) { LocalAmbientSound(pickupSound, 127); }
    }
    else
    {
        ActivatorSound(pickupSound, 127);
    }

    // And pickup flash.

    int r = PKP_PickupFlashes[itemNum][classNum+1][PFLASH_RED];
    int g = PKP_PickupFlashes[itemNum][classNum+1][PFLASH_GREEN];
    int b = PKP_PickupFlashes[itemNum][classNum+1][PFLASH_BLUE];
    int a = PKP_PickupFlashes[itemNum][classNum+1][PFLASH_ALPHA];
    int t = PKP_PickupFlashes[itemNum][classNum+1][PFLASH_TIME];

    FadeRange(r, g, b, a, r, g, b, 0, t);


    // This must be at the end, or else for every two numbers sent from the
    //  server, the client will do another likely malformed pickup message
    CMSG_ClearPickupData(pln);

    CMSG_LastPickup[pln][LASTPICKUP_INDEX] = itemNum;
    CMSG_LastPickup[pln][LASTPICKUP_TIME]  = Timer() + 1;
}

