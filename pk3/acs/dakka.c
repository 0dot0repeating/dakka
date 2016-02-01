#include "zcommon.acs"
#library "dakka"

#include "commonFuncs.h"




// Constant definitions that don't fit anywhere else.
#include "dakka_const.h"

// Script numbers and nothing else.
#include "dakka_scriptNums.h"

// Monster definitions. Mainly used for calculating the score needed to get
//  a full heal.
#include "dakka_monsters.h"




// Pickup system.
#include "pickup/pickup.h"

// Score system.
#include "score/score.h"

// Arc system, used for the Channeler.
#include "arc/arc.h"



// Temporary items. These are items that are given and taken every tic by the
//  spawn script (dakka_enter.h) and used to signal shit to DECORATE. There may,
//  however, be a one tic delay between when a condition related to a temporary
//  item changes and when you actually get the item, due to the nature of
//  scripts running in no particular order. So don't use this for stuff that
//  needs to be communicated instantly.
#include "dakka_tempitems.h"


// Scrap processing. For the scrapper.
#include "dakka_scrap.h"


// Auto-reload the SSG when it's in your pants.
#include "dakka_ssg.h"


// BFG BEAM SON
#include "dakka_bfg.h"


// Gaussian distribution script, for spread that looks circular rather than a
//  rectangle.
#include "dakka_gaussian.h"



// Tracers, for all sorts of things.
#include "dakka_tracers.h"


// Clientside recoil script. Does anyone actually use this?
#include "dakka_recoil.h"


// Serverside DECORATE-slave script. I can't fucking wait for named scripts.
#include "dakka_decorate.h"

// Clientside same thing.
#include "dakka_decorate_client.h"


// Used in dakka_enter.h. Stuff that runs once, when the player spawns for the
//  first time. Not sure how I'll handle competitive gamemodes just yet.
#include "dakka_levelstart.h"


// Each of these is for their respecting ACS script type.
#include "dakka_open.h"

// This also has the RESPAWN script.
#include "dakka_enter.h"
