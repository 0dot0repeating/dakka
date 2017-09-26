// Client-to-server definitions here.
//
// C2S is for constant definitions, CToS is for array definitions, and CSender is
// for function definitions. Scripts use the PICKUP prefix like all other scripts.



// Same deal as S2C_RESENDTIME.
#define C2S_RESENDTIME  36

// Client-stored data. We have an array of 64 mainly because ZDoom doesn't have
//  ConsolePlayerNumber, and things should be made to at least work in single-
//  player there.
int CToS_ClientData[PLAYERMAX][C2S_DATACOUNT];


// The server stores whatever responses it got here.
int CToS_ServerData[PLAYERMAX][C2S_DATACOUNT];


// The clients want a pingback for C2S for the same reason servers want a
//  pingback for S2C. This is for clients.
int CToS_LastServerResponse[PLAYERMAX][C2S_DATACOUNT];


// We gotta keep track of when the last packet was sent if we're gonna be
//  sending packets. This is for clients.
int CToS_LastSendTime[PLAYERMAX][C2S_DATACOUNT];


// This is for the server. It just stores when it last got a data packet
//  from a client.
int CToS_LastReceiveTime[PLAYERMAX][C2S_DATACOUNT];


// I already had the reverse order bug bite me in the ass before, so we're
//  doing the same thing here. We only send each data entry once per tic.
int CToS_ToSend[PLAYERMAX][C2S_DATACOUNT];



// For a lot of this I'm just gonna be copypasting from pickup_client, since
//  the concepts are largely the same.




// Base function for setting client-to-server data.
function void CSender_SetData(int pln, int index, int value)
{
    int oldval = CToS_ClientData[pln][index];
    if (oldval == value) { return; }

    CToS_ClientData[pln][index]     = value;
    CToS_LastSendTime[pln][index]   = -1;
}



// There's no need for a CSender_ClearData. OPEN clientside scripts will run
//  for the entire time you're connected to a server.



// Base function to send data from client to server.
// 
// Just like Sender_SendData, this doesn't send anything. It relies on CSender_
// ActuallySend to do that, and for the same reasons as Sender_SendData.

function void CSender_SendData(int pln, int index)
{
    CToS_ToSend[pln][index] = true;
}




// Check if we need to send any data to the server, and if so, do so.
function void CSender_UpdateServer(int pln)
{
    int i, j;

    for (j = 0; j < C2S_DATACOUNT; j++)
    {
        int data_client = CToS_ClientData[pln][j];
        int data_server = CToS_LastServerResponse[pln][j];

        if (data_server == data_client) { continue; }

        int lastSend    = CToS_LastSendTime[pln][j]; 

        if ((lastSend < 0) || (Timer() - lastSend > C2S_RESENDTIME))
        {
            CSender_SendData(pln, j);
        }
    }
}



// Unlike server->client being able to assure that the client's SToC data is
//  completely zeroed out (or able to assume it because he just connected), we
//  can't assume that the server's CToS data for this particular client is
//  zeroed out. So when we first connect, we're gonna send everything. EVERYTHING.
function void CSender_ForceSendAll(int pln)
{
    int i;

    for (i = 0; i < C2S_DATACOUNT; i++)
    {
        // We do this so that the client will *keep* sending until it gets a
        //  correct response.
        CToS_LastServerResponse[pln][i] = CToS_ClientData[pln][i] - 1;
        CSender_SendData(pln, i);
    }
}



// Okay NOW send the crap. This should only ever be run
// by the OPEN CLIENTSIDE script.
function void CSender_ActuallySend(int pln)
{
    int i;

    int useScriptPuke = IsZandronum && !IsServer;

    for (i = 0; i < C2S_DATACOUNT; i++)
    {
        if (CToS_ToSend[pln][i])
        {
            int data = CToS_ClientData[pln][i];

            if (useScriptPuke)
            {
                // RequestScriptPuke will also run on the console player puking
                //  the script, thankfully. No need to worry about player numbers
                //  here.
                NamedRequestScriptPuke("Pickup_SendToServer", i, data, 0);
            }
            else
            {
                // We need this script to run on the player whose data is being sent.
                //
                // Zandronum doesn't do listen servers, thankfully, so if we're
                //  running this clientside stuff on the server,
                //  there's a couple possibiltiies:
                //
                // 1. We're in single player or a botmatch, where the only
                //    player number we need to worry about is 0.
                //
                // 2. We're in ZDoom netplay, where everything's going to shit
                //    anyway.
                //
                // Either way, this is covered by the AAPTR_PLAYER# actor pointers.
                // Really, we only need the PLAYER1 actor pointer!
                
                SetActivator(0, AAPTR_PLAYER1);
                ACS_NamedExecuteWithResult("Pickup_SendToServer", i, data);
            }

            CToS_LastSendTime[pln][i] = Timer();
            CToS_ToSend[pln][i] = false;
        }
    }
}


function void CSender_PingBack(int pln, int index, int value)
{
    if (ConsolePlayerNumber() == -1)
    {
        // Send back to the client that we got the right data
        ACS_NamedExecuteAlways("Pickup_PingClient", 0, pln, index, value);
    }
    else
    {
        // Eh fuck it, we'll do it ourself
        ACS_NamedExecuteWithResult("Pickup_PingClient", pln, index, value);
    }
}



script "Pickup_SendToServer" (int index, int data) net
{
    int pln = PlayerNumber();

    CToS_ServerData[pln][index]      = data;
    CToS_LastReceiveTime[pln][index] = Timer() + 1;

    CSender_PingBack(pln, index, data);
}

script "Pickup_PingClient" (int pln, int index, int value) clientside
{
    CToS_LastServerResponse[pln][index] = value;
}
