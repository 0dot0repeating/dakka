// OH MY GAWD SCRIPTING CRAP I CAN'T FUCKIN EVEN


// Now, not every pickup's gonna be scripted - most of them won't be -
//  so instead of having something like this for every single item for every
//  single class, we'll instead define whether a pickup should be scripted on
//  a case by case basis.

#define PK_SCRIPTEDCOUNT    4

// Row 0 indicates which item index this is for.
#define PK_S_ITEMNUM        0

// Row 1 indicates which class this is for. 0 is Cl_Unknown.
#define PK_S_CLASSNUM       1

// Row 2 is the script number.
#define PK_S_SCRIPTNUM      2

// Rows 3 and 4 are the three arguments you can supply. Data about the
//  pickup's item number, class number, etc are stored in PKP_PickupData in
//  pickup_pickup_defs.h.
#define PK_S_ARG1           3
#define PK_S_ARG2           4
#define PK_S_ARG3           5

// Do we just want the script to determine which pickup index we're going to
//  use? If so, this is true.

#define PK_S_RETURNINDEX    6

// Is this a named script? If so, read the script name from PKP_PickupNamed.

#define PK_S_NAMEDSCRIPT    7


// And the data itself.
// 
// If you don't actually want any scripted pickups, just define the scripted
//  whatever count as 1, and use {-1, -1, -1, -1, -1, -1}.
//  
// It won't match anything.

int PKP_ScriptedPickups[PK_SCRIPTEDCOUNT][8] =
{
    {It_PlasmaRifle, Cl_Dakkaguy, 494, 0, 1, 0, true, true}, // pickup/dakka_wepdisplay.h
    {It_BFG9000,     Cl_Dakkaguy, 494, 1, 1, 0, true, true},
    {It_Shotgun,     Cl_Dakkaguy, 494, 2, 1, 0, true, true},
    {It_Chaingun,    Cl_Dakkaguy, 494, 3, 1, 0, true, true},
};

int PKP_PickupNamed[PK_SCRIPTEDCOUNT] =
{
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
};




// Let's allow scripted messages as well.
//
// This doesn't handle the server sending message info to the client; that's
//  handled by Pickup_SendMessage in pickup_pickup.h. If you need to send more
//  info over, that's the place to look.
//
// I would *not* recommend using the stuff in pickup_client.h for pickup
//  messages, despite it seeming like an obvious fit. The reason is that there
//  isn't a guarantee that the info will get to the client in time for use in
//  a pickup message. Plus, if the info is unique per pickup, you're guaranteed
//  to get faulty behaviour if you pick up multiple things in the same tic.
//
// Look at the MDATA_* stuff in pickup_pickup.h instead. That's what you'd want
//  to modify.
//
// I copypasted PKP_ScriptedPickups over for this. It's the exact same layout.

#define MSG_SCRIPTEDCOUNT   16

// Row 0 indicates which item index this is for.
#define MSG_S_ITEMNUM       0

// Row 1 indicates which class this is for. 0 is Cl_Unknown.
#define MSG_S_CLASSNUM      1

// Row 2 is the script number.
#define MSG_S_SCRIPTNUM     2

// Rows 3 and 4 are the three arguments you can supply. Data about the
//  pickup's item number, class number, etc are stored in CMSG_MessageData
//  in pickup_pickup_defs.h.
#define MSG_S_ARG1          3
#define MSG_S_ARG2          4
#define MSG_S_ARG3          5

// Are we handling the entire pickup through the script, or just getting a
//  string to display through it?
#define MSG_S_ONLYSTRING    6

// Is this a named script? If so, read the script name from PKP_MessageNamed.

#define MSG_S_NAMEDSCRIPT    7


