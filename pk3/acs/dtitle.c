#library "dakka_titlemap"

#include "zcommon/zcommon.h"
#include "dakka_commonFuncs.h"


#define MUSICCOUNT  3

int TitleMusic[MUSICCOUNT] =
{
    "DKTITLE1",
    "DKTITLE2",
    "DKTITLE3",
};


script "dakka_titlemap" open
{
    if (GameType() != GAME_TITLE_MAP) { terminate; }
    if (stricmp(StrParam(n:PRINTNAME_LEVELNAME), "DAKKA")) { terminate; }

    SetHudSize(320, 200, true);
    FadeRange(0,0,0, 1.0, 0,0,0, 1.0, 10.0);

    SetFont("TITLEPIC");
    HudMessage(s:"A"; HUDMSG_FADEOUT, 1, CR_UNTRANSLATED, 160.4, 100.0, 2.0, 1.0);

    Delay(105);

    // dakka title stuff here

    AmbientSound("dakka/hammerwall", 127);

    SetFont("DKLOGOF1");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, 1.0);
    Delay(2);
    SetFont("DKLOGON1");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, 1.0);

    Delay(10);

    AmbientSound("dakka/hammerwall", 127);

    SetFont("DKLOGOF2");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, 1.0);
    Delay(2);
    SetFont("DKLOGON2");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, -1);

    Delay(10);

    AmbientSound("dakka/hammerwall", 127);

    SetFont("DKLOGOF3");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, 1.0);
    Delay(2);
    SetFont("DKLOGON3");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, -1);

    Delay(10);

    AmbientSound("dakka/hammerwall", 127);

    SetFont("DKLOGOF4");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, 1.0);
    Delay(2);
    SetFont("DKLOGON4");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, -1);

    Delay(10);

    ACS_NamedExecuteWithResult("titlemap_showtips");

    AmbientSound("dakka/hammerwall", 127);

    SetFont("DKLOGOF5");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, 1.0);
    Delay(2);
    SetFont("DKLOGON5");
    HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, -1);

    ACS_NamedExecuteWithResult("titlemap_logoflash");

    Delay(35);

    SetMusic(TitleMusic[random(0, MUSICCOUNT-1)]);

    Delay(35);

    ACS_NamedExecuteWithResult("titlemap_playblink");

    // 140 + (4 * (10 + 2)) 2 + 35 + x = 350
    Delay(90);

    FadeRange(0,0,0, 1.0, 0,0,0, 0.0, 2.0);

    ACS_NamedExecuteWithResult("titlemap_camera");
    ACS_NamedExecuteWithResult("titlemap_spinny");
}


#define PIVOTANGLE_MIDDLE   0.875
#define PIVOTANGLE_SWAY     0.125
#define PIVOT_DISTANCE      2048.0
#define PIVOT_PERIOD        1050

script "titlemap_camera" (void)
{
    ChangeCamera(2, false, false);

    int pivotX = GetActorX(1);
    int pivotY = GetActorY(1);
    int pivotZ = GetActorZ(1);

    int ranOnce = false;

    while (true)
    {
        int pivotStep  = sin((Timer() * 1.0) / PIVOT_PERIOD);
        int pivotAngle = PIVOTANGLE_MIDDLE + FixedMul(pivotStep, PIVOTANGLE_SWAY);

        int fromX = GetActorX(2);
        int fromY = GetActorY(2);
        int fromZ = GetActorZ(2);

        int toX = pivotX + FixedMul(PIVOT_DISTANCE, cos(pivotAngle));
        int toY = pivotY + FixedMul(PIVOT_DISTANCE, sin(pivotAngle));
        int toZ = GetActorZ(1);

        SetActorAngle(2, pivotAngle - 0.5);

        if (!ranOnce)
        {
            SetActorPosition(2, toX, toY, toZ, false);
            ranOnce = true;
        }
        else
        {
            SetActorVelocity(2, toX - fromX, toY - fromY, toZ - fromZ, false, false);
        }

        Delay(1);
    }
}

script "titlemap_spinny" (void)
{
    while (true)
    {
        SetActorAngle(3, GetActorAngle(3) + 0.05);

        int fun = random(1.5, 2.5);

        int velX = FixedMul(fun, cos(GetActorAngle(3)));
        int velY = FixedMul(fun, sin(GetActorAngle(3)));

        SetActorVelocity(3, velX, velY, 0, true, true);

        Delay(1);
    }
}



#define DLOGO_FRAMECOUNT 21

