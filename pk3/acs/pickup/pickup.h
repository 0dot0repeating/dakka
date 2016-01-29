// This is a library dedicated to making pickups give different items to
//  different classes. Theoretically, you could use it for just one class,
//  but why the fuck would you?


// What each file corresponds to:
//
//
//      == pickup_const ==
//
// This is where constant definitions that don't fit anywhere else lay. Mainly
//  script numbers.
//
//
//      == pickup_classes ==
//
// This stores the data and functions required to distinguish classes from each
//  other. Notably, class-related constants are here.
//
//
//      == pickup_items ==
//
// This is where pickups are actually defined!
//
// It's also entirely constant definitions, like pickup_const.
//
//
//      == pickup_items_pickup ==
// 
// This is such a dumb, redundant name. But whatever. This stores purely
//  cosmetic data used on pickup for each item. Pickup messages, flashes,
//  pickup sounds, the like.
//
//
//      == pickup_items_weapons ==
//      == pickup_items_health ==
//      == pickup_items_armor ==
//      == pickup_items_ammo ==
//
// Since these are special pickup types (not just plain Inventory items), they
//  need their own metadata kept for them.
//  
//
//      == pickup_scripted ==
//
// Every constant/array definition for scripting pickups goes in here. Pickup,
//  display, messages, doesn't matter, it's in here.
//
//
//      == pickup_pickup_defs ==
//
// Constant and array definitions for the pickup code.
//
//
//      == pickup_pickup_weapons ==
//      == pickup_pickup_health ==
//      == pickup_pickup_armor ==
//      == pickup_pickup_ammo ==
//
// Specialized pickup functions. Only pickup code for the inventory type should
//  be in these files.
//
//
//      == pickup_pickup ==
//
// It's redundant! But it's also where the pickup functions are stored. Unlike
//  pickup_classes, there's too much stuff in pickup_items to put them in there
//  without everything becoming too damn crowded.
//
//
//      == pickup_client_defs ==
//
// The actual server-to-client slots are defined here. You don't need to go
//  into pickup_client to do anything if you just want to add slots to this.
//
//
//      == pickup_client ==
//
// This is where various functions and scripts for sending data from the server
//  to the client are found. The system-critical S2C definitions are here too.
//
//
//      == pickup_clientmessage ==
//
// When a pickup succeeds, the server sends the client some script calls, giving
//  the client enough data about the pickup to run a pickup message script. This
//  is where those scripts are.
//
//      
//      == pickup_display ==
//
// This is what the stuff in pickup_client is mainly for. It controls display
//  of pickups, which can be deferred to other scripts.
//
//
//      == pickup_daemon ==
// 
// This is where the OPEN script that sends stuff from the server to the client
//  is at. It also contains the DISCONNECT script resetting the data when the
//  client leaves/spectates, and the ENTER script force-sending the presumably
//  empty data to the client (as a reset button, in case they spec/rejoined).
//
//
//          ====================
//          ===== FUN FACT =====
//          ====================
// 
// If you take only pickup_const.h, pickup_client.h, and pickup_daemon.h, you'll
//  have yourself a generic system for sending data from the server to the client
//  via and for ACS. Handy!
//
// Well, so long as you get rid of the script stuff.

#include "pickup_const.h"
#include "pickup_classes.h"
#include "pickup_items.h"
#include "pickup_items_pickup.h"
#include "pickup_items_weapons.h"
#include "pickup_items_health.h"
#include "pickup_items_armor.h"
#include "pickup_items_ammo.h"
#include "pickup_scripted.h"
#include "pickup_pickup_defs.h"
#include "pickup_pickup_weapons.h"
#include "pickup_pickup_health.h"
#include "pickup_pickup_armor.h"
#include "pickup_pickup_ammo.h"
#include "pickup_pickup.h"
#include "pickup_client_defs.h"
#include "pickup_client.h"
#include "pickup_clientmessage.h"
#include "pickup_display.h"
#include "pickup_daemon.h"


// This is such a tiny script, it's a complete waste making a file for it. So
//  here it is.
script PICKUP_DROPPED (void)
{
    SetResultValue(GetActorProperty(0, APROP_Dropped));
}


// Class number checking script. If this returns true, then the class number
//  this script is for is considered the class for this player. Otherwise,
//  it keeps checking class numbers.

