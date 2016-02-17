# dakka\_bundleplasma

**Related CVars:** `dakka_bundlebfg`

**Related files:** `/pk3/acs/pickup/dakka_wepdisplay.h`, `/pk3/acs/dakka_open.h`

**CVar type:** bool

**Default value:** false

## Description

`dakka_bundleplasma` is a [CVar](../cvars.md) that controls how the channeler is
found in the map. If the CVar is false, you will only get the channeler if you
enter the map with a plasma rifle (a plasma rifle given by `dakka_startmode_weapons`
counts for this) and find a plasma rifle pickup - instead of a plasma rifle, it will
be the channaler.

If this CVar is true, you still instead find a big 'ol crate where plasma rifle
pickups would be. It will contain both the plasma rifle and channeler, along with
a net total of 80 cells.

If you rapidly flip this CVar around, plasma rifle pickups look funny while you do it.

(this is top tier documentation)
