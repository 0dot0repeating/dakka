# dakka\_bug_nossginvcheck

**Related CVars:** none

**Related files:** `/pk3/acs/dakka_ssg.h`

**CVar type:** bool

**Default value:** false

## Description

`dakka_bug_nossginvcheck` is a [CVar](../cvars.md) that re-enables a funny bug
in the SSG. With the CVar off, it'll check how many shots you have left in the
SSG, and refuse to let you pull the trigger again if you're out of shots. With
the CVar on, it disables that check, and always lets you pull the trigger, even
if you're out of shots to fire and need to reload.

You only get a four tic window to pull the trigger again, but if you can, you'll
be able to fire your SSG more than twice per reload. If you use an alias, your
SSG will fire faster than the chaingun. You're welcome.

The SSG will even try to shoot without ammo! It won't succeed, but it'll still
do the recoil, firing animation, and sound. It just won't actually put bullets
out.

[An accurate representation of what happens with this CVar on.](https://my.mixtape.moe/uvtqvp.webm)