int DakkaLogo_Frames[DLOGO_FRAMECOUNT] =
{
    "DLOGOS01",
    "DLOGOS02",
    "DLOGOS03",
    "DLOGOS04",
    "DLOGOS05",
    "DLOGOS06",
    "DLOGOS07",
    "DLOGOS08",
    "DLOGOS09",
    "DLOGOS10",
    "DLOGOS11",
    "DLOGOS12",
    "DLOGOS13",
    "DLOGOS14",
    "DLOGOS15",
    "DLOGOS16",
    "DLOGOS17",
    "DLOGOS18",
    "DLOGOS19",
    "DLOGOS20",
    "DLOGOS21",
};

script "titlemap_logoflash" (void)
{
    Delay(random(70, 105));

    while (true)
    {
        int i;

        for (i = 0; i < DLOGO_FRAMECOUNT; i++)
        {
            SetFont(DakkaLogo_Frames[i]);
            SetHudSize(320, 200, true);
            HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, -1);
            Delay(1);
        }

        SetHudSize(320, 200, true);
        SetFont("DKLOGON5");
        HudMessage(s:"A"; HUDMSG_PLAIN, 2, CR_UNTRANSLATED, 160.4, 10.1, -1);

        Delay(random(140, 280));
    }
}

script "titlemap_playblink" (void)
{
    SetFont("DKPLAY");
    SetHudSize(320, 200, true);

    while (true)
    {
        HudMessage(s:"A"; HUDMSG_PLAIN, 3, CR_UNTRANSLATED, 160.4, 195.2, -1);
        Delay(24);

        HudMessage(s:""; HUDMSG_PLAIN, 3, CR_UNTRANSLATED, 160.4, 195.2, -1);
        Delay(24);
    }
}


script "titlemap_showtips" (void)
{
    Delay(12);
    ACS_NamedExecuteWithResult("titlemap_weptips", true, 10.0 + (1.0/3));

    Delay(24);
    ACS_NamedExecuteWithResult("titlemap_scoretips", true, 10.0);

    Delay(385);
    
    int lastGameTip = 0x7FFFFFFF;

    while (true)
    {
        int i;

        for (i = 0; i < 3; i++)
        {
            int msgDelay;

            if (lastGameTip >= 8 || random(0, 4) == 0)
            {
                msgDelay = ACS_NamedExecuteWithResult("titlemap_gametips");
                lastGameTip = 0;
            }
            else
            {
                msgDelay = ACS_NamedExecuteWithResult("titlemap_dumbtips");
                lastGameTip++;
            }

            Delay(70 + msgDelay);
        }

        ACS_NamedExecuteWithResult("titlemap_weptips",   false, 4.0);
        ACS_NamedExecuteWithResult("titlemap_scoretips", false, 4.0);

        Delay(210);
    }
}


function void ShowWeaponTips(int doFade, int forceColor, int overrideColor, int duration)
{
    int msgMode = HUDMSG_FADEOUT;
    if (doFade) { msgMode = HUDMSG_FADEINOUT; }

    int fistColor       = "DTitle_Fists";
    int scrapperColor   = "DTitle_Scrappers";
    int pistolColor     = "DTitle_Pistol";
    int pistolsColor    = "DTitle_Pistols";
    int shotgunColor    = "DTitle_Shotgun";
    int ssgColor        = "DTitle_SuperShotgun";
    int minigunColor    = "DTitle_Minigun";
    int impalerColor    = "Red";
    int plasmaColor     = "DTitle_PlasmaLance";
    int bfgColor        = "Green";

    if (forceColor)
    {
        fistColor       = overrideColor;
        scrapperColor   = overrideColor;
        pistolColor     = overrideColor;
        pistolsColor    = overrideColor;
        shotgunColor    = overrideColor;
        ssgColor        = overrideColor;
        minigunColor    = overrideColor;
        impalerColor    = overrideColor;
        plasmaColor     = overrideColor;
        bfgColor        = overrideColor;
    }

    SetHudSize(320, 200, 0);
    SetFont("ARCFONT");

    HudMessage(s:"Fists";         msgMode | HUDMSG_COLORSTRING, 21, fistColor,        85.4, 85.0, duration, 1.0, 1.0);
    HudMessage(s:"Scrappers";     msgMode | HUDMSG_COLORSTRING, 22, scrapperColor,    85.4, 93.0, duration, 1.0, 1.0);

    HudMessage(s:"Pistols";       msgMode | HUDMSG_COLORSTRING, 23, pistolsColor,    160.4,  89.0, duration, 1.0, 1.0);

    HudMessage(s:"Shotgun";       msgMode | HUDMSG_COLORSTRING, 24, shotgunColor,    235.4,  85.0, duration, 1.0, 1.0);
    HudMessage(s:"S. Shotgun";    msgMode | HUDMSG_COLORSTRING, 25, ssgColor,        235.4,  93.0, duration, 1.0, 1.0);

    HudMessage(s:"Minigun";       msgMode | HUDMSG_COLORSTRING, 26, minigunColor,    70.4,  109.0, duration, 1.0, 1.0);

    HudMessage(s:"Impaler";       msgMode | HUDMSG_COLORSTRING, 27, impalerColor,    130.4, 109.0, duration, 1.0, 1.0);

    HudMessage(s:"Lancer";        msgMode | HUDMSG_COLORSTRING, 28, plasmaColor,     190.4, 109.0, duration, 1.0, 1.0);

    HudMessage(s:"BFG9000";       msgMode | HUDMSG_COLORSTRING, 29, bfgColor,        250.4, 109.0, duration, 1.0, 1.0);
}

