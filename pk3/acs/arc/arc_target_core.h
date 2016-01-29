/*
 * ===========================================================================
 * ======================= IMPORTANT THING LOOKEE HERE =======================
 * 
 * This is where core functions required for arc_target_user.h are located.
 *  You probably don't need to modify anything in this file.
 *
 * In fact, unless you just thought "fuck you, yes I do" to that, you don't
 *  need to modify anything in here.
 *
 * All functions here should be preceded by "TGT_".
 *
 * All functions and constants here should ONLY be used by arc_target_user.h.
 *
 * Nothing in arc_target_user.h should ever directly access TargetData.
 *
 * ===========================================================================
 */

// Important notes:
//
// - Column masks can only handle up to 32 columns. Should you exceed this amount
//   of columns, you'll need to expand TGT_SetDataByArray to allow multiple
//   column masks - specifically, (T_COLS/32) column masks.
// 
// - Switch +FRIENDLY to +NEVERTARGET once Zandronum 3.0 comes out.


// How functions in here fit together:
//
// int TGT_Find(int tid)
//  - Given a TID, find the target data row it occupies.
//
//  Returns: Row index for TID, -1 if nonexistent or if TID was 0.
//
//
// int TGT_Add(int tid)
//  - Add a given TID to the target data table.
//
//  Returns: Row index added to if added, negative row index if already present,
//           T_ROWS if TID was 0.
//
//
// int TGT_Del(int row)
//  - Delete a given row in the target data table.
//
//  Returns: 0 if delete successful, 1 if row is out of bounds.
//
//
// int TGT_DelTID(int tid)
//  - Delete a given TID from the target data table.
//
//  Returns: 0 if delete successful, 1 if TID not found, 2 if TID was 0, 3 if
//           things went horribly wrong and you should start panicking.
//
//
// int TGT_SetData(int row, int column, int val)
//  - Set a column in the target data table to a given value for the given row.
//    Setting T_TID is expliclty disallowed.
//
//  Returns: 0 if set properly, 1 if row was out of range, 3 if the column to be
//           set was out of range, 4 if the column was T_TID.
//
//
// int TGT_SetDataByTID(int tid, int column, int val)
//  - Set a column in the target data table to a given value for the given TID.
//    Setting T_TID is expliclty disallowed.
//
//  Returns: 0 if set properly, 1 if TID not found, 2 if TID was 0, 3 if the
//           column to be set was out of range, 4 if the column was T_TID.
//
//
// int TGT_DataArray[T_COLS];
//  - Data array read by TGT_SetDataByArray according to the column mask passed
//    to it. Same layout as TargetData columns.
//
// int TGT_SetDataByArray(int row, int colmask)
//  - Sets an entire row of target data, save T_TID, for the given row based off
//    of a given column mask and the values of TGT_DataArray. This exists because
//    setting an entire row at once is quicker than using TGT_SetDataByTID repeatedly
//    when you need it.
//
//  Returns: 0 if columns were set properly, 1 if row was out of range.
//
//
// int TGT_FlipFriend(int row, int state)
//  - Gives friendliness/unfriendliness item to the TID on the given row.
//
//  Returns: 0 if friendliness state was set properly, 1 if row was out of bounds.
//
//
// int TGT_ApplySlots(int row)
//  - Give out friendliness/unfriendliness items to given row based off slot status
//    in the target data. Only gives them if status changed.
//
//  Returns: -1 if TID was not found, 0 (false) if target at original friendliness,
//           1 (true) if set to opposite friendliness, 2 is TID is 0, and 3 if
//           it's time to start panicking again.
//
//
// int TGT_Prune(void)
//  - Clear out any targets that have no slots set on them.
//
//  Returns: amount of targets that got pruned.
//
//
// int TGT_Clear(void)
//  - Clear out TargetData entirely.
//
//  Returns: amount of targets that *were* in TargetData.
//
//
//
// 
// If anything here contradicts the definitions above the functions themselves,
//  the definitions above the functions take precedence.


