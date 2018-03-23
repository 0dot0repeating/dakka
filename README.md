![DAKKA](/README_logo.png "necessary")

**For Zandronum 3.0+, GZDoom 2.2.0+, and ZDoom 2.8.1 (with an addon)**

```text
IT IS 2162. THE LAST HUMAN OUTPOST HAS FALLEN TO THE MINIONS OF HELL. EVERYTHING
IS LOST, AND THE REMNANTS OF HUMANITY FLEE TO THE DEEPEST REACHES OF SPACE, TO
HIDE. BUT EVEN NOW, THE FIGHT IS NOT OVER.

YOU WERE A MARINE ON THE "USS LIBERTY". AS YOUR SHIP SPED AWAY FROM THE BROKEN
REMAINS OF EARTH, A GREAT HATRED WAS BUILT IN YOUR HEART, STRONGER THAN ANY
HATRED BEFORE. WITH THE HELP OF SCIENTISTS ON BOARD, YOU TRAINED YOUR BODY AND
MIND TO BECOME THE MOST POWERFUL SOLDIER HUMANITY HAS EVER HAD.

YEARS LATER, YOUR CREW RETURNS TO EARTH AND LOOKS UPON THE TWISTED REMAINS OF
HUMANITY. YOU DON'T KNOW WHAT'S IN STORE FOR YOU ON THE SURFACE, BUT THAT WON'T
STOP YOU. YOU TELEPORT DOWN AND DRAW YOUR PISTOLS.

TIME FOR SOME DAKKA.
```

It's a mod with guns. The guns make everything easy - not Russian Overkill levels
of easy, but still pretty damn easy. Eventually, I'll change up the monsters to
fix this. Today's not that day.

Any version at and before 0.06-alpha6-2 were made in the days of Zandronum 1.3.
They might use an old, deprecated method to set console variables, so be wary.
Zandronum 3 should run them perfectly well.

Early OpenAL implementations in GZDoom will play stereo sounds incorrectly - if
the pistols and super shotgun sound quiet and muffled compared to the impaler,
that's the bug. Update to the latest GZDoom if this is happening.

**DOWNLOAD:** http://jinotra.in/downloads/mods/doom/dakka/



## HOW TO GET A GIT BUILD

**[Visual instructions](http://jinotra.in/static/downloads/dakkaVids/how2dakkaGit.webm)**

Above the latest-commit bar, on the very right, there's a download icon.

Click that, choose ".ZIP", then unzip the ZIP file into its own directory.

Go into the pk3/ directory, select everything in there, and zip it all up.
Change the extension of the new ZIP file to ".pk3", and you should be good to go.

If, in the ZIP you created, everything's still in the "pk3/" folder, you zipped
the folder, not its contents. Don't do that.



## WHAT'S CHANGED SINCE 0.06?

The `CHANGELOG.txt` file will tell you in greater detail what's changed, but the
most important things:

- Dakkaguy's limp-wrist punching days are over. Instead, you can pummel your foes
    with an utterly ridiculous barrage of jabs, or cave their face in with a hook
    to the face that'd leave anyone reeling in pain, if not splattered all over the
    walls. Or give them a one-two-**THREE** to bust the skulls of both the poor sap
    in front of your fist, and all his buddies next to him.
    
- The pistols have gotten a complete overhaul! No longer the boring, odd looking
    Berettas you've been looking at since 1994, but instead sleek, sexy chrome-
    black hand cannons that do a far better job convincing you to use them! You
    gotta reload them every now and then now, but the shots hit harder, the guns
    sound cooler, you can mash the trigger to fire faster, and overall it's just
    much more fun to use. Who knows, you might actually run out of pistol rounds
    for a change!

- The minigun's flamethrower now properly sets things on fire! Focus it on a
    demon to cleanse his sinful presence, or sweep a crowd and wipe out all those
    pesky hitscanners within it!

- The rocket launcher has been dropped and replaced with the **IMPALER.**
    Originally in the BFG slot before I decided that was stupid, it's a triple
    barreled, fully automatic deliverer of explosive death. The primary fire is
    a 300RPM torrent of missiles, while the alt-fire... well, it impales enemies
    and carries them away. Then they explode. Because why shouldn't you be able
    to weaponize the demons themselves?

- The plasma rifle and channeler have been dropped in favor of the **LANCER.**
    Taking the best of both guns and combining it into one, you have a double-barreled
    demon busting machine that anyone can appreciate. Do you want to pepper your
    enemies with plasma nails, or do you want to charge up a lightning storm to
    roast everyone in front of you? Either way, the lancer has you covered.

- Why did the BFG of a mod called *DAKKA* not exemplify the idea of dakka? To be
    honest, I couldn't tell you, so I fixed that. The primary fire of the BFG is
    now an endless deluge of green plasma death, while the alt fire spits out an
    entire wall of plasma orbs, which then seek and destroy your current target.
    Either way, your enemies will die in a maelstrom of plasma, and that's the
    way it should've been from the start.

- The berserk pickup, now vacant due to the minigun changing spots, is now occupied
    by the **VAMPIRE SPHERE.** Every kill you make grants you health, with more
    health being granted for tougher enemies and being low on health yourself.
    Good encouragement to stay in the fight and keep killing demons, don't you think?

- Dakka no longer replaces enemies! DEHACKED patches no longer break, and if you're
    using GZDoom, you no longer need monster mod compatibility patches. Zandronum
    still needs them for the score system to work optimally, due to its lack of
    `RGF_NOSIGHT` parameter for `A_RadiusGive`. But monsters will always give you
    points when they die, in either port!

- [The HUD isn't ugly anymore!](https://imgur.com/a/LaoAI) Seriously, what was I
    *thinking* with those rounded corners and that awful alignment?



## COMPATIBILITY PATCHES

- **ZDoom support (`dakka_zdoom.wad`):** ZDoom lacks an event handler for monsters
    dying, so you need this patch to hook the Doom monster roster into the score
    system. This probably won't play well with DEHACKED monsters either. But it works.

- **`dakka_keyconfclass.wad`:** If you load DAKKA and don't have the fists and pistols
    when you start a game, that usually means the maps you're playing have their own
    player classes, and are overwriting DAKKA. This gets around that.

- **`dakka_keyconfslots.wad`:** Similarly, if you load DAKKA and can't switch weapons,
    the maps you're using are almost definitely using an old, broken method of changing
    weapon slots. This gets around that.

- **Ancient Aliens (`dakka_aaliens.wad`):** This adds the gibbed marine and dead
    lost soul to the score system since Ancient Aliens replaces them with custom
    enemies, and adjusts their score values upwards to match their increased danger.
    
    - **ZDoom support (`dakka_aaliens_zdoom.wad`):** If you're using ZDoom, you
        need this so that the custom aliens give you points when they die. Load
        this after `dakka_zdoom.wad` instead of `dakka_aaliens.wad`.

- **Scythe 2 (`dakka_scythe2.wad`):** This boosts the score values of the Nazi
    and Commander Keen actors dramatically, as they're replaced with evil marines
    and afrits respectively, and are much more dangerous than their replacements.
    
    - **ZDoom support (`dakka_scythe2_zdoom.wad`):** If you're using ZDoom, you
        need this for the custom monsters to work properly. Load this after
        `dakka_zdoom.wad` instead of `dakka_scythe2.wad`.


To use any of these addons, download them from the `addons/` directory (use the
"Raw" button), and load the mods in this order:

- The other maps/monsters in question
- **DAKKA**
- The compatibility patch(es)



## Are you *ever* going to release this?

I'm trying man :(