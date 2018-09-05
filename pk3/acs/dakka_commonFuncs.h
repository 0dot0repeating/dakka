// I managed to hit ACC's function limit, so I pruned this down to what DAKKA actually uses

#define PLAYERMAX 64

// GZDoom uses a more accurate timer, so now we can just assume 1 second = 35 tics again
#define SECOND_TICS         35.0

int CommonFuncs_ZeroString = "[commonFuncs.h: string index #0]";

int msgColors[22] =
{
    "\ca", "\cb", "\cc", "\cd", "\ce", "\cf", "\cg", "\ch", "\ci", "\cj", "\ck",
    "\cl", "\cm", "\cn", "\co", "\cp", "\cq", "\cr", "\cs", "\ct", "\cu", "\cv"
};

function int itof(int x) { return x << 16; }

function int safeAdd(int a, int b)
{
    if (b > 0 && (a > 0x7FFFFFFF - b)) { return 0x7FFFFFFF; }
    if (b < 0 && (a < 0x80000000 - b)) { return 0x80000000; }

    return a + b;
}


// multiply integer by fraction, and avoid overflows
function int fractionMult(int num, int mult, int div)
{
    return ((num / div) * mult) + (((num % div) * mult) / div);
}

// convert to fixed point and divide, while avoiding overflows
//  - specialized version of fractionMult above
function int itofDiv(int x, int div)
{
    return ((x / div) << 16) + ((x % div) << 16) / div;
}


function int abs(int x)
{
    if (x < 0) { return -x; }
    return x;
}

function int sign(int x)
{
    if (x < 0)  { return -1; }
    if (x == 0) { return 0; }
    return 1;
}

function int randSign(void)
{
    return (2*random(0,1))-1;
}


function int mod(int x, int y)
{
    int ret = x - ((x / y) * y);
    if (ret < 0) { ret = y + ret; }
    return ret;
}

function int powFloat(int x, int y)
{
    int n = 1.0;
    while (y-- > 0) { n = FixedMul(n, x); }
    return n;
}


function int min(int x, int y)
{
    if (x < y) { return x; }
    return y;
}

function int max(int x, int y)
{
    if (x > y) { return x; }
    return y;
}

function int middle(int x, int y, int z)
{
    if ((x < z) && (y < z)) { return max(x, y); }
    return max(min(x, y), z);
}


function int percFloat(int intg, int frac)
{
    return itof(intg) + itofDiv(frac, 100);
}



function int inputUp(int input)
{
    if ((~GetPlayerInput(-1, MODINPUT_BUTTONS) & input) == input) { return 1; }
    return 0;
}

function int inputUp_any(int input)
{
    if (~GetPlayerInput(-1, MODINPUT_BUTTONS) & input) { return 1; }
    return 0;
}

function int inputDown(int input)
{
    if ((GetPlayerInput(-1, MODINPUT_BUTTONS) & input) == input) { return 1; }
    return 0;
}

function int inputDown_any(int input)
{
    if (GetPlayerInput(-1, MODINPUT_BUTTONS) & input) { return 1; }
    return 0;
}

function int inputsPressed(void)
{
    return GetPlayerInput(-1, MODINPUT_BUTTONS) & ~GetPlayerInput(-1, MODINPUT_OLDBUTTONS);
}

function int inputsReleased(void)
{
    return ~GetPlayerInput(-1, MODINPUT_BUTTONS) & GetPlayerInput(-1, MODINPUT_OLDBUTTONS);
}

function int inputPressed(int input)
{
    if ((inputsPressed() & input) == input) { return 1; }
    return 0;
}

function int inputPressed_any(int input)
{
    if (inputsPressed() & input) { return 1; }
    return 0;
}

function int inputReleased(int input)
{
    if ((inputsReleased() & input) == input) { return 1; }
    return 0;
}

function int inputReleased_any(int input)
{
    if (inputsReleased() & input) { return 1; }
    return 0;
}


function int isPrintable(int c)
{
    return ((c > 8) && (c < 14)) || ((c > 31) && (c < 127)) || ((c > 160) && (c < 173));
}


