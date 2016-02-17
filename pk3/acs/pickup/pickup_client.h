// Server-to-client definitions here.
//
// S2C is for constant definitions, SToC is for array definitions, and Sender is
// for function definitions. Scripts use the PICKUP prefix like all other scripts.
//
// This is mainly to avoid same-name issues.


// When the server sends data to the client, it will wait this many tics. If it
//  hasn't gotten a response from the client indicating it got the right data
//  by then, it'll resend the data-setting script.
#define S2C_RESENDTIME  36


// This is where the data's stored on the server side. One row for each player,
//  one column for each data point.
int SToC_ServerData[PLAYERMAX][S2C_DATACOUNT];


// This is for the clients. For Zandronum, it technically could just be a 1D
//  array, but that makes me feel uneasy about the ZDoom scenario. So you get
//  a 2D array.
int SToC_ClientData[PLAYERMAX][S2C_DATACOUNT];


// In order to avoid packet loss issues, the clients ping back to the server
//  when they recieve a data-setting script. We store their responses here.
//
// Until the data in SToC_ServerData and here match, the server will re-send
//  the packet every second, or however long specified in S2C_RESENDTIME.
int SToC_LastClientResponse[PLAYERMAX][S2C_DATACOUNT];


// This is where it stores the time the server last sent a data ping.
int SToC_LastSendTime[PLAYERMAX][S2C_DATACOUNT];


// This is for the clients, to keep track of when we got the last ping from a
//  server.
int SToC_LastReceiveTime[PLAYERMAX][S2C_DATACOUNT];


// Okay, so one bug I ran into right away: not only does ACS_ExecuteAlways get
// sent in reverse order, but so does RequestScriptPuke. This won't do.
//
// So instead of sending directly, the update functions instead add to this,
//  and another intermediate function does the sending.
int SToC_ToSend[PLAYERMAX][S2C_DATACOUNT];





// ========
// == FUNCTION DEFINITIONS
// ========


// Are we in Zandronum?
// Has the side effect of setting IsZandronum if it's -1 (ie. uninitialized).
function int Pickup_IsZandronum(void)
{
    if (IsZandronum == -1)
    {
        SetDBEntry("pickup", "client_iszand", true);
        IsZandronum = GetDBEntry("pickup", "client_iszand") == 1;
    }
    
    return IsZandronum;
}



// Base function for setting server-to-client data.
function void Sender_SetData(int pln, int index, int value)
{
    int oldval = SToC_ServerData[pln][index];
    if (oldval == value) { return; }

    SToC_ServerData[pln][index]     = value;
    SToC_LastSendTime[pln][index]   = -1;
}



// Clear all server-to-client data for the specified player number. This is
//  called on disconnect to make sure no data propagates to the next client
//  accidentally.
function void Sender_ClearData(int pln)
{
    int i;

    for (i = 0; i < S2C_DATACOUNT; i++)
    {
        Sender_SetData(pln, i, 0);
    }
}



// Base function to send data from server to client.
// 
// Note that it doesn't actually *send* anything. It only adds it to an array,
//  to be sent by another function.
//
// This is to avoid dumb issues with scripts being last-in-first-out when it
//  comes to executing.
function void Sender_SendData(int pln, int index)
{
    SToC_ToSend[pln][index] = true;
}



// Check if we need to send any data to anyone, and if so, do so.
function void Sender_UpdateClients(void)
{
    int i, j;

    for (i = 0; i < PLAYERMAX; i++)
    {
        // Zandyland has spectators too, and they can also ping back
        if (IsZandronum)
        {
            if (!(PlayerInGame(i) || PlayerIsSpectator(i))) { continue; }
        }
        else
        {
            if (!PlayerInGame(i)) { continue; }
        }

        for (j = 0; j < S2C_DATACOUNT; j++)
        {
            int data_server = SToC_ServerData[i][j];
            int data_client = SToC_LastClientResponse[i][j];

            if (data_server == data_client) { continue; }

            int lastSend    = SToC_LastSendTime[i][j]; 

            if ((lastSend < 0) || (Timer() - lastSend > S2C_RESENDTIME))
            {
                Sender_SendData(i, j);
            }
        }
    }
}



// Force a data send for a certain player number.
function void Sender_ForceUpdateClient(int pln)
{
    int i;

    for (i = 0; i < S2C_DATACOUNT; i++)
    {
        Sender_SendData(pln, i);
    }
}



// Okay NOW send the crap. This should only ever be run by PICKUP_MAINLOOP.
function void Sender_ActuallySend(void)
{
    int i, j;

    int useAlways = IsZandronum && ConsolePlayerNumber() == -1;

    for (i = 0; i < PLAYERMAX; i++)
    {
        for (j = 0; j < S2C_DATACOUNT; j++)
        {
            if (SToC_ToSend[i][j])
            {
                int data = SToC_ServerData[i][j];

                if (useAlways)
                {
                    ACS_ExecuteAlways(PICKUP_SENDTOCLIENT, 0, i, j, data);
                }
                else
                {
                    ACS_ExecuteWithResult(PICKUP_SENDTOCLIENT, i, j, data);
                }

                SToC_LastSendTime[i][j] = Timer();

                SToC_ToSend[i][j] = false;
            }
        }
    }
}



// Ping back from the client to server, to let the server know you got the data
//  properly. In single player/ZDoom at all, just set the value directly.
function void Sender_PingBack(int pln, int index, int value)
{
    if (Pickup_IsZandronum() && !IsServer)
    {
        // Don't send player number since RequestScriptPuke will always run
        //  on ConsolePlayer on the server's end
        //
        // The server won't go by what the script arguments say for player
        //  number because one could spoof pings really easily then - it's
        //  harmless in this case, but annoying
        RequestScriptPuke(PICKUP_PINGSERVER, index, value, 0);
    }
    else
    {
        // Eh fuck it, we'll do it ourself
        SToC_LastClientResponse[pln][index] = value;
    }
}
    

// ========
// == SCRIPT DEFINITIONS
// ========


// Called by the server to send data. Recieved by the client to get that data.
script PICKUP_SENDTOCLIENT (int pln, int index, int value) clientside
{
    int cpln = ConsolePlayerNumber(); // always 0 in ZDoom

    // Add 1 to this so that 0 means "we never got anything", not "tic 0"
    SToC_LastReceiveTime[pln][index] = Timer() + 1;
    SToC_ClientData[pln][index] = value;

    // Don't send pingbacks for data that isn't ours.
    if (pln == cpln)
    {
        Sender_PingBack(pln, index, value);
    }
}


// Called by the client to send data back to the server. Recieved by the server
//  to get that data. This is only called in Zandronum online, unless the player
//  is puking scripts for some reason.
script PICKUP_PINGSERVER (int index, int value) net
{
    int pln = PlayerNumber();
    SToC_LastClientResponse[pln][index] = value;
}
