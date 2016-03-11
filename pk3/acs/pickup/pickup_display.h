// Pickup display stuff.

// This is for display scripts. It gets set right before they get called, so
//  it's always safe to use.

#define DPASS_SLOTS 5

// Item number for this pickup.
#define DPASS_ITEMNUM       0

// Previous and current class number, so the script knows if it needs to
//  SetActorState or not.
#define DPASS_CLASSNUM      1
#define DPASS_OLDCLASSNUM   2

// Is the item considered dropped?
#define DPASS_DROPPED       3

// When a script number changes, we use this to signal to it that it should do
//  cleanup.
#define DPASS_DOCLEANUP     4

int DISP_ScriptArgs[DPASS_SLOTS];

int DISP_ScriptDisplayCache[PICKUPCOUNT][CLASSCOUNT];
int DISP_ScriptDisplayCached[PICKUPCOUNT][CLASSCOUNT];


// Copypasta!
function int Pickup_IsDisplayScripted(int index, int classNum)
{
    int i;
    int ret = -1;

    if (DISP_ScriptDisplayCached[index][classNum+1])
    {
        return DISP_ScriptDisplayCache[index][classNum+1];
    }

    for (i = 0; i < DISP_SCRIPTEDCOUNT; i++)
    {
        int checkIndex = DISP_ScriptedDisplays[i][DISP_S_ITEMNUM];
        int checkClass = DISP_ScriptedDisplays[i][DISP_S_CLASSNUM];

        if (index == checkIndex && classNum == checkClass)
        {
            ret = i;
            break;
        }
    }

    DISP_ScriptDisplayCached[index][classNum+1] = true;
    DISP_ScriptDisplayCache[index][classNum+1]  = ret;

    return ret;
}
    

script "Pickup_Display" (int index, int dropped, int firstDisplay)
{
    int cpln = ConsolePlayerNumber();

    if (cpln == -1)
    {
        ACS_NamedExecuteAlways("Pickup_Display_Main", 0, index, dropped, firstDisplay);
    }
    else if (IsServer)
    {
        ACS_NamedExecuteWithResult("Pickup_Display_Main", index, dropped, firstDisplay);
    }
}

script "Pickup_Display_Main" (int index, int dropped, int firstDisplay) clientside
{
    int cpln     = ConsolePlayerNumber();
    // Don't do shit if we're somehow running on the server.
    if (cpln == -1) { terminate; }

    // Only accept the first one of these. That way, the next fifty billion
    //  times the server polls the item, nothing happens and our item script
    //  doesn't break.
    //
    // Special case to handle item respawns: if this is the first display ping,
    //  take away the KickedToClientside item, because the item hasn't been
    //  kicked to clientside yet.
    //
    // Special special case: ignore that if we've been kicked to clientside this
    //  tic, because - guess what! - ZDoom is utterly fucking retarded and will
    //  have ACS_ExecuteAlways scripts run a full tic later... sometimes! For
    //  absolutely no fucking reason!
    //
    // *sigh*

    int clientKick = CheckInventory("Class_KickedToClientside");
    int time = Timer();

    if ((clientKick == time) || (!firstDisplay && clientKick)) { terminate; }

    // Intentionally break sync between client and server for display shit.
    //  This is kinda voodoo magic.
    SetInventory("Class_KickedToClientside", time);

    // Default state to go to.
    SetActorState(0, "Unknown");

    int oldClassNum;
    int classNum = -1;
    int oldScript;
    int scriptIndex = -1;
    int snum, arg1, arg2, arg3, named, name;

    while (true)
    {
        // The script doesn't auto-terminate on pickup, so we need to handle
        //  that ourselves.
        if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }

        oldClassNum = classNum;
        // Subtract 1 so that the default value - 0 - means "unknown".
        classNum = SToC_ClientData[cpln][S2C_D_CLASSNUM] - 1;

        oldScript = scriptIndex;
        scriptIndex = Pickup_IsDisplayScripted(index, classNum);
        
        // Slight optimization.
        if (oldScript != -1 || scriptIndex != -1)
        {
            Disp_ScriptArgs[DPASS_ITEMNUM]      = index;
            Disp_ScriptArgs[DPASS_CLASSNUM]     = classNum;
            Disp_ScriptArgs[DPASS_OLDCLASSNUM]  = oldClassNum;
            Disp_ScriptArgs[DPASS_DROPPED]      = dropped;
        }

        // If class number changes, try to go back to the default display state,
        //  then change pickup state.
        if (oldClassNum != classNum)
        {
            SetActorProperty(0, APROP_RenderStyle, STYLE_Normal);
            SetActorProperty(0, APROP_Alpha,       1.0);

            if (classNum == -1)
            {
                SetActorState(0, "Unknown");
            }
            else
            {
                // DISP_ClassStates is in pickup_items.h.
                SetActorState(0, DISP_ClassStates[classNum]);
            }

            // Let the old script do whatever cleanup it needs.
            if (oldScript != -1)
            {
                Disp_ScriptArgs[DPASS_DOCLEANUP] = true;

                snum = DISP_ScriptedDisplays[oldScript][DISP_S_SCRIPTNUM];
                arg1 = DISP_ScriptedDisplays[oldScript][DISP_S_ARG1];
                arg2 = DISP_ScriptedDisplays[oldScript][DISP_S_ARG2];
                arg3 = DISP_ScriptedDisplays[oldScript][DISP_S_ARG3];

                named = DISP_ScriptedDisplays[oldScript][DISP_S_NAMEDSCRIPT];
                name  = DISP_DisplayNamed[oldScript];

                if (named)
                {
                    ACS_NamedExecuteWithResult(name, arg1, arg2, arg3);
                }
                else
                {
                    ACS_ExecuteWithResult(snum, arg1, arg2, arg3);
                }

                Disp_ScriptArgs[DPASS_DOCLEANUP] = false;
            }
        }

        // If we got a script, let it handle everything. If it wants to
        //  SetActorState every tic, let it. I don't give a fuck!
        if (scriptIndex != -1)
        {
            snum = DISP_ScriptedDisplays[scriptIndex][DISP_S_SCRIPTNUM];
            arg1 = DISP_ScriptedDisplays[scriptIndex][DISP_S_ARG1];
            arg2 = DISP_ScriptedDisplays[scriptIndex][DISP_S_ARG2];
            arg3 = DISP_ScriptedDisplays[scriptIndex][DISP_S_ARG3];

            named = DISP_ScriptedDisplays[scriptIndex][DISP_S_NAMEDSCRIPT];
            name  = DISP_DisplayNamed[scriptIndex];

            if (named)
            {
                ACS_NamedExecuteWithResult(name, arg1, arg2, arg3);
            }
            else
            {
                ACS_ExecuteWithResult(snum, arg1, arg2, arg3);
            }
        }

        Delay(1);
    }
}
