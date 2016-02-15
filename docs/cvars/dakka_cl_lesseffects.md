# dakka\_cl_lesseffects

**Related CVars:** none

**Related files:** `/pk3/acs/dakka_tracers.h`, `/pk3/acs/dakka_decorate.h`

**CVar type:** bool

**Default value:** false

## Description

`dakka_cl_lesseffects` is a clientside [CVar](../cvars.md) that, when enabled,
causes less particles to be spawned or displayed for Dakka's more flashy effects.

Things changed:

- Bullet and plasma tracers have their density decrease threefold - the distance between
    tracer puffs increases from 12 units to 36 units.

- Channeler trails have their density decrease fourfold - the distance between tracer
    tracer puffs increases from 8 units to 32 units.

- Bullet puffs no longer spawn bouncing particles, and plasma impact no longer sparks.

- Minigun flames become much less dense.

- Plasma trails don't last as long and involve less particles.

- The BFG beam loses it helix.