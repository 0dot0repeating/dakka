# dakka\_chaingun2minigun

**Related CVars:** `dakka_shotgun2ssg`

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/dakka_gunstoguns.h`, `/pk3/acs/pickup/dakka_wepdisplay.h`

**CVar type:** int

**Default value:** 0

## Description

`dakka_chaingun2minigun` is a [CVar](../cvars.md) that controls how chainguns 
spawned on the map are replaced with miniguns. Any value less than 1 disables 
the behaviour - chainguns stay chainguns, and miniguns stay miniguns. Any value 
at or above 1 indicates how many maps you must have a chaingun in your 
inventory in order for map-placed chainguns to become miniguns. For example, if 
`dakka_chaingun2minigun` is 1, you must have a chaingun in your inventory for 
one map to have map chainguns replaced with miniguns.

This CVar counts how many levels you've had a chaingun by checking at the start 
of every map (or every hub, if playing in hub-based maps) if you have a 
chaingun. If you do have a chaingun, one instance of an inventory counter 
(`DakkaChaingunTracker`) is placed in your inventory. If you don't, they're all 
taken away.

This CVar mainly as a counterpart to `dakka_shotgun2ssg`, and I guess as a way 
to guarantee the ability to get the minigun in Episode 1.

(copypasted all of this from the `dakka_shotgun2ssg` page)
