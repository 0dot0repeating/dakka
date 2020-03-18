// This is a library dedicated to making pickups give different items to
//  different classes. Theoretically, you could use it for just one class,
//  but why the fuck would you?


// What each file corresponds to:
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
//      == pickup_server_defs ==
//
// What pickup_client_defs is to server-to-client communication,
//  pickup_server_defs is to client-to-server. If you just want to add slots to
//  this, you should only need to go into here.
//
//
//      == pickup_server ==
//
// The client needs to keep the server in the loop on some things as well. That
//  shit's handled here.
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

#include "pickup_client_defs.h"
#include "pickup_client.h"
#include "pickup_server_defs.h"
#include "pickup_server.h"
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
#include "pickup_clientmessage.h"
#include "pickup_display.h"
#include "pickup_daemon.h"

#include "dakka_wepdisplay.h"
#include "dakka_moremessages.h"
#include "dakka_vanillaarmor.h"

// This is such a tiny script, it's a complete waste making a file for it. So
//  here it is.
script "Pickup_Dropped" (void)
{
    SetResultValue(GetActorProperty(0, APROP_Dropped));
}
