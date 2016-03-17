# dakka\_ignorehubs

**Related CVars:** none

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/dakka_enter.h`

**CVar type:** bool

**Default value:** false

## Description

`dakka_ignorehubs` is a [CVar](../cvars.md) that determines whether Dakka should reset things between hub maps or not.

If it's on, it'll treat each hub level like its own separate level, even when returning, and reset things appropriately.

If it's off, it'll only reset when going to a different hub, not when moving around between levels in the same hub.