// And the data itself.
int CMSG_ScriptedMessages[MSG_SCRIPTEDCOUNT][8] =
{
    {It_Fist,               Cl_Dakkaguy,  -1, 0,   0,0, true, true},
    {It_Chainsaw,           Cl_Dakkaguy,  -1, 1,   0,0, true, true},
    {It_Pistol,             Cl_Dakkaguy,  -1, 2,   0,0, true, true},
    {It_Shotgun,            Cl_Dakkaguy,  -1, 3,   0,0, true, true},
    {It_SuperShotgun,       Cl_Dakkaguy,  -1, 4,   0,0, true, true},
    {It_Chaingun,           Cl_Dakkaguy,  -1, 5,   0,0, true, true},
    {It_Berserk,            Cl_Dakkaguy,  -1, 6,   0,0, true, true},
    {It_ST_Minigun,         Cl_Dakkaguy,  -1, 6,   0,0, true, true},
    {It_RocketLauncher,     Cl_Dakkaguy,  -1, 7,   0,0, true, true},
    {It_ST_GrenadeLauncher, Cl_Dakkaguy,  -1, 7,   0,0, true, true},
    {It_PlasmaRifle,        Cl_Dakkaguy,  -1, 8,   0,0, true, true},
    {It_Dakka_Channeler,    Cl_Dakkaguy,  -1, 9,   0,0, true, true},
    {It_ST_Railgun,         Cl_Dakkaguy,  -1, 9,   0,0, true, true},
    {It_BFG9000,            Cl_Dakkaguy,  -1, 10,  0,0, true, true},
    {It_Dakka_Impaler,      Cl_Dakkaguy,  -1, 11,  0,0, true, true},
    {It_ST_BFG10k,          Cl_Dakkaguy,  -1, 11,  0,0, true, true},
};

int CMSG_MessageNamed[MSG_SCRIPTEDCOUNT] =
{
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
    "Dakka_MoreMessages",
};




// OH HEY LOOK AT THAT IT'S SCRIPTED DISPLAY SHIT.
//
// I copypasted PKP_ScriptedPickups again. You still only get two arguments.
//  First one is still 'dropped'.

// Important things to note when scripting pickup displays:
//
// - Do not rely on ANYTHING in DECORATE that doesn't change purely decorative
//   things to work. In fact, don't even trust the functions that DO change
//   purely decorative things. A_SetTranslucent works on the client's end.
//
// - Typically, anything you run in ACS will go through, even on the client's
//   end. But it is ***ONLY*** running on the client, so if you move an item
//   around for example, know that it is only moving around for the client,
//   and you're creating a weird display for everyone else as he helplessly
//   chases a nonexistent item around.


#define DISP_SCRIPTEDCOUNT  6

// Row 0 indicates which item index this is for.
#define DISP_S_ITEMNUM      0

// Row 1 indicates which class this is for. 0 is Cl_Unknown.
#define DISP_S_CLASSNUM     1

// Row 2 is the script number.
#define DISP_S_SCRIPTNUM    2

// Rows 3 and 4 are the three arguments you can supply. Data about the pickup
//  (old class number, current class number, item number, drop status, etc)
//  are stored in Disp_ScriptArgs, in pickup_display.h.
#define DISP_S_ARG1         3
#define DISP_S_ARG2         4
#define DISP_S_ARG3         5

// Named script? Yaddayadda.

#define DISP_S_NAMEDSCRIPT  6

int DISP_ScriptedDisplays[DISP_SCRIPTEDCOUNT][7] =
{
    {It_Blursphere,  Cl_Unknown,  493, 0, 0, 0, true}, // pickup/dakka_vispulse.h
    {It_Blursphere,  Cl_Dakkaguy, 493, 0, 0, 0, true},
    {It_PlasmaRifle, Cl_Dakkaguy, 494, 0, 0, 0, true}, // pickup/dakka_wepdisplay.h
    {It_BFG9000,     Cl_Dakkaguy, 494, 1, 0, 0, true},
    {It_Shotgun,     Cl_Dakkaguy, 494, 2, 0, 0, true},
    {It_Chaingun,    Cl_Dakkaguy, 494, 3, 0, 0, true},
};

int DISP_DisplayNamed[DISP_SCRIPTEDCOUNT] = 
{
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
    "Dakka_WepDisplay",
};