// Strip out color codes and unprintable characters
function int cleanString(int string)
{
    int ret = "";
    int strSize = StrLen(string);

    int c, i;

    int inLongColor = false;
    int inColorCode = false;
    int foundChar28 = false;

    for (i = 0; i < strSize; i++)
    {
        c = GetChar(string, i);

        if (inColorCode)
        {
            if (foundChar28 && c == '[')
            {
                inLongColor = true;
            }

            if (!inLongColor || (c == ']'))
            {
                inColorCode = false;
            }

            foundChar28 = false;
        }
        else
        {
            if (c == 28)
            {
                foundChar28 = true;
                inColorCode = true;
                inLongColor = false;
            }
            else if (isPrintable(c))
            {
                ret = StrParam(s:ret, c:c);
            }
        }
    }

    return ret;
}


// Get string slice, like python's slicing operator
function int sliceString(int string, int start, int end)
{
    int len = StrLen(string);
    int ret = "";
    int i;

    if (start < 0)
    {
        start = len + start;
    }

    if (end <= 0)
    {
        end = len + end;
    }

    start = max(0, start);
    end   = min(end, len);

    for (i = start; i < end; i++)
    {
        ret = StrParam(s:ret, c:GetChar(string, i));
    }

    return ret;
}


// Replace <from> with <to> in string
function int strsub(int string, int from, int to)
{
    int ret = "";
    int len = StrLen(string);
    int fromLen = StrLen(from);

    int i, j, c;
    int charsFound  = 0;
    int lastEnd     = -1;
    int lastWasWord = 0;

    for (i = 0; i < len; i++)
    {
        int chr     = GetChar(string, i);
        int fromChr = GetChar(from, charsFound);
        lastWasWord = 0;

        if (chr == fromChr)
        {
            charsFound++;

            if (charsFound == fromLen)
            {
                charsFound  = 0;
                lastEnd     = i;
                lastWasWord = 1;

                ret = StrParam(s:ret, s:to);
            }
        }
        else
        {
            for (j = 0; j <= charsFound; j++)
            {
                c = GetChar(string, lastEnd + j + 1);
                if (c == 92) { ret = StrParam(s:ret, s:"\\"); }
                else { ret = StrParam(s:ret, c:c); }
            }

            charsFound  = 0;
            lastEnd     = i;
        }
    }

    if (!lastWasWord) // dump whatever is left
    {
        for (j = 0; j <= charsFound; j++)
        {
            c = GetChar(string, lastEnd + j + 1);
            if (c == 92) { ret = StrParam(s:ret, s:"\\"); }
            else { ret = StrParam(s:ret, c:c); }
        }
    }


    return ret;
}


function int getMaxHealth(void)
{
    int maxHP = GetActorProperty(0, APROP_SpawnHealth);

    if ((maxHP == 0) && (PlayerNumber() != -1))
    {
        maxHP = 100;
    }

    return maxHP;
}


function int isDead(int tid)
{
    return GetActorProperty(tid, APROP_Health) <= 0;
}


function void SetInventory(int item, int amount)
{
    int count = CheckInventory(item);

    if (count == amount) { return; }

    if (count > amount)
    {
        TakeInventory(item, count - amount);
        return;
    }

    GiveAmmo(item, amount - count);
    return;
}


function int ToggleInventory(int inv)
{
    if (CheckInventory(inv))
    {
        TakeInventory(inv, 0x7FFFFFFF);
        return 0;
    }

    GiveInventory(inv, 1);
    return 1;
}


function void GiveAmmo(int type, int amount)
{
    if (GetCVar("sv_doubleammo"))
    {
        int expected = CheckInventory(type) + amount;

        GiveInventory(type, amount);
        TakeInventory(type, CheckInventory(type) - expected);
    }
    else
    {
        GiveInventory(type, amount);
    }
}


function int cond(int test, int trueRet, int falseRet)
{
    if (test) { return trueRet; }
    return falseRet;
}


function int onGround(int tid)
{
    return (GetActorZ(tid) - GetActorFloorZ(tid)) == 0;
}


function int defaultTID(int def)
{
    return _defaulttid(def, 0);
}

function int _defaulttid(int def, int alwaysPropagate)
{
    if (IsWorld()) { return 0; }

    int tid = ActivatorTID();

    if (tid == 0 || IsTIDUsed(tid))
    {
        if (def <= 0)
        {
            tid = UniqueTID();
        }
        else
        {
            tid = def;
        }

        Thing_ChangeTID(0, tid);
    }
    else if (alwaysPropagate) { Thing_ChangeTID(0, tid); }

    return tid;
}


