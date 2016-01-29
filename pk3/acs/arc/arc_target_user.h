/*
 * ===========================================================================
 * ======================= IMPORTANT THING LOOKEE HERE =======================
 * 
 * This is where everything in arc_target_core.h should be used.
 *
 * In fact, this is the *only* place where anything in arc_target_core.h
 *  should be used.
 *
 * Here, you can specify whatever functions you need to manipulate thing
 *  targeting junk, to use wherever else in the arc code.
 *
 * This file also serves as example code for how to use arc_target_core.h.
 *
 * All functions in here should start with "Target_".
 * 
 * All functions and constants in here can be freely accessed elsewhere.
 *
 * You should never need to modify TargetData directly. Reading from it is fine.
 *
 * ===========================================================================
 */

// Notes:
//
//  - Switch ThingCount to IsTIDUsed once Zandronum 3.0 comes out.
//


// How functions in here fit together:
//
// void Target_Debug(int past)
//  - Prints out the TargetData array up to TargetCount + <past> to the console.


// Column names for Target_Debug.

int TargetColumnNames[T_COLS] =
{
    "TID           ",
    "orig TID      ",
    "state         ",
    "started friend",
    "[slot] hit    ",
    "[slot] checked",
};

// Also for Target_Debug.

#define TD_INT  0
#define TD_BOOL 1
#define TD_STR  2

int TargetColumnTypes[T_COLS] = {TD_INT, TD_INT, TD_BOOL, TD_BOOL, TD_BOOL, TD_BOOL};


// Sets slots on the current target according to two given slot masks
//  (column masks, but only the slot columns are read). The first one
//  turns any slot in its mask on. The second one turns any slot in its
//  mask off. On overrides off.
//
// Then it applies slots.
//
// TID 0 is considered activator TID, rather than erroring.
//
// This function will automatically create a row for the target if it doesn't
//  have one.
//
// Example:
//
// -----------------------------------------------------------------------------
//
// int slotOn  = 0;
// int slotOff = 0;
//
// slotOn  |= TS_HIT;
// slotOff |= TS_CHECKED;
//
// Target_SetSlots(12345, slotOn, slotOff);
//
// // T_HIT is now set on, and T_CHECKED is set off
//
// -----------------------------------------------------------------------------
//
// Return values:
//  - 0: Slot mask set properly.

function int Target_SetSlots(int tid, int slotmask_on, int slotmask_off)
{
    int i;

    // The reason why we aren't doing defaultTID here is because we already
    //  did that with Target_Add. Presumably their TID is already unique,
    //  especially since arc execution should be contiguous
    //  and occur instantly.

    if (tid == 0) { tid = defaultTID(-1); }

    int tidRow = TGT_Find(tid);

    if (tidRow == -1)
    {
        tidRow = TGT_Add(tid);
    }

    int combinedMask;

    for (i = T_SLOTSTART; i < T_COLS; i++)
    {
        int curMask = 1 << i;
        int onState  = slotmask_on  & curMask;
        int offState = slotmask_off & curMask;

        // combinedMask is for either on or off
        if (onState || offState)
        {
            combinedMask |= curMask;
        }

        if (onState)       { TGT_DataArray[i] = true; }
        else if (offState) { TGT_DataArray[i] = false; }
    }

    TGT_SetDataByArray(tidRow, combinedMask);
    TGT_ApplySlots(tidRow);

    return 0;
}

// For when you can't be arsed with slot masks. This sets the specified slot
//  mask to the specified state, then applies slots if it succeeded. Returns
//  whatever TGT_SetDataByTID returns.
//
// This function will automatically create a row for the target if it doesn't
//  have one.
//
//  Return values:
//   - 0: Slot set properly.
//
//   - 3: Slot out of range.

function int Target_SetSlot(int tid, int slot, int state)
{
    if (tid == 0) { tid = defaultTID(-1); }
    if (slot < T_SLOTSTART || slot >= T_COLS) { return 2; }

    int tidRow = TGT_Find(tid);

    if (tidRow == -1)
    {
        tidRow = TGT_Add(tid);
    }

    int ret = TGT_SetData(tidRow, slot, state);

    if (state)
    {
        TGT_FlipFriend(tidRow, true);
    }
    else
    {
        TGT_ApplySlots(tidRow);
    }

    return ret;
}



// Clear a slot for everyone and prune.
//
// Return value is the amount of pruned targets.
//
// Alternately, return value is -1 when the slot is out of range.

function int Target_ClearSlot(int slot)
{
    if (slot < T_SLOTSTART || slot >= T_COLS) { return -1; }

    int i;

    for (i = 0; i < TargetCount; i++)
    {
        TGT_SetData(i, slot, false);
    }

    return TGT_Prune();
}


// It is literally TGT_Clear.
//
// Returns whatever TGT_Clear does.

function int Target_Clear(void)
{
    return TGT_Clear();
}



// Logs debug info for TargetData, up to TargetCount + <past>.
//
// Example output format:
//
//  ---------------------------------------------------------------------------
//
//    == TARGETDATA DEBUG ==
//
//    == ROW 0 ==
//
//  TID            - 17000
//  orig TID       - 0
//  state          - true
//  started friend - false
//  [slot] hit     - false
//  [slot] checked - true
//
//    ======
//
//  1 row found in TargetData (showing 1)
//
//    == DONE ==
//
//  ---------------------------------------------------------------------------

function void Target_Debug(int past)
{
    int rowsToPrint = TargetCount + past;

    Log(s:"\n  == TARGETDATA DEBUG ==\n");

    int row;

    for (row = 0; row < rowsToPrint; row++)
    {
        Log(s:"\n  == ROW ", d:row, s:" ==\n");

        int col;

        for (col = 0; col < T_COLS; col++)
        {
            int colVal  = TargetData[row][col];
            int colType = TargetColumnTypes[col];
            int colName = TargetColumnNames[col];

            int colStr;

            switch (colType)
            {
                case TD_INT:  colStr = StrParam(d:colVal); break;
                case TD_BOOL: colStr = cond(colVal, "true", "false"); break;
                case TD_STR:  colStr = colVal; break;
            }

            Log(s:colName, s:" - ", s:colStr);
        }
    }

    Log(s:"\n  =====\n");
    Log(d:TargetCount, s:" row", s:cond(TargetCount == 1, "", "s"), s:" found in TargetData (showing ", d:rowsToPrint, s:")");
    Log(s:"\n  == DONE ==\n");
}