script "titlemap_weptips" (int flash, int duration)
{
    SetHudSize(320, 200, 0);
    SetFont("ARCFONT");

    if (flash)
    {
        AmbientSound("dakka/hammerwall", 127);
        HudMessage(s:"Weapons:"; HUDMSG_PLAIN | HUDMSG_COLORSTRING, 11, "DTitle_AllWhite", 160.4, 70.0, 0);
        Delay(2);
        HudMessage(s:"Weapons:"; HUDMSG_PLAIN, 11, CR_WHITE, 160.4, 70.0, 0);

        Delay(10);

        AmbientSound("dakka/hammerwall", 127);
        ShowWeaponTips(false, true, "DTitle_AllWhite", duration);
        Delay(2);

        // do this here to sync them up
        HudMessage(s:"Weapons:"; HUDMSG_FADEOUT, 11, CR_WHITE, 160.4, 70.0, duration, 1.0);
        ShowWeaponTips(false, false, "", duration);
    }
    else
    {
        HudMessage(s:"Weapons:"; HUDMSG_FADEINOUT, 11, CR_WHITE, 160.4, 70.0, duration, 1.0, 1.0);
        ShowWeaponTips(true, false, "", duration);
    }
}

int ScoreMessage = "\
First point reward\n\
\ckAmmo regen\n\
\n\
Second point reward\n\
\ckExtra life";

script "titlemap_scoretips" (int flash, int duration)
{
    SetHudSize(320, 200, 0);
    SetFont("ARCFONT");

    if (flash)
    {
        AmbientSound("dakka/hammerwall", 127);
        HudMessage(s:ScoreMessage; HUDMSG_PLAIN | HUDMSG_COLORSTRING, 41, "DTitle_AllWhite",     160.4, 145.0, 0);
        Delay(2);
        HudMessage(s:ScoreMessage; HUDMSG_FADEOUT | HUDMSG_COLORSTRING, 41, "White",             160.4, 145.0, duration, 1.0);
    }
    else
    {
        HudMessage(s:ScoreMessage; HUDMSG_FADEINOUT | HUDMSG_COLORSTRING, 41, "White",           160.4, 145.0, duration, 1.0, 1.0);
    }
}


#define GAMETIPCOUNT    23

