# dakka\_startmode\_weapons

**Related CVars:** [dakka_startmode_ammo.md](dakka_startmode_ammo)

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/pickup/pickup_items_weapons.h`

**CVar type:** int

**Default value:** 0

## Description

`dakka_startmode_weapons` is a [../cvars.md](CVar) that determines what types of
weapons you get when you enter a map, or respawn in a cooperative game mode if
server settings cause you to lose your weapons on death. It has two modes.

1. If the CVar is set to a value less than or equal to 0, the weapons you
    (re)spawn with are untouched.

2. Otherwise, the weapons you spawn with are recalculated. The value of the CVar
    is used to determine the maximum power ranking of any weapon you spawn with.
    Any weapon above that power ranking is stripped from you, along with its ammo.


The power rankings are located in `/pk3/acs/pickup/pickup_items_weapons.h`, in 
`Start_WeaponPowers[]` The weapons they correspond to are in `Start_Weapons[]`, if
the comments are unclear.
