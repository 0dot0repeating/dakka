// Class data whoa

// First, how many classes are we talking about here?
#define CLASSCOUNT_DEFINED  4

// But we're not actually going to use that number a lot, because Cl_Unknown
//  exists. This is typically what we want.
#define CLASSCOUNT  (CLASSCOUNT_DEFINED + 1)



// Define constants to match to classes here. No one wants to deal with
//
//      if (Pickup_ClassNumber() == 3)
//
// when trying to figure out what the hell you mean.
//
// Cl_Unknown should always be defined: it's the class you get when no other
//  one applies.

#define Cl_Unknown              (-1)
#define Cl_Test_ClipGuy         0
#define Cl_Test_ShellGuy        1
#define Cl_Test_RocketGuy       2
#define Cl_Test_CellGuy         2



// This is handy.

int ClassNames[CLASSCOUNT] = 
{
    "Unknown",
    "Clipguy",
    "Shellguy",
    "Rocketguy",
    "Cellguy",
};



// This is the default state an item on display will jump to if it isn't
//  scripted. It's used in pickup_display.h, but it's here to keep relevant
//  data in the same place.

int DISP_ClassStates[CLASSCOUNT_DEFINED] =
{
    "Clipguy",
    "Shellguy",
    "Rocketguy",
    "Cellguy",
};



// Now, in the past I've done class differentiation purely through inventory
//  checks, but that's not always the best way to do it. So I'm giving the
//  option to use either an inventory check, a class name check, or a script-
//  based check to determine class. First class found is used.
//
// FINDBYINV     finds by inventory item.
//
// FINDBYNAME    finds by actor class name.
//
// FINDBYSCRIPT  finds by calling a given script with the provided arguments.
//
// FINDBYNSCRIPT finds by calling a given *named* script. This doesn't work
//               Zandronum 2.0, but will in 3.0. If you're using this in Z&,
//               don't use this.

#define CLASS_FINDBYINV     0
#define CLASS_FINDBYNAME    1
#define CLASS_FINDBYSCRIPT  2
#define CLASS_FINDBYNSCRIPT 3

int ClassCheckMethod[CLASSCOUNT_DEFINED] =
{
    CLASS_FINDBYSCRIPT,
    CLASS_FINDBYSCRIPT,
    CLASS_FINDBYSCRIPT,
    CLASS_FINDBYSCRIPT,
};



// This is for FINDBYINV. I'd adjust this down as low as possible, just to
//  speed things up some.
#define CLASS_INVCHECKCOUNT     1

// And the actual items.
int ClassCheck_ByInv[CLASSCOUNT_DEFINED][CLASS_INVCHECKCOUNT] =
{
    {""},
    {""},
    {""},
    {""},
};


// This is for FINDBYNAME. Again, adjust down as low as possible.
#define CLASS_NAMECHECKCOUNT    1

// Etc.
int ClassCheck_ByName[CLASSCOUNT_DEFINED][CLASS_NAMECHECKCOUNT] =
{
    {""},
    {""},
    {""},
    {""},
};


// And this is for FINDBYSCRIPT. You only get to call one script, but you can
//  specify whatever three arguments you want for it. Script economy go!
//
// The first argument passed is always the TID to check.
//
// You probably can't use the third argument in Zandronum, so be warned.

int ClassCheck_ByScript[CLASSCOUNT_DEFINED][4] =
{
    {490, 0, 0, 0},
    {490, 1, 0, 0},
    {490, 2, 0, 0},
    {490, 3, 0, 0},
};


// Same deal, but for FINDBYNSCRIPT. Arguments have to be stored in ByScript,
//  however. Mix strings and integers in an array and things go bad.

int ClassCheck_ByNamedScript[CLASSCOUNT_DEFINED] =
{
    "",
    "",
    "",
    "",
};






// ========
// == FUNCTIONS
// ========


// Used by Pickup_ClassNumber to check inventory items,
//  because I hate indentation.
function int Pickup_CheckInv(int tid, int classNum)
{
    int i;

    for (i = 0; i < CLASS_INVCHECKCOUNT; i++)
    {
        int item = ClassCheck_ByInv[classNum][i];
        if (stringBlank(item)) { continue; }

        if (tid == 0)
        {
            if (CheckInventory(item)) { return true; }
        }
        else
        {
            if (CheckActorInventory(tid, item)) { return true; }
        }
    }

    return false;
}


// Used by Pickup_ClassNumber to check class names.
//  Still hate indentation.
function int Pickup_CheckName(int tid, int classNum)
{
    int i;

    for (i = 0; i < CLASS_NAMECHECKCOUNT; i++)
    {
        if (CheckActorClass(tid, ClassCheck_ByName[classNum][i]))
        {
            return true;
        }
    }

    return false;
}


// Function's name is pretty self-explanatory.
function int Pickup_ClassNumber(int tid)
{
    int i;
    int found = false;

    for (i = 0; i < CLASSCOUNT_DEFINED; i++)
    {
        int checkType = ClassCheckMethod[i];

        switch (checkType)
        {
          case CLASS_FINDBYINV:
            found = Pickup_CheckInv(tid, i);
            break;

          
          case CLASS_FINDBYNAME:
            found = Pickup_CheckName(tid, i);
            break;

          case CLASS_FINDBYSCRIPT:
            found = ACS_ExecuteWithResult(ClassCheck_ByScript[i][0],
                                          tid,
                                          ClassCheck_ByScript[i][1],
                                          ClassCheck_ByScript[i][2],
                                          ClassCheck_ByScript[i][3]);
            break;

          case CLASS_FINDBYNSCRIPT:
            found = ACS_ExecuteWithResult(ClassCheck_ByNamedScript[i],
                                          tid,
                                          ClassCheck_ByScript[i][1],
                                          ClassCheck_ByScript[i][2],
                                          ClassCheck_ByScript[i][3]);
            break;
        }

        if (found) { return i; }
    }

    return -1;
}