// TARGETSLOTS defines how many slots are defined for targeting purposes.
//
// Target slots are used to determine whether an enemy should be marked +FRIENDLY
// or -FRIENDLY. If even one of them is set, the target's friendliness is flipped.
// If all of them are unset, it's flipped back to what it was normally.
//
// The reason this is necessary is so that the arc won't pick up unwanted targets
// multiple times with A_LookEx.
//
// The reason why this complicated system is necessary is because the last time I
// tried this, friendliness tended to break. A lot. It probably still will, since
// dead actors don't show up on ThingCount and TIDs can be shared with actors,
// but there really isn't much of anything I can do about that.
//
// I hate TIDs. I hate juggling them around. I hate what they do to my code.
// I hate the unexplainable bugs that arise from them, the inconsistencies in
// how they're handled, how in the 17 years that ZDoom has had ACS there has been
// no better solution made, how I can't even be goddamn sure that the TID I assign
// to an actor will be *unique* or even *be there* next tic. I hate the fact that
// by changing a TID on *anything*, I am breaking multiple maps out there. I hate
// the dipshit developers who have refused to put in anything better for the past
// 17 years. The whole goddamn system can go to hell.
//
// Just once - just fucking once - I want my code to not fall apart into a pile
// of special cases and incomprehensible gibberish the moment it runs into them.
// I want to be sure that I'm working with the right actor, that I'm not
// accidentally moving some random demon or teleporter halfway across the map.
// 
// Is that seriously so much to ask for?
//
// Anyway, enough ranting. Here's what the slots are for.
//
// HIT      - used when a target's been hit by an arc
//            can be unset automatically or manually, but always gets cleared
//            every tic, at some point during the tic
//
// CHECKED  - used when looking for targets - this makes them untargetable so
//            A_LookEx doesn't get stuck on them during an arc cycle

#define TARGETSLOTS 2

#define TSLOT_HIT       0
#define TSLOT_CHECKED   1



// Columns for TargetData!
//
// T_ROWS is how many targets can be stored. It's high so things don't go bad.
//
// T_COLS just defines how many columns there are.
//
// T_TID corresponds to the TID of the actor being looked at.
//
// T_ORIGTID corresponds to the original TID of the actor being looked at. It's
//  normally equal to T_TID, since it's set to the same value as T_TID in TGT_Add,
//  but it can be set elsewhere, presumably to avoid breaking maps horribly by
//  changing a demon's TID from, say, 5 to 26001. Did I mention I hate TIDs?
//
// T_CURSTATE is there to mark whether the enemy's friendliness is currently flipped
//  or not. Friendliness works in mysterious ways; apparently, making a monster
//  -FRIENDLY multiple times adds to the monster counter every time.
//  False means "original" and true means "flipped".
//
// T_ALREADYFRIEND marks whether the target started friendly or not. This is to
//  reverse friendliness behaviour.
//
// T_SLOTSTART is the column at which the target slots above start at.
//  From that column on, everything is a target slot.

#define T_ROWS  65536
#define T_COLS  (4 + TARGETSLOTS)

#define T_TID               0
#define T_ORIGTID           1
#define T_CURSTATE          2
#define T_ALREADYFRIEND     3
#define T_SLOTSTART         4

// These are for easier access to the slot columns.

#define T_HIT               (T_SLOTSTART + TSLOT_HIT)
#define T_CHECKED           (T_SLOTSTART + TSLOT_CHECKED)



// These are the slot mask constants for TGT_SetDataByArray. Combine these with
//  bitwise OR "|" to create the slot mask you want to use to set data with.
//  These correspond with the above columns directly.
//
// TS_ALL specifies all of the columns.
//
// TS_TID isn't included because T_TID should not be changed, only read. The
//  only two functions that should alter T_TID are TGT_Add and TGT_Del. If
//  anything else alters them, duplicate entries could be made, with undefined
//  behaviour.
//
// Alternately, just use the constants directly (1 << colIndex), but that's
//  less readable.

#define TS_ALL              ((1 << T_COLS) - 1)

#define TS_ORIGTID          (1 << T_ORIGTID)
#define TS_CURSTATE         (1 << T_CURSTATE)
#define TS_ALREADYFRIEND    (1 << T_ALREADYFRIEND)

#define TS_HIT              (1 << T_HIT)
#define TS_CHECKED          (1 << T_CHECKED)



// How many targets are currently stored in TargetData. This is used not
//  just to keep track of how many targets there are, but also to keep
//  TargetData contiguous - when a target is removed, the topmost target
//  gets moved down into the now-empty slot. This way, when adding a target
//  we can just slap it on the end without looking for an empty slot.
//
//  The less time spent traversing the array, the better.

int TargetCount;



// This stores the states of enemies that have been set +FRIENDLY as a result
//  of the arc code. Don't touch it manually; down that road lies madness.
//
//  Trust me on that one. I'm rewriting this for a reason.

int TargetData[T_ROWS][T_COLS];



// Without this, TGT_Find takes O(n) time to run, where n's TargetCount. This
//  quickly became unacceptable, given the amount of times TGT_Find gets called.
//
// When TGT_Add and TGT_Del manipulate rows, they'll go into TargetLookup and
//  set index <TID> to the TID's row + 1.
//
// Why +1? World arrays initialize with everything set to 0 (I guess more
//  accurately, each index is set to 0 when you first access it), but that 
//  can't mean row 0 or else every uninitialized TID is on row 0 according to
//  this. That's dumb. So now on a lookup, 0 gets shifted to -1, which is
//  obviously impossible as an array index. 1 gets shifted to 0, so that means
//  row 0.

world int 45:TargetLookup[];



// Find the index of a given TID in TargetData. If it doesn't exist,
//  return -1.
//
// Used for many things, like making sure no duplicate TIDs end up in the
//  array. +FRIENDLY is very fragile, so I'm taking precautions.
//
// There was a "int checkTid = TargetData[i][T_TID];" here. I took it out.
//  Instruction count for looking dropped by 33%. Thanks, ACS.
//
// Special case: TID 0 always returns -1, because messing with TID 0 is horrible.

function int TGT_Find(int tid)
{
    if (tid == 0) { return -1; }

    int i;

    // Do the fast way first.
    
    int lookup = TargetLookup[tid] - 1;
    if (lookup == -1) { return -1; }
    if (TargetData[lookup][T_TID] == tid) { return lookup; }

    // Now the slow way, if the target lookup got the wrong index.

    //for (i = 0; i < TargetCount; i++)
    //{
    //    if (tid == TargetData[i][T_TID]) { return i; }
    //}

    return -1;
}



// Add a target to TargetData, with no slots set yet, and return the
//  index of the target. All other columns are set to appropriate values.
//
// If the target already exists in TargetData, return the index as a
//  negative number. That way, the caller knows it already existed, but
//  doesn't need to do another call to find it.
//
// This doesn't actually change any +FRIENDLY/-FRIENDLY flags: that is
//  handled elsewhere.
//
// Special case: TID 0 returns T_ROWS as the index, since it's guaranteed
//  to be out-of-bounds and no you're not messing with TID 0 seriously stop.

function int TGT_Add(int tid)
{
    if (tid == 0) { return T_ROWS; }

    int curRow = TGT_Find(tid);
    if (curRow != -1) { return -curRow; }

    curRow = TargetCount;

    TargetData[curRow][T_TID]            = tid;
    TargetData[curRow][T_ORIGTID]        = tid;
    TargetData[curRow][T_CURSTATE]       = false;
    TargetData[curRow][T_ALREADYFRIEND]  = GetActorProperty(tid, APROP_Friendly);

    // Add +1 to the row because world arrays start at 0, and we want that to
    //  mean "uninitialized", not "row 0".

    TargetLookup[tid] = curRow + 1;

    // Guess what? The above line might not actually grab the right actor!
    //  GetActorProperty for TIDs with multiple actors assigned to them is
    //  *completely undefined*!
    //
    // Yes, something as *checking friendliness* can cause bugs to appear.
    
    int i;

    for (i = T_SLOTSTART; i < T_COLS; i++)
    {
        TargetData[curRow][i] = false;
    }

    TargetCount += 1;

    return curRow;
}



