# dakka\_bundlebfg

**Related CVars:** `dakka_bundleplasma`

**Related files:** `/pk3/acs/pickup/dakka_wepdisplay.h`, `/pk3/acs/dakka_open.h`

**CVar type:** bool

**Default value:** false

## Description

`dakka_bundlebfg` is a [CVar](../cvars.md) that controls how the impaler is
found in the map. If the CVar is false, you will only get the impaler if you
enter the map with a BFG9000 (a BFG given by `dakka_startmode_weapons`
counts for this) and find a BFG pickup - instead of a BFG, it will
be the impaler.

If this CVar is true, you still instead find a big 'ol crate where BFG
pickups would be. It will contain both the BFG and impaler, with ammo for both.
