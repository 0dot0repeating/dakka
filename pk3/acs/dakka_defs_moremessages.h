
// Message slots:
//  0  - Fists (yes)
//  1  - Scrappers
//  2  - Pistols
//  3  - Shotgun
//  4  - SSG
//  5  - Chaingun
//  6  - Minigun
//  7  - Rocket Launcher
//  8  - Plasma rifle
//  9  - Channeler
//  10 - Plasma lance
//  11 - BFG9000
//  12 - Impaler

#define MMSG_SLOTS  13
#define MMSG_COUNT  11


int MMSG_ExtraMessages[MMSG_SLOTS][MMSG_COUNT] = 
{
    { // Fists
        "And so I rip and tear, for I have seen the demons, and seen that they have huge guts.",
        "Yes, it's better for killing demons than the normal shotgun. Is that bad?",
        "There's nothing wrong with you that I can't fix... with my hands!",
        "More mods should do this whole 'random pickup messages' thing. It's fun!",
        "Get berserked, beat spiderdemons to death.",
        "Here comes the night train!",
        "Guns are for wusses.",
    },
    { // Scrappers
        "This thing would normally require two hands to hold. You dual wield them.",
        "Ow, ow, fuck! These casings are hot! Why am I doing this?!",
        "Oh my god this mod rips off Unreal Tournament so much.",
        "Wait until you see the weaponized pressure washers.",
        "What are you gonna do, give me some spare tin?",
        "Questionably legal. Unquestionably awesome.",
        "These sure are some funky-looking rivets.",
        "Where'd the second one come from?",
        "It turns ammo into more ammo!",
        "It's eco-friendly! Kinda.",
    },
    { // Pistols
        "I still can't tell where the back and top of the slide actually meets on these things.",
        "You could probably beat the map with only these... but do you really want to?",
        "The pistol actually has four barrels. No, don't look down them!",
        "Oh yeah. These things. The pistols. Right. Yeah.",
        "People actually put pistol pickups in a map?",
        "Just be glad it's not an assault rifle.",
        "Well, you gotta have them... I guess.",
    },
    { // Shotgun
        "If you don't have any clay pigeons lying around, lost souls work too.",
        "Even as you pump it and load it, you still feel... unsatisfied.",
        "For the refined, tasteful demon slayer. And everyone else.",
        "Why's the military using hunting shotguns, anyway?",
        "The long end goes towards the demons, by the way.",
        "It's never too early to blow everything up.",
        "Your shells don't also double as grenades?",
        "You got a club that can hold shells!",
        "You can probably find it at S-Mart.",
        "Don't blow your load too quickly.",
        "As opposed to what, a punchgun?",
    },
    { // SSG
        "Because nothing says 'fuck you' like four shells from a double-barreled shotgun.",
        "YES I KNOW THE DOUBLE-TAP MAKES NO SENSE IT'S A COOL GAME MECHANIC OKAY?",
        "Good thing I learned how to reload shotguns with my buttcheeks!",
        "It's a miracle those tiny rockets fit in the barrel at all.",
        "Even the shotguns have tiny teleporters in them!",
        "In town, you're the law. Out here, it's me.",
        "He also uses explosions to make smoothies.",
        "Hello, cacodemon! Goodbye, cacodemon!",
        "Wouldn't be a Doom mod without one.",
        "Proof that demons skeet shoot.",
        "Bang bang. You're dead.",
    },
    { // Chaingun
        "The grenades are launched by very tiny gnomes inside the barrels.",
        "For when you need to shave a demon from halfway across the map.",
        "Not pictured: the fifty pounds of ammo boxes you lug around.",
        "No, it's not technically a chaingun. No, no one cares.",
        "When are they gonna invent a gatling-gatling gun?",
        "So \cathis\c- is where the military budget goes.",
        "Just shove it into your chest. It'll be fine.",
        "Your grenade launcher isn't belt-fed?",
        "\cd$$$$$\c- MAKE IT RAIN \cd$$$$$",
        "What sort of dakka is this?!",
        "omg it shoots balls",
    },
    { // Minigun
        "It's like a fire hose! Except it shoots fire. And there's six of them. And they're made of metal.",
        "Is \cayour\c- flashlight chambered for 5mm rounds? No? Didn't think so.",
        "Absolutely \cano\c- resemblance to any guns from Painkiller. No siree.",
        "I sure hope the map wasn't relying on you having 100 health now.",
        "There's a reason you can carry 1000 rounds for this.",
        "It doesn't even light enemies on fire. How shitty.",
        "It's probably \castill\c- not fast enough!",
        "It's legal! Probably. Who's asking?",
        "How serious? Very serious.",
        "Now go, and terminate.",
        "What's ammo?",
    },
    { // Rocket launcher
        "Giving this the same fire rate as the Doom pistol will surely have no negative repercussions.",
        "Go ahead, stuff two more rockets in the barrel. That won't cause problems.",
        "Because if it wasn't exploded before, it's gonna explode now.",
        "Is that a rocket in your pants, or are you happy to see me?",
        "You are a pain. Luckily, I have the painkiller!",
        "Seriously, how does this thing even reload?",
        "Maybe you should look into being a ranger.",
        "You got the impaler's uglier brother!",
        "Bastard guns for bastard people.",
        "Hey guys, I brought the salsa!",
        "Accept no substitutes.",
    },
    { // Plasma rifle
        "The top of the gun is not a mirror. Doesn't stop you trying to admire yourself on it.",
        "Because the ranch dressing rifle didn't pan out so well in marketing.",
        "Effective range: four feet. What did you expect, it's plasma.",
        "And yet your cell phone still only lasts six hours.",
        "Gotta meet that laser quota somehow.",
        "Lasers have recoil, right guys?",
        "The culmination of power creep.",
        "I'm good at balance, I swear!",
        "But can it play Tetris?",
        "Plasma? Lasers.",
    },
    { // Channeler
        "If hell was invading, then why do they have UAC weapons lying around? \cgCHECKMATE, ATHEISTS.\c-",
        "It was a good day when we taught electricity to only fry demons.",
        "The original design was to just shoot batteries at the demons.",
        "They \castill\c- haven't duck-taped a flashlight to it?",
        "If you think you're dodging this, you're in for a shock.",
        "Can also be used as a defibrilator in a pinch.",
        "Zip, zap, zop! I'm gonna make you pop!",
        "It's positively electrifying!",
        "Aiming is overrated anyway.",
    },
    { // Plasma lance
        "You see, when a marine and a ranger love each other very much...",
        "Hey wait a second, this isn't a lance at all! I was lied to!",
        "Fuck it, just add more barrels to everything.",
        "Yeah just slap lights all over it, it's fine.",
        "The side grip makes it fire 50% faster.",
        "It's the nail for the demons' coffins.",
        "What? Your nailgun is evolving!",
        "Way to get a point across.",
    },
    { // BFG9000
        "Honestly, it's amazing that the UAC even lost with guns like these.",
        "In case you haven't yet realized, this is not a deathmatch mod.",
        "Bringing you the most hated gun from Quake 3 in an alt-fire!",
        "Wasn't needing any of that \"difficulty\" stuff anyway.",
        "The beam is bigger than the gun because shut up.",
        "Everything's better with spinning. Even lasers.",
        "Is this what they meant by going green?",
        "Close range weapon? Eh.",
        // you're welcome
        "B-but Dakka-kun, I don't know if I can take it...",
        "You know you want to thrust your rod upon them.",
        "They can't ignore your girth now.",
    },
    { // Impaler
        "If you ignore fire rate, this is totally weaker than the vanilla rocket launcher.",
        "Reach out and touch someone. With someone else. Then watch them explode.",
        "You don't get to keep it if you keep making dick jokes about it.",
        "What's that? I can't hear you over these endless explosions!",
        "Would you believe this was once a pressure washer?",
        "Oh my god this mod rips off Duke Nukem 3D so much.",
        "And this is why you can hold up to 160 rockets.",
        "The magazine for it is bigger than your head.",
        "Jesus christ this dumb thing is so heavy.",
        "Actually, maybe accept this substitute.",
        "You picked up a cartoony church organ!",
    },
};