// Clears a target by row, and pulls the top row down to the deleted slot to
//  preserve contiguity.
//
// Note that we don't actually clear out the top row, we just copy its contents
// over, since TGT_Add clears it anyway when it gets back to it.
//
// It also restores T_ORIGTID if it's different from T_TID, unless T_ORIGTID was 0.
//
// And just in case, we make sure that friendliness is set back to normal when
//  we delete.
//
// Return values:
//
// - 0: Success, yay.
//
// - 1: Row given was out of bounds, and therefore nothing was done.

function int TGT_Del(int row)
{
    if (row < 0 || row >= TargetCount) { return 1; }

    // First, reset everything on the TID getting deleted.

    TGT_FlipFriend(row, false);

    int curTID  = TargetData[row][T_TID];
    int origTID = TargetData[row][T_ORIGTID];

    TargetLookup[curTID] = 0;

    if ((origTID != 0) && (curTID != origTID)) { Thing_ChangeTID(curTID, origTID); }

    // Now delete.

    TargetCount -= 1;

    // if we deleted the last row, we don't need to copy anything over
    if (row != TargetCount)
    {
        int i;

        for (i = 0; i < T_COLS; i++)
        {
            TargetData[row][i] = TargetData[TargetCount][i];
        }

        int newRowTID = TargetData[row][T_TID];
        TargetLookup[newRowTID] = row + 1;
    }

    return 0;
}



// Clears a target by TID. Uses TGT_Del to do it.
//
// Return values:
//
// - 0: TID was found and deleted.
//
// - 1: The TID was not found, and nothing was deleted.
//
// - 2: The TID was 0.
//
// - 3: Something went horribly wrong: somehow, the TID was found, and it
//      was out of bounds despite TGT_Find only looking within the bounds.
//      If this happens, be scared.

function int TGT_DelTID(int tid)
{
    if (tid == 0) { return 2; }

    int curRow = TGT_Find(tid);
    if (curRow == -1) { return 1; }

    int result = TGT_Del(curRow);

    // This should never happen.
    if (result == 1) { return 3; }

    return 0;
}



// Sets the given column of data on a row to the given value.
//  This doesn't do any auto-pruning, nor does it apply anything.
//  Setting T_TID is not allowed, because this could create duplicates.
//
// Return values:
//
// - 0: Column was set properly.
//
// - 1: Row was out of range.
// 
// - 2: Column was out of range.
//
// - 3: Column was T_TID, which is expliclty disallowed.

function int TGT_SetData(int row, int column, int val)
{
    if (column == T_TID) { return 3; }
    if (column < 0 || column >= T_COLS) { return 2; }
    if (row < 0 || row >= TargetCount) { return 1; }

    TargetData[row][column] = val;

    return 0;
}
    

// Sets the given column of data for a TID to the given value.
//  This doesn't do any auto-pruning, nor does it apply anything.
//  Setting T_TID is not allowed, because this could create duplicates.
//
// Return values:
//
// - 0: Column was set properly.
//
// - 1: TID was not found.
//
// - 2: TID was 0.
// 
// - 3: Column was out of range.
//
// - 4: Column was T_TID, which is expliclty disallowed.

function int TGT_SetDataByTID(int tid, int column, int val)
{
    if (column == T_TID) { return 4; }
    if (column < 0 || column >= T_COLS) { return 3; }
    if (tid == 0) { return 2; }

    int curRow = TGT_Find(tid);
    if (curRow == -1) { return 1; }

    TargetData[curRow][column] = val;

    return 0;
}



