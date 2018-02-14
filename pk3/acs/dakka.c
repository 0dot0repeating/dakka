#library "dakka"
#include "zcommon.acs"

#include "dakka_commonFuncs.h"




// Constant definitions that don't fit anywhere else.
#include "dakka_defs.h"

// Messages for dakka_cl_moremessages.
#include "dakka_defs_moremessages.h"




// Pickup system.
#include "pickup/pickup.h"

// Score system.
#include "score/score.h"


// Class weapons. I want to keep this separate from the pickup system itself,
//  since the pickup system doesn't actually need to know about this.
#include "dakka_defs_classweapons.h"


////////
// vv STUFF FOR DAKKA_SPAWN

// Temporary items. These are items that are given and taken every tic by the
//  spawn script (dakka_enter.h) and used to signal shit to DECORATE. There may,
//  however, be a one tic delay between when a condition related to a temporary
//  item changes and when you actually get the item, due to the nature of
//  scripts running in no particular order. So don't use this for stuff that
//  needs to be communicated instantly.
#include "dakka_tempitems.h"

// Used in dakka_enter.h. Stuff that runs once, when the player spawns for the
//  first time. Not sure how I'll handle competitive gamemodes just yet, but
//  then again, who the hell would deathmatch or CTF with Dakka?
#include "dakka_levelstart.h"

// Handle IDFA and IDKFA here.
#include "dakka_idfa.h"

// Scrap processing. For the scrapper.
#include "dakka_scrap.h"

// Handle the CVars dakka_shotgun2ssg and dakka_chaingun2minigun.
#include "dakka_gunstoguns.h"

// Make sure we don't have guns belonging to other classes.
#include "dakka_translateguns.h"

// <insert dead meme here>
#include "dakka_powerlevel.h"

// Vampirism and ammo regen need sounds that don't take up sound slots. This is
//  how it's done.
#include "dakka_soundloopers.h"

// ^^ STUFF FOR DAKKA_SPAWN
////////


// Tracers, for all sorts of things.
#include "dakka_tracers.h"

// Lightning tracer.
#include "dakka_lightning.h"

// Rotate sound slots so weapon firing sounds can overlap with each other.
#include "dakka_rotatesound.h"

// Kill things, get health.
#include "dakka_vampirism.h"


// Functions used in multiple projectiles.
#include "dakka_projchecks.h"

// Punch the things.
#include "dakka_fists.h"

// Mainly for revolver accuracy.
#include "Dakka_Pistols.h"

// This is how the sticky grenades for the shotgun and SSG stick.
#include "dakka_stickynade.h"

// Auto-reload the SSG when it's in your pants. Also scripts called by the SSG,
//  to handle the triggers and whatnot
#include "dakka_ssg.h"

// Handles both the minigun's spindown and the flamethrower.
#include "dakka_minigun.h"

// This determines how the impaler alt-fire actually impales.
#include "dakka_impaler.h"

// Plasma lance hit detection. Also used in the impaler.
#include "dakka_plasmalance.h"

// BFG BEAM SON
#include "dakka_bfg.h"


// Spread script, so that projectiles spread in a more circular manner, rather than a box.
#include "dakka_spread.h"


// Clientside recoil script. Does anyone actually use this?
#include "dakka_recoil.h"

// Clientside script to check dakka_cl_lesseffects.
#include "dakka_lesseffects.h"

// Clientside weapon switching script, because A_SelectWeapon don't do shit online.
#include "dakka_wepswitch.h"

// Zandronum netcode is fun, isn't it?
#include "dakka_online.h"


// Each of these is for their respecting ACS script type.
#include "dakka_open.h"

// This also has the RESPAWN script.
#include "dakka_enter.h"

// Monsters call this script when they die.
#include "dakka_onkill.h"
