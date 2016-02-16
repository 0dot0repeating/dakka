# dakka\_shotgun2ssg

**Related CVars:** `dakka_chaingun2minigun`

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/dakka_gunstoguns.h`, `/pk3/acs/pickup/dakka_wepdisplay.h`

**CVar type:** int

**Default value:** 0

## Description

`dakka_shotgun2ssg` is a [CVar](../cvars.md) that controls how shotguns spawned 
on the map are replaced with SSGs. Any value less than 1 disables the behaviour 
\- shotguns stay shotguns, and SSGs stay SSGs. Any value at or above 1 indicates 
how many maps you must have a shotgun in your inventory in order for map-placed 
shotguns to become SSGs. For example, if `dakka_shotgun2ssg` is 1, you must 
have a shotgun in your inventory for one map to have map shotguns replaced with 
SSGs.

This CVar counts how many levels you've had a shotgun by checking at the start 
of every map (or every hub, if playing in hub-based maps) if you have a 
shotgun. If you do have a shotgun, one instance of an inventory counter 
(`DakkaShotgunTracker`) is placed in your inventory. If you don't, they're all 
taken away.

This CVar exists as a way to get the SSG in Doom 1. For Doom, I would 
personally recommend setting the CVar to 0 for Episode 1, 4 or 5 for Episode 2, 
around 2 or 3 for Episode 3, and as low as 1 for Episode 4.

Or just don't use it at all and play Doom 1 without an SSG. The minigun's 
certainly good enough, and that replaces berserk packs.
