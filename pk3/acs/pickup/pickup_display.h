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



// Copypasta!
function int Pickup_IsDisplayScripted(int index, int classNum)
{
    int i;

    for (i = 0; i < DISP_SCRIPTEDCOUNT; i++)
    {
        int checkIndex = DISP_ScriptedDisplays[i][DISP_S_ITEMNUM];
        int checkClass = DISP_ScriptedDisplays[i][DISP_S_CLASSNUM];

        if (index == checkIndex && classNum == checkClass)
        {
            return i;
        }
    }

    return -1;
}
    



script PICKUP_DISPLAY (int index, int dropped) clientside
{
    int cpln     = ConsolePlayerNumber();
    // Don't do shit if we're somehow running on the server.
    if (cpln == -1) { terminate; }

    // Only accept the first one of these. That way, the next fifty billion
    //  times the server polls the item, nothing happens and our item script
    //  doesn't break.
    if (CheckInventory("Class_KickedToClientside")) { terminate; }

    // Intentionally break sync between client and server for display shit.
    //  This is kinda voodoo magic.
    GiveInventory("Class_KickedToClientside", 1);

    // Default state to go to.
    SetActorState(0, "Unknown");

    int oldClassNum;
    int classNum = -1;
    int oldScript;
    int scriptIndex = -1;
    int snum, arg1, arg2, arg3;

    while (true)
    {
        // The script doesn't auto-terminate on pickup, so we need to handle
        //  that ourselves.
        if (ClassifyActor(0) & ACTOR_WORLD) { terminate; }

        oldClassNum = classNum;
        classNum = SToC_ClientData[cpln][S2C_D_CLASSNUM];

        // If class number changes, try to go back to the default display state.
        if (oldClassNum != classNum)
        {
            SetActorProperty(0, APROP_RenderStyle, STYLE_Normal);
            SetActorProperty(0, APROP_Alpha,       1.0);
        }

        oldScript = scriptIndex;
        scriptIndex = Pickup_IsDisplayScripted(index, classNum);

        Disp_ScriptArgs[DPASS_ITEMNUM]      = index;
        Disp_ScriptArgs[DPASS_CLASSNUM]     = classNum;
        Disp_ScriptArgs[DPASS_OLDCLASSNUM]  = oldClassNum;
        Disp_ScriptArgs[DPASS_DROPPED]      = dropped;

        // Let the old script that we're phasing out do cleanup.
        if ((oldScript != scriptIndex) && (oldScript != -1))
        {
            Disp_ScriptArgs[DPASS_DOCLEANUP] = true;

            snum = DISP_ScriptedDisplays[oldScript][DISP_S_SCRIPTNUM];
            arg1 = DISP_ScriptedDisplays[oldScript][DISP_S_ARG1];
            arg2 = DISP_ScriptedDisplays[oldScript][DISP_S_ARG2];
            arg3 = DISP_ScriptedDisplays[oldScript][DISP_S_ARG3];
            ACS_ExecuteWithResult(snum, arg1, arg2, arg3);

            Disp_ScriptArgs[DPASS_DOCLEANUP] = false;
        }

        // If we got a script, let it handle everything. If it wants to
        //  SetActorState every tic, let it. I don't give a fuck!
        if (scriptIndex != -1)
        {
            snum = DISP_ScriptedDisplays[scriptIndex][DISP_S_SCRIPTNUM];
            arg1 = DISP_ScriptedDisplays[scriptIndex][DISP_S_ARG1];
            arg2 = DISP_ScriptedDisplays[scriptIndex][DISP_S_ARG2];
            arg3 = DISP_ScriptedDisplays[scriptIndex][DISP_S_ARG3];

            ACS_ExecuteWithResult(snum, arg1, arg2, arg3);
        }
        else if (classNum != oldClassNum)
        {
            if (classNum == -1)
            {
                SetActorState(0, "Unknown");
            }
            else
            {
                // DISP_ClassStates is in pickup_items.h.
                SetActorState(0, DISP_ClassStates[classNum]);
            }
        }

        Delay(1);
    }
}
