#define T_ROWS 65536

world int 43:T_FriendState[];
world int 44:THit_TIDToRow[];

int THit[T_ROWS];

int THitCount = 0;

world int 45:TCheck_TIDToRow[];

int TCheck[T_ROWS];

int TCheckCount = 0;


function void T_SetFriend(int tid, int state)
{
    int makeFriend  = (!!state) ^ T_FriendState[tid];
    int friendState = GetActorProperty(tid, APROP_Friendly); 

    if (friendState == makeFriend) { return; }

    if (makeFriend) { GiveActorInventory(tid, "ArcFriendly",   1); }
    else            { GiveActorInventory(tid, "ArcUnfriendly", 1); }
}

function int THit_FindTID(int tid)
{
    int ret = THit_TIDToRow[tid] - 1;
    if (ret >= THitCount) { return -1; }

    if (THit[ret] != tid) { return -1; }
    return ret;
}

function int TCheck_FindTID(int tid)
{
    int ret = TCheck_TIDToRow[tid] - 1;
    if (ret >= TCheckCount) { return -1; }
    return ret;
}

function int THit_Add(int tid)
{
    int row = THitCount++;

    THit[row]           = tid;
    THit_TIDToRow[tid]  = row + 1;

    int otherRow = TCheck_FindTID(tid);

    if (otherRow == -1)
    {
        T_FriendState[tid] = GetActorProperty(tid, APROP_Friendly);
        T_SetFriend(tid, true);

        if (T_FriendState[tid])
        {
            GiveActorInventory(tid, "ArcThingCountTest", 1);
        }
    }

    return row;
}

function int TCheck_Add(int tid)
{
    int row = TCheckCount++;

    TCheck[row]             = tid;
    TCheck_TIDToRow[tid]    = row + 1;

    int otherRow = THit_FindTID(tid);

    if (otherRow == -1)
    {
        T_FriendState[tid] = GetActorProperty(tid, APROP_Friendly);
        T_SetFriend(tid, true);

        if (T_FriendState[tid])
        {
            GiveActorInventory(tid, "ArcThingCountTest", 1);
        }
    }
    
    return row;
}

function void THit_Clear(void)
{
    int i;

    for (i = 0; i < THitCount; i++)
    {
        int tid = THit[i];
        int otherRow = TCheck_FindTID(tid);

        if (otherRow == -1)
        {
            T_SetFriend(tid, false);

            if (GetActorProperty(tid, APROP_Health) <= 0)
            {
                Thing_ChangeTID(tid, 0);
            }
        }

        THit_TIDToRow[tid] = 0;
    }
    
    THitCount = 0;
}

function void TCheck_Clear(void)
{
    int i;

    for (i = 0; i < TCheckCount; i++)
    {
        int tid = TCheck[i];
        int otherRow = THit_FindTID(tid);

        if (otherRow == -1)
        {
            T_SetFriend(tid, false);

            if (GetActorProperty(tid, APROP_Health) <= 0)
            {
                Thing_ChangeTID(tid, 0);
            }
        }

        TCheck_TIDToRow[tid] = 0;
    }

    TCheckCount = 0;
}

function void T_Clear(void)
{
    THit_Clear();
    TCheck_Clear();
}