// Sets data for a given row of data, given a column mask. See the TS_* constants
//  at the top of the file for the column mask constants you should use.
//
// To use this function, you set the appropriate values in TGT_DataArray, then
//  call the function with the proper column mask. 
// 
// For example, to set T_ORIGTID and T_CHECKED, you'd do something like this:
//
//      TGT_DataArray[T_ORIGTID] = 12345;
//      TGT_DataArray[T_CHECKED] = true;
//      TGT_SetDataByArray(row, TS_ORIGTID | TS_CHECKED);
//
// Again, you aren't allowed to change T_TID, to guarantee each TID is only found
//  once in the target data.
//
// You *could* set index T_TID of TGT_DataArray, but it just gets ignored.
//
// Return values:
//
// - 0: Columns were set properly.
//
// - 1: Row was out of range.

int TGT_DataArray[T_COLS];

function int TGT_SetDataByArray(int row, int colmask)
{
    if (row < 0 || row >= TargetCount) { return 1; }

    int i, checkmask;
    for (i = 0; i < T_COLS; i++)
    {
        // preserve target data integrity
        if (i == T_TID) { continue; }

        // check if setting this column
        checkmask = 1 << i;
        if (!(colmask & checkmask)) { continue; }

        TargetData[row][i] = TGT_DataArray[i];
    }

    return 0;
}

        

// Flips the friendliness of the TID on the given row to the given state.
//
// Return values:
//
// - 0: Friendliness state was set properly.
// 
// - 1: Row was out of bounds, nothing changed.

function int TGT_FlipFriend(int row, int state)
{
    if (row < 0 || row >= TargetCount) { return 1; }

    int curState = TargetData[row][T_CURSTATE];
    int curTID   = TargetData[row][T_TID];

    if (curState != state)
    {
        // flip end state if started as friend

        if (TargetData[row][T_ALREADYFRIEND])
        {
            if (state) { GiveActorInventory(curTID, "ArcUnfriendly", 1); }
            else       { GiveActorInventory(curTID, "ArcFriendly",   1); }
        }
        else
        {
            if (state) { GiveActorInventory(curTID, "ArcFriendly",   1); }
            else       { GiveActorInventory(curTID, "ArcUnfriendly", 1); }
        }
    }
    
    TargetData[row][T_CURSTATE] = state;

    return 0;
}



// Checks the values of all the slots set on a TID, and if any of them are on, flips
//  its friendliness.
//
// Return values:
//
// - -1: Row out of range.
// 
// -  0 (false): Target is set to original friendliness.
//
// -  1 (true):  Target is set to opposite friendliness.

function int TGT_ApplySlots(int row)
{
    if (row < 0 || row >= TargetCount) { return -1; }

    int flipFriendliness = false;
    int i;

    for (i = T_SLOTSTART; i < T_COLS; i++)
    {
        if (TargetData[row][i])
        {
            flipFriendliness = true;
            break;
        }
    }

    TGT_FlipFriend(row, flipFriendliness);
    return flipFriendliness;
}


// Clears out any entries with no target slots set.
//
// We start from TargetCount - 1 because, since we pull rows from the top down,
//  any row that gets pulled down from a deletion will have already been covered,
//  simplifying the code just a little bit.
//
// Plus, if the top rows get deleted, this saves some cycles copying.
//
// Return value is the amount of targets that got pruned.

function int TGT_Prune(void)
{
    int pruneCount = 0;
    int curRow;

    if (TargetCount == 0) { return 0; }

    for (curRow = TargetCount - 1; curRow >= 0; curRow--)
    {
        int isFlipped = false;
        int i;

        for (i = T_SLOTSTART; i < T_COLS; i++)
        {
            if (TargetData[curRow][i])
            {
                isFlipped = true;
                break;
            }
        }

        if (!isFlipped)
        {
            TGT_Del(curRow);
            pruneCount += 1;
        }
    }

    return pruneCount;
}

// Just clear out the entirety of TargetData. We start from the top so that
//  TGT_Del doesn't spend any time copying things over.
//
// Returns the amount of entries that *were* in TargetData.

function int TGT_Clear(void)
{
    int ret = TargetCount;
    int curRow;

    if (ret == 0) { return 0; }

    for (curRow = TargetCount - 1; curRow >= 0; curRow--)
    {
        TGT_Del(curRow);
    }

    return ret;
}