int GameTips[GAMETIPCOUNT] =
{
    // General
    "Dakkaguy takes reduced damage from his own explosions. So go on, rocket jump to that key and break the map. You know you want to...",
    "Your weapons can still be swapped out while in a firing animation, so take advantage of it!",
    "By default, your ammo gets reset when you beat a map. Don't hoard it - use it!",

    // Fist
    "Your fists are silent, which is perfect for breaking those maps that force you to shoot a switch at the start.\n\nOr you could just shoot the switch anyway, since it lets you kill more demons.",
    "You can charge up the fist's alt-fire up to three times, and at max charge, your punch will ruin the day of both whoever you hit and his buddies next to him!",
    "Heavy punches will greatly speed up your light punches, but if you miss, all that speed goes into the trash. So don't miss!",

    // Scrappers
    "Spent ammo of all kinds refuels your scrappers. Respect the environment; scrap some demons before that meter tops off.",
    "Scrappers + small places = very dead demons.",

    // Pistols
    "Your pistols are always fully loaded when you pull them out, making them great for saving ammo for your other guns!",
    "The faster you fire your pistols, the less accurate they get. Space your shots out a bit if you're trying to snipe.",

    // Shotgun
    "If you disable the \cdGrenades auto-detonate\c- setting, shotgun grenades will stick to whatever they hit. Detonate them with the \cfreload\c- key!",
    "Shotgun grenades can be detonated in midair with the \cfreload\c- key, which causes all the mini-grenades to fly forward instead of all around. Handy!",

    // SSG
    "Dakkaguy can reload the super shotgun with his buttcheeks. This makes it great for a quick, heavy burst of close-range damage!",
    "You know how Dakkaguy reloads the super shotgun even if it isn't empty yet? You can still shoot when he does that.",
    "With \cdSSG trigger rolling\c- enabled, pulling both triggers in rapid succession will use the first trigger twice. This can even let you fire four shells at the same time!",

    // Minigun
    "The minigun primary fire has two speeds: full auto if you just hold the primary fire, and \caFULLER AUTO\c- if you hold both fire buttons!\n\nIt burns through ammo twice as quickly, but that's the fun part!",
    "The minigun's flamethrower is great at weeding out weaker enemies in a crowd - if the flamethrower itself doesn't kill them, the afterburn will.",
    "Minigun flames pierce invulnerability.\nBecause why not?",

    // Impaler
    "The impaler's primary fire has most of its explosion directed forward, while the alt-fire's explosion is all around. Keeping this in mind will save you rockets.",
    "The impaler's alt-fire impales demons, surprisingly enough. Use it to bring stragglers close to their friends and blow them all up!",

    // Lancer
    "Plasma nails pierce through enemies, so line them up and get a two-for-one deal on your cells!",
    "The lancer's arcs like to travel forward as much as possible. Line up your enemies when using it; it's more effective that way.",
    
    // BFG9000
    "If you aim at something shootable, the BFG targets that enemy for a second instead of your crosshair. So don't worry so much about keeping your aim steady.",
};


#define GAMEHEADERCOUNT 6

int GameHeaders[GAMEHEADERCOUNT] =
{
    "Quick Tip:",
    "Tip in a Can:",
    "I bet you knew this already:",
    "Here's how this thing works:",
    "Loading, please wait...",
    "PRO DAKKA STRATS",
};


#define DUMBTIPCOUNT 93

