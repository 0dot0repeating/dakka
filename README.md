# DAKKA

It's a mod with guns.

**DOWNLOAD:** http://jinotra.in/downloads/mods/doom/dakka/


## Wait, why's it missing so much stuff?

Dakka's code got messy, and underlying systems changed enough that I decided
enough was enough. Rather than let it go the way of Samsara, I decided to nip
it in the bud and restructure everything, cleaning up extraneous code,
streamlining things, and so on and so forth.

## So what *is* in so far?

* HUD
* Arc system
* Score system
* All of the guns
* Recoil system, if anyone actually used that
* Pickup system, though with some pickups left to define
* Tracer system, cleaned up and much better at respecting your pitch
* dakka_cl_lesseffects


## What's left?

* dakka_startmode (big one)
* dakka_cl_moremessages (I miss it already)
* dakka_scrapperstart and dakka_backpackstart
* dakka_jumpmod (might not come back in, honestly)
* Pickup definitions for health, armor, and powerups (minigun and channeler can't be found right now)
* The dakka_rearm CVars
* Score rewards


## How long's it gonna take to add all the shit back in?

Hopefully not very long. All of this shit was coded before, so it's not like I
have to redo fucking everything. I just need to untangle it as best I can, and
where I can't, leave comments indicating where to look instead of just shit like
"// and this one" with no other comments within like 50 lines. That was so dumb.


*README to be expanded as I think of what to add in this damn thing.*
