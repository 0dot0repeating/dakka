# DAKKA

It's a mod with guns.

***IMPORTANT:*** This mod only works in Zandronum versions at least 3.0 and
ZDoom versions at least 2.7.1. Multiplayer play is only support on Zandronum
versions at least 3.0 - it could conceivably work on ZDoom, but the pickup
display for sure would be utterly broken for every player besides the host.

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
* dakka\_cl\_lesseffects
* dakka\_startmode (redone as dakka\_startmode\_weapons, dakka\_startmode\_ammo, and dakka\_startmode\_health)
* dakka\_scrapperstart and dakka\_backpackstart


## What's left?

* dakka\_cl\_moremessages (I miss it already)
* dakka\_jumpmod (might not come back in, honestly)
* The dakka\_rearm CVars
* Score rewards


## Anything new?

*   The Channeler, a new slot 6 weapon with weapon behaviour that puts the original
    BFG to shame in complexity. In short, it fires chain lightning. Primary fire
    launches it straight from the channeler itself, and the alt-fire will fire
    a ball of cyan energy stuff that will arc lightning through anyone nearby,
    both as it flies through the air and for a couple seconds after it hits something.

    You can get the Channeler through entering a map with the plasma rifle in
    your inventory and finding a plasma rifle pickup - it'll have been replaced
    with the channeler, just for you (other people will see the plasma rifle if
    they didn't enter the map with one). Alternately, if dakka\_bundleplasma is
    on, plasma rifle pickups will be replaced with a big box containing both guns.

*   The Impaler, a new slot 7 weapon. It currently has no alt-fire, but th
    primary fire rapidly fires rockets that deal up to 304 damage and pierce
    straight through walls. Damage is obviously diminished, but it still hurts
    enough to kill with ease through most walls. Don't try to kill the boss brain
    through the wall, though. I already tried that. Wall's too thick.

    It currently can't be found in the map. Use idfa or something to get it.


## How long's it gonna take to add all the shit back in?

Hopefully not very long. All of this shit was coded before, so it's not like I
have to redo fucking everything. I just need to untangle it as best I can, and
where I can't, leave comments indicating where to look instead of just shit like
"// and this one" with no other comments within like 50 lines. That was so dumb.


*README to be expanded as I think of what to add in this damn thing.*