int DumbTips[DUMBTIPCOUNT] =
{
    "When all you have is a minigun, everything starts looking like something that miniguns are good for. Which is everything.",
    "Balance is for chumps.",
    "ru \caDKAKGUY\c- n u mstu \caFGITH THE DMOENS\c- whi \caPOWEFRL WEAPONS\c- leki the \caSHTONGY OF SUEPR\c- to averge \caTHE WLROD\n\ngo now \caDAKAKGYU\c- and svu \caHRMUINTY\c-",
    "Shoot demons! Get points! Get ammo with those points! Shoot demons with that ammo!",
    "My immersion is ruined forever.",
    "What was the benchmark for quality before sliced bread?",
    "I'd buy this for a dollar.",
    "It's always goddamn skeletons.",
    "It is a mystery.",
    "One of these days, I am going to get a time machine, travel back to the Ion Storm offices of 1998, and punch John Romero in the face.",
    "Your opinion, my choice.",
    "Ford, you're turning into a penguin. Stop it.",
    "How was I supposed to know the big red glowing button labeled \"SELF DESTRUCT, DO NOT PUSH\" was going to do something?",
    "You know how the Doomguy has a pet rabbit? Well, Dakkaguy has a pet rocket. It's a friendly rocket. Never made a mess in the house, never barked at anyone - it was a well behaved rocket. Well, one day, as Hell was invading, some imp picked up the innocent rocket and slashed it. The rocket exploded in his face.\n\nDeserved it, too. Fucking fascist.",
    "This mod's been around for four years in some form or another. And pretty much all of those other forms are terrible!",
    "No wang for you.",
    "Roll to fondle yourself.",
    "What are you gonna do, stab me?",
    "To defeat the Cyberdemon, shoot at it until it dies.",
    "Is there insurance against getting played like a damn fiddle?",
    "Hey, chaingun! The hell with respect!",
    "Dakka's guns work by one rule: would it be cool if they worked like that? If no, then they don't work like that.",
    "If a tree falls in the woods and no one is around to hear it, it should've had LifeAlert.",
    "You are huge! That means you're worth a lot of points!",
    "If you are reading this message, that means you are reading this message.",
    "Now with 100% more percentages!",
    "Perhaps the same could be said of all religions.",
    "Only you can prevent the world from being plunged balls-deep into an asspocalypse! Do your part today! Join the Buttpain Army!",
    "RIP Cyberrunner 2013-2013 - it was too fast for Doom",
    "In the beginning were the words, and the words were \"fucking hacker il report u\".",
    "The year is 2047. The cybernetic revolution is full force. Cyborgs walk the street, and while some still give them trouble, the majority simply regard it as everyday life. Man and machine have conjoined inseparably, and even as transcendence nears us, The Space Pirate has still not seen an update.",
    "That one marine spinning forever is both the solution to our energy troubles, and the reason the demons invaded. It's canon now.",
    "\"All you had to do was not punch an astronaut.\"\n\n\"Sometimes it's hard not to punch an astronaut...\"",
    "Soylent pink is made of demons! \caDEMONS!",
    "Do not pet the cacodemon.",
    "The UAC should build a wall to keep the demons out! And then make Hell pay for it!",
    "Insert coin to continue.",
    "Welcome to die!",
    "Those demon bastards are gonna pay for screwing up my vacation.",
    "Just because you can't cram four shells into a double-barreled shotgun and have it actually work doesn't mean I won't try anyway.",
    "As it turns out, the demons were in your heart all along.\n\nThat's probably a bad thing.",
    "Stay awhile and listen!",
    "Greetings, mortal. Are you ready to die?",
    "Eat the path. Literally eat this dirt path. Here's some ketchup and a knife. Get to it.",
    "It's not workplace violence if you don't work there.",
    "So where's the revenants with jetpacks? I was promised revenants with jetpacks.",
    "\cd<\casystem error \cf0xfded\cd>\c-",
    "Welcome to the world of a minute from now!",
    "What's that? I can't hear you over this endless torrent of bullets!",
    "The leg bone is connected to the cash bone!",
    "You know what's the worst? Inconsistent hurt floors. What, is the water punching me now?",
    "Samsara? What a piece of shit.",
    "omg this mod rips off everything so much",
    "Because what even is running out of ammo?",
    "Why are you letting some dumb text on the title screen of an unbalanced weapon mod distract you? Go shoot some demons already!",
    "\caWIZARDS!\c- NO SENSE OF RIGHT OR WRONG!",
    "whoops you fucked up and now you're dead + stupid\n\n\nidiot",
    "Sweet christmas! Big-mouthed floating thingies! It's always something!",
    "Once fusion reactors become sustainable, how long until someone cooks their pizza bagels with one? My guess is \"ten minutes\".",
    "Man, it's actually pretty difficult to think of new messages for this. Oh wait, that's a message right there! How convenient!",
    "You'd think this was a title map for some adventure game or something! But nope, it's a weapon mod. Damn.",
    "Guns don't kill people. Lag kills people.",
    "I love the power glove. It's so bad.",
    "Y'know, in all likelihood, you probably couldn't have prevented this.",
    "To rocket jump, fire a rocket and jump.",
    "Is anyone reading this messages? Anyone? Get me out of here! I need to use the bathroom!",
    "Alright, chums, let's do this.",
    "Whenever you're feeling down in the dumps, remember this: Magneto was defeated by a fake wooden gun. Would you be defeated by a fake wooden gun?\n\nIf the answer is 'yes', make sure you aren't secretly Magneto.",
    "Why aren't you lifting \cgRIGHT NOW?\c-",
    "Mom, get off the phone! I'm downloading something!",
    "Now deleting your computer.",
    "Strange I was chosen, the fool divine\nTo challenge the master of riddle and rhyme\nAnother mountain for me to climb\nI must find the answer\nBefore I run out of time",
    "The stairway to heaven is currently out of order. Please use the ladder instead.",
    "Where is the brouzouf? I was told there'd be brouzouf!",
    "Are you lost? Or have you already been lost since the very beginning?",
    "The only accident I fear will be your head falling into a vat of acid with me wearing your fucking moron face as a party mask.",
    "When will rocket jumping be included at the Olympics? It's a valid sport!",
    "\caHOW TO SURVIVE A NUCLEAR APOCALYPSE:\nStep 1 - Don't get hit.\nStep 2 - don't get blown up.\nStep 3 - make sure to get plenty of water.",
    "Oh, right. The murders.",
    "The sign lied! This ride isn't wild at all! I want off!",
    "It don't mean a thing if it ain't got that SSG.",
    "Do a flip!",
    "Stop! Or I'll say stop again!",
    "Somebody stop the damn match! Enough's enough!",
    "Crush your enemies. Drive them before you. Hear the lamentations of their women. That is what's best in life.",
    "Sometimes, it's not about the destination or the journey. Sometimes it's just about killing things. Those are the better times.",
    "I'm sorry, this line has been disconnected.",
    "I just called to say... I love you.",
    "Laws are optional if you can drive fast enough.",
    "Maybe... maybe I'm the chump.",
    "Slated for a 1.0 release in 2097!",
    "DAKKA: It's better than getting your legs chopped off!",
    "If it moves, shoot it. If it doesn't move, kick it so it moves a bit. Then shoot it.",
};


