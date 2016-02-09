# dakka\_backpackstart

**Related CVars:** none

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/dakka_const.h`

**CVar type:** int

**Default value:** 0

## Description

`dakka_backpackstart` is a [CVar](../cvars.md) that controls whether you spawn
with a backpack or not. If it's set to 0, it does nothing. If it's set to a
positive value, it gives you a backpack. If it's set to negative, it takes your
backpack away.

Note that being given a backpack doesn't give you any extra ammo. On the other hand,
having a backpack taken away from you *will* remove any over-capacity ammo.

This CVar is used before `dakka_startmode_ammo` is, so if you have a backpack start
and a start mode that gives max ammo, you will get the full amount of ammo, not just
half of it.

The array that stores the backpack items, `Dakka_BackpackItems[]`, is in `/pk3/acs/dakka_const.h`.