script 490 (int tid, int gunType)
{
    if (tid != 0) { SetActivator(tid); }

    int ret = false;

    switch (gunType)
    {
      case 0: ret = CheckWeapon("Pistol")   || CheckWeapon("Chaingun");     break;
      case 1: ret = CheckWeapon("Shotgun")  || CheckWeapon("SuperShotgun"); break;
      case 2: ret = CheckWeapon("RocketLauncher");                          break;
      case 3: ret = CheckWeapon("BFG9000")  || CheckWeapon("PlasmaRifle");  break;
    }

    SetResultValue(ret);
}


// Pickup testing script.
// 
// If, in PKP_ScriptedPickups, you've specified this to be a pickup script that
//  returns a new index, you need to do that with SetResultValue, before any
//  delays if you do any for some reason. Otherwise, you'll want to set values
//  in PKP_ReturnArray, defined in pickup_pickup_defs.h.
//
// Pickup data's in PKP_PickupData, defined in pickup_pickup_defs.h.
//
//
// Relevant definitions:
//
// #define PDATA_ITEMNUM   0
// #define PDATA_CLASSNUM  1
// #define PDATA_DROPPED   2
//
// #define PARRAY_SUCCEEDED        0
// #define PARRAY_CONSUME          1
// #define PARRAY_NOCONSUME        2
// #define PARRAY_DIDSOMETHING     3
// #define PARRAY_LOWHEALTH        4
//

script 491 (int arg1, int arg2, int arg3)
{
    Log(s:"[PKP] args: (", d:arg1, s:", ", d:arg2, s:", ", d:arg3, s:")");
    Log(s:"[PKP] item: ", d:PKP_PickupData[PDATA_ITEMNUM],
           s:", class: ", d:PKP_PickupData[PDATA_CLASSNUM],
         s:", dropped: ", d:PKP_PickupData[PDATA_DROPPED]);

    SetResultValue(It_Shell);
}


// Pickup message testing script.
//
// If, in CMSG_ScriptedMessages, you've specified this to be a message script
//  that returns a pickup message, you need to do that with SetResultValue,
//  before any delays if there are any. Otherwise, do whatever the hell you
//  want, because this message script ends with this script.
//
// Message data's in CMSG_MessageData, defined in pickup_clientdata.h. The
//  constant definitions for it are in pickup_pickup_defs.h, however, because
//  they're the same as the constants for PKP_MessageData, which is also in
//  there.
//
// 
// Relevant definitions:
//
// #define MDATA_CLASSNUM     0
// #define MDATA_ITEMNUM      1
// #define MDATA_DROPPED      2
// #define MDATA_LOWHEALTH    3
//

script 492 (int arg1, int arg2, int arg3)
{
    int pln = PlayerNumber();

    Log(s:"[MSG] args: (", d:arg1, s:", ", d:arg2, s:", ", d:arg3, s:")");
    Log(s:"[MSG] dropped: ", d:CMSG_MessageData[pln][MDATA_DROPPED]);

    SetResultValue("my bum");
}

// Pickup display testing script.
//
// Note that you have to handle DPASS_DOCLEANUP somehow. Here, it doesn nothing
//  since APROP_RenderStyle and APROP_Alpha are already reset by the main
//  display script when class number changes, but if you're doing any other
//  changes to the pickup, make sure to undo them here.
//
// Pickup data's in DISP_ScriptArgs, defined in pickup_display.h.
//
//
// Relevant definitions:
//
// #define DPASS_ITEMNUM       0
// #define DPASS_CLASSNUM      1
// #define DPASS_OLDCLASSNUM   2
// #define DPASS_DROPPED       3
// #define DPASS_DOCLEANUP     4
//

script 493 (int arg1, int arg2, int arg3)
{
    if (DISP_ScriptArgs[DPASS_DOCLEANUP])
    {
        terminate;
    }

    if (DISP_ScriptArgs[DPASS_CLASSNUM] != DISP_ScriptArgs[DPASS_OLDCLASSNUM])
    {
        SetActorState(0, "Scripted");
    }

    int timerPulse = Timer() % 48;

    if (timerPulse > 24) { timerPulse = 48 - timerPulse; }

    int newAlpha = 0.25 + ((1.0 / 24) * timerPulse);

    SetActorProperty(0, APROP_RenderStyle,  STYLE_Translucent);
    SetActorProperty(0, APROP_Alpha,        newAlpha);
}