#define DUMBHEADERCOUNT 11

int DumbHeaders[DUMBHEADERCOUNT] =
{
    "TOP SECRET:",
    "FWD: FWD: FWD: RE: RE: RE: FWD: LOL!!!!",
    "Message of the Second:",
    "Words of the Wiseass:",
    "Dakka sez:",
    "The Magic 8-Ball sez:",
    "Message for you:",
    "?????????",
    "(play some dakka already)",
    "On another note:",
    "Breaking news!",
};

int LastDumbTip = -1;

script "titlemap_dumbtips" (void)
{
    int dumbIndex;

    if (LastDumbTip == -1)
    {
        dumbIndex = random(0, DUMBTIPCOUNT - 1);
    }
    else
    {
        dumbIndex = random(0, DUMBTIPCOUNT - 2);
        if (dumbIndex >= LastDumbTip) { dumbIndex += 1; }
    }

    LastDumbTip = dumbIndex;

    int dumbMessage = DumbTips[dumbIndex];
    int dumbLength  = StrLen(cleanString(dumbMessage));

    int dumbDelay    = 105 + FixedMul(dumbLength, 1.5);
    int dumbDuration = max(0, (dumbDelay * 1.0) / 35);

    int dumbColor = "\c[White]";

    int dumbReplaced = strsub(dumbMessage, "\c-", dumbColor);

    int dumbHeader = DumbHeaders[random(0, DUMBHEADERCOUNT-1)];

    SetHudSize(320, 200, true);
    SetFont("ARCFONT");

    HudMessage(s:dumbHeader;   HUDMSG_FADEINOUT | HUDMSG_COLORSTRING, 51, "Gold",  160.4, 70.1, dumbDuration, 1.0, 1.0);

    SetHudClipRect(30, 0, 262, 200, 260); // buffer for right side of letters
    HudMessage(s:dumbReplaced; HUDMSG_FADEINOUT | HUDMSG_COLORSTRING, 52, "White", 160.4, 85.1, dumbDuration, 1.0, 1.0);
    SetHudClipRect(0, 0, 0, 0, 0);
    SetResultValue(dumbDelay);
}

// I totally did not copypaste this.
int LastGameTip = -1;

script "titlemap_gametips" (void)
{
    int dumbIndex;

    if (LastGameTip == -1)
    {
        dumbIndex = random(0, GAMETIPCOUNT - 1);
    }
    else
    {
        dumbIndex = random(0, GAMETIPCOUNT - 2);
        if (dumbIndex >= LastGameTip) { dumbIndex += 1; }
    }

    LastGameTip = dumbIndex;

    int dumbMessage = GameTips[dumbIndex];
    int dumbLength  = StrLen(cleanString(dumbMessage));

    int dumbDelay    = 105 + FixedMul(dumbLength, 1.5);
    int dumbDuration = max(0, (dumbDelay * 1.0) / 35);

    int dumbColor = "\c[White]";

    int dumbReplaced = strsub(dumbMessage, "\c-", dumbColor);

    int dumbHeader = GameHeaders[random(0, GAMEHEADERCOUNT-1)];

    SetHudSize(320, 200, true);
    SetFont("ARCFONT");

    HudMessage(s:dumbHeader;   HUDMSG_FADEINOUT | HUDMSG_COLORSTRING, 51, "Green", 160.4, 70.1, dumbDuration, 1.0, 1.0);

    SetHudClipRect(30, 0, 262, 200, 260); // buffer for right side of letters
    HudMessage(s:dumbReplaced; HUDMSG_FADEINOUT | HUDMSG_COLORSTRING, 52, "White", 160.4, 85.1, dumbDuration, 1.0, 1.0);
    SetHudClipRect(0, 0, 0, 0, 0);
    SetResultValue(dumbDelay);
}