function int HeightFromJumpZ(int jumpz, int gravFactor)
{
    if (jumpz < 0) { return 0; }
    return FixedDiv(FixedMul(jumpz, jumpz), gravFactor << 1);
}

function int JumpZFromHeight(int height, int gravFactor)
{
    return FixedSqrt(2 * height * gravFactor);
}


function int oldRound(int toround)
{
    return (toround + 0.5) >> 16;
}


function int roundTo(int num, int interval)
{
    return ((num + (interval >> 1)) / interval) * interval;
}


// Shortest distance between two angles
function int angleDifference(int ang1, int ang2)
{
    ang1 = mod(ang1, 1.0);
    ang2 = mod(ang2, 1.0);

    int angDiff = ang2 - ang1;

    if (angDiff >= 0.5) { return angDiff - 1.0; }
    if (angDiff < -0.5) { return angDiff + 1.0; }
    return angDiff;
}


// Dot product of 2D vectors
function int dot2(int x1, int y1, int x2, int y2)
{
    return FixedMul(x1, x2) + FixedMul(y1, y2);
}


// Dot product of 3D vectors
function int dot3(int x1, int y1, int z1, int x2, int y2, int z2)
{
    return FixedMul(x1, x2) + FixedMul(y1, y2) + FixedMul(z1, z2);
}


function int acos(int f)
{
    if(f > 1.0 || f < -1.0) f %= 1.0; // range bound
    return VectorAngle(f, FixedSqrt(1.0 - FixedMul(f, f)));
}



// Check for blank string.
//
// The CommonFuncs_ZeroString check is only safe because the very first
//  string I defined - in commonFuncs.h - is a placeholder string
//  specifically meant to use up string index 0 without actually using
//  it for anything. Without a string like this, this is NOT a safe check.
//
// And we use CommonFuncs_ZeroString directly because that's safe to use
//  in libraries. If we just compare to 0, other ACS that gets loaded will
//  move CommonFuncs_ZeroString to something other than 0, breaking this.
//
// Thanks, ACC.
//
// Also, we need to use StrParam because a nonexistent string that has StrLen
//  done on it will crash the game. StrParam makes a valid string, and doesn't
//  crash if given a nonexistent string. Yay.
function int stringBlank(int string)
{
    if (string == CommonFuncs_ZeroString) { return true; }

    int safeString = StrParam(s:string);
    return StrLen(safeString) == 0;
}


int Rotate3D_Ret[3];

function void Rotate3D(int x, int y, int z, int yaw, int pitch)
{
    // x' =  cos(angle) cos(pitch)x + -sin(angle)y + cos(angle) sin(pitch)z
    // y' =  sin(angle) cos(pitch)x +  cos(angle)y + sin(angle) sin(pitch)z
    // z' =            -sin(pitch)x                +            cos(pitch)z
    //
    // helix.txt shows how (very abbreviated)

    Rotate3D_Ret[0] = FixedMul(x, FixedMul(cos(yaw), cos(pitch)))
                    - FixedMul(y,          sin(yaw))
                    + FixedMul(z, FixedMul(cos(yaw), sin(pitch)));

    Rotate3D_Ret[1] = FixedMul(x, FixedMul(sin(yaw), cos(pitch)))
                    + FixedMul(y,          cos(yaw))
                    + FixedMul(z, FixedMul(sin(yaw), sin(pitch)));

    Rotate3D_Ret[2] = FixedMul(x,                   -sin(pitch))
                    + FixedMul(z,                    cos(pitch));
}


function int ActivatorToPlayer(int i)
{
    if (i < 0 || i >= PLAYERMAX) { return false; }
    if (i < 8) { SetActivator(0, AAPTR_PLAYER1 << i); }
    return SetActivatorToPlayer(i);
}


function int HasInfiniteAmmo(void)
{
    return GetCVar("sv_infiniteammo") || CheckInventory("PowerInfiniteAmmo");
}


function int IsWorld(void)
{
    return ClassifyActor(0) & ACTOR_WORLD;
}