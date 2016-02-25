// A bunch of functions that I've built up
// They come in handy :>

#define PLAYERMAX 64
#define TEAMCOUNT 8
#define DEFAULTTID_SCRIPT 471

#define SECOND_TICS 35.714285714285715
#define UNIT_CM     2.73921568627451

int CommonFuncs_ZeroString = "[commonFuncs.h: string index #0]";

int TeamNames[TEAMCOUNT] = 
{
    "Blue", "Red", "Green", "Gold", "Black", "White", "Orange", "Purple"
};

int TeamColors[TEAMCOUNT] = 
{
    CR_BLUE, CR_RED, CR_GREEN, CR_GOLD, CR_BLACK, CR_WHITE, CR_ORANGE, CR_PURPLE
};

int TeamColorCodes[TEAMCOUNT] = 
{
    "\ch", "\cg", "\cd", "\cf", "\cm", "\cj", "\ci", "\ct"
};

int msgColors[22] = 
{
    "\ca", "\cb", "\cc", "\cd", "\ce", "\cf", "\cg", "\ch", "\ci", "\cj", "\ck",
    "\cl", "\cm", "\cn", "\co", "\cp", "\cq", "\cr", "\cs", "\ct", "\cu", "\cv"
};

function int itof(int x) { return x << 16; }
function int ftoi(int x) { return x >> 16; }

function int abs(int x)
{
    if (x < 0) { return -x; }
    return x;
}

function int sign(int x)
{
    if (x < 0) { return -1; }
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

function int pow(int x, int y)
{
    int n = 1;
    while (y-- > 0) { n *= x; }
    return n;
}

function int powFloat(int x, int y)
{
    int n = 1.0;
    while (y-- > 0) { n = FixedMul(n, x); }
    return n;
}

function int gcf(int a, int b)
{
    int c;
    while (1)
    {
        if (b == 0) { return a; }
        c = a % b;
        a = b;
        b = c;
    }
    
    return -1;
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
    return itof(intg) + (itof(frac) / 100);
}

function int percFloat2(int intg, int frac1, int frac2)
{
    return itof(intg) + (itof(frac1) / 100) + (itof(frac2) / 10000);
}

function int keyUp(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if ((~buttons & key) == key) { return 1; }
    return 0;
}

function int keyUp_any(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if (~buttons & key) { return 1; }
    return 0;
}

function int keyDown(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if ((buttons & key) == key) { return 1; }
    return 0;
}

function int keyDown_any(int key)
{
    int buttons = GetPlayerInput(-1, INPUT_BUTTONS);

    if (buttons & key) { return 1; }
    return 0;
}

function int keysPressed(void)
{
    int buttons     = GetPlayerInput(-1, INPUT_BUTTONS);
    int oldbuttons  = GetPlayerInput(-1, INPUT_OLDBUTTONS);
    int newbuttons  = (buttons ^ oldbuttons) & buttons;

    return newbuttons;
}

function int keyPressed(int key)
{
    if ((keysPressed() & key) == key) { return 1; }
    return 0;
}

function int keyPressed_any(int key)
{
    if (keysPressed() & key) { return 1; }
    return 0;
}

function int inputUp(int input)
{
    int buttons = GetPlayerInput(-1, MODINPUT_BUTTONS);

    if ((~buttons & input) == input) { return 1; }
    return 0;
}

function int inputUp_any(int input)
{
    int buttons = GetPlayerInput(-1, MODINPUT_BUTTONS);

    if (~buttons & input) { return 1; }
    return 0;
}

function int inputDown(int input)
{
    int buttons = GetPlayerInput(-1, MODINPUT_BUTTONS);

    if ((buttons & input) == input) { return 1; }
    return 0;
}

function int inputDown_any(int input)
{
    int buttons = GetPlayerInput(-1, MODINPUT_BUTTONS);

    if (buttons & input) { return 1; }
    return 0;
}

function int inputsPressed(void)
{
    int buttons     = GetPlayerInput(-1, MODINPUT_BUTTONS);
    int oldbuttons  = GetPlayerInput(-1, MODINPUT_OLDBUTTONS);
    int newbuttons  = (buttons ^ oldbuttons) & buttons;

    return newbuttons;
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

function int adjustBottom(int tmin, int tmax, int i)
{
    if (tmin > tmax)
    {
        tmax ^= tmin; tmin ^= tmax; tmax ^= tmin;  // XOR swap
    }

    if (i < tmin) { tmin = i; }
    if (i > tmax) { tmin += (i - tmax); }

    return tmin;
}

function int adjustTop(int tmin, int tmax, int i)
{
    if (tmin > tmax)
    {
        tmax ^= tmin; tmin ^= tmax; tmax ^= tmin;
    }

    if (i < tmin) { tmax -= (tmin - i); }
    if (i > tmax) { tmax = i; }

    return tmax;
}

function int adjustShort(int tmin, int tmax, int i)
{
    if (tmin > tmax)
    {
        tmax ^= tmin; tmin ^= tmax; tmax ^= tmin;
    }

    if (i < tmin)
    {
        tmax -= (tmin - i);
        tmin = i;
    }
    if (i > tmax)
    {
        tmin += (i - tmax);
        tmax = i;
    }
    
    return packShorts(tmin, tmax);
}


function int magnitudeTwo(int x, int y)
{
    return sqrt(x*x + y*y);
}

function int magnitudeTwo_f(int x, int y)
{
    int len, ang;

    ang = VectorAngle(x, y);
    if (((ang + 0.125) % 0.5) > 0.25) { len = FixedDiv(y, sin(ang)); }
    else { len = FixedDiv(x, cos(ang)); }

    return len;
}

function int magnitudeThree(int x, int y, int z)
{
    return sqrt(x*x + y*y + z*z);
}

function int magnitudeThree_f(int x, int y, int z)
{
    int len, ang;

    ang = VectorAngle(x, y);
    if (((ang + 0.125) % 0.5) > 0.25) { len = FixedDiv(y, sin(ang)); }
    else { len = FixedDiv(x, cos(ang)); }

    ang = VectorAngle(len, z);
    if (((ang + 0.125) % 0.5) > 0.25) { len = FixedDiv(z, sin(ang)); }
    else { len = FixedDiv(len, cos(ang)); }

    return len;
}

// All the arguments are to be fixed-point
function int quad(int a, int b, int c, int y)
{
    return FixedMul(a, FixedMul(y, y)) + FixedMul(b, y) + c + y;
}

function int inRange(int low, int high, int x)
{
    return ((x >= low) && (x < high));
}

function void AddAmmoCapacity(int type, int add)
{
    SetAmmoCapacity(type, GetAmmoCapacity(type) + add);
}

function int packShorts(int left, int right)
{
    return ((left & 0xFFFF) << 16) + (right & 0xFFFF);
}

function int leftShort(int packed) { return packed >> 16; }
function int rightShort(int packed) { return (packed << 16) >> 16; }


// Strip out color codes
function int cleanString(int string)
{
    int ret = "";
    int strSize = StrLen(string);

    int c, i, ignoreNext;
    
    for (i = 0; i < strSize; i++)
    {
        c = GetChar(string, i);

        if ( ( ((c > 8) && (c < 14)) || ((c > 31) && (c < 127)) || ((c > 160) && (c < 173)) ) && !ignoreNext)
        {
            ret = StrParam(s:ret, c:c);
        }
        else if (c == 28 && !ignoreNext)
        {
            ignoreNext = 1;
        }
        else
        {
            ignoreNext = 0;
        }
    }

    return ret;
}

// Pad right side of string with padChar
function int padStringR(int baseStr, int padChar, int len)
{
    int baseStrLen = StrLen(baseStr);
    int pad = "";
    int padLen; int i;

    if (baseStrLen >= len)
    {
        return baseStr;
    }
    
    padChar = GetChar(padChar, 0);
    padLen = len - baseStrLen;

    for (i = 0; i < padLen; i++)
    {
        pad = StrParam(s:pad, c:padChar);
    }

    return StrParam(s:baseStr, s:pad);
}

// Pad left side of string with padChar
function int padStringL(int baseStr, int padChar, int len)
{
    int baseStrLen = StrLen(baseStr);
    int pad = "";
    int padLen; int i;

    if (baseStrLen >= len)
    {
        return baseStr;
    }
    
    padChar = GetChar(padChar, 0);
    padLen = len - baseStrLen;

    for (i = 0; i < padLen; i++)
    {
        pad = StrParam(s:pad, c:padChar);
    }

    return StrParam(s:pad, s:baseStr);
}

// Insert <repl> into position <where>, overwriting what was there
function int changeString(int string, int repl, int where)
{
    int i; int j; int k;
    int ret = "";
    int len = StrLen(string);
    int rLen = StrLen(repl);

    if ((where + rLen < 0) || (where >= len))
    {
        return string;
    }
    
    for (i = 0; i < len; i++)
    {
        if (inRange(where, where+rLen, i))
        {
            ret = StrParam(s:ret, c:GetChar(repl, i-where));
        }
        else
        {
            ret = StrParam(s:ret, c:GetChar(string, i));
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

// Find substring in string, starting from position 0
function int strstr(int string, int substring)
{
    return strstr_o(string, substring, 0);
}

// Find substring in string, starting from position <offset>
function int strstr_o(int string, int substring, int offset)
{
    int len = StrLen(string);
    int fromLen = StrLen(substring);

    int i, j, c;
    int charsFound  = 0;
    int lastEnd     = -1;

    // the string to search for could not possibly be in the range given
    if (offset >= (len - fromLen)) { return -1; }

    for (i = offset; i < len; i++)
    {
        int chr     = GetChar(string, i);
        int fromChr = GetChar(substring, charsFound);

        if (chr == fromChr)
        {
            charsFound++;

            if (charsFound == fromLen)
            {
                return lastEnd+1;
            }
        }
        else
        {
            charsFound  = 0;
            lastEnd     = i;
        }
    } 

    return -1;
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



function int unusedTID(int start, int end)
{
    int ret = start - 1;
    int tidNum;

    if (start > end) { start ^= end; end ^= start; start ^= end; }  // good ol' XOR swap
    
    while (ret++ != end)
    {
        if (ThingCount(0, ret) == 0)
        {
            return ret;
        }
    }
    
    return -1;
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

function int giveHealth(int amount)
{
    return giveHealthFactor(amount, 1.0);
}

function int giveHealthFactor(int amount, int maxFactor)
{
    return giveHealthMax(amount, FixedMul(getMaxHealth(), maxFactor));
}

function int giveHealthMax(int amount, int maxHP)
{
    int newHP;

    int curHP = GetActorProperty(0, APROP_Health);

    if (maxHP == 0) { newHP = max(curHP, curHP+amount); }
    else
    {
        if (curHP > maxHP) { return 0; }
        newHP = middle(curHP, curHP+amount, maxHP);
    }

    SetActorProperty(0, APROP_Health, newHP);

    return newHP - curHP;
}

function int isDead(int tid)
{
    return GetActorProperty(tid, APROP_Health) <= 0;
}

function int isSinglePlayer(void)
{
    return GameType() == GAME_SINGLE_PLAYER;
}

function int isLMS(void)
{
    return GetCVar("lastmanstanding") || GetCVar("teamlms");
}

function int isCoop(void)
{
    int check1 = GameType() == GAME_NET_COOPERATIVE;
    int check2 = GetCVar("cooperative") || GetCVar("invasion") || GetCVar("survival");

    return check1 || check2;
}

function int isInvasion(void)
{
    return GetCVar("invasion");
}

function int isFreeForAll(void)
{
    if (GetCVar("terminator") || GetCVar("duel"))
    {
        return 1;
    }

    int check1 = GetCVar("deathmatch") || GetCVar("possession") || GetCVar("lastmanstanding");
    int check2 = check1 && !GetCVar("teamplay");

    return check2;
}

function int isTeamGame(void)
{
    int ret = (GetCVar("teamplay") || GetCVar("teamgame") || GetCVar("teamlms"));
    return ret;
}

// Spawn actor certain distance in front of self
function int spawnDistance(int item, int dist, int tid)
{
    int myX, myY, myZ, myAng, myPitch, spawnX, spawnY, spawnZ;

    myX = GetActorX(0); myY = GetActorY(0); myZ = GetActorZ(0);
    myAng = GetActorAngle(0); myPitch = GetActorPitch(0);

    spawnX = FixedMul(cos(myAng) * dist, cos(myPitch));
    spawnX += myX;
    spawnY = FixedMul(sin(myAng) * dist, cos(myPitch));
    spawnY += myY;
    spawnZ = myZ + (-sin(myPitch) * dist);

    return Spawn(item, spawnX, spawnY, spawnZ, tid, myAng >> 8);
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

function void GiveActorAmmo(int tid, int type, int amount)
{
    if (GetCVar("sv_doubleammo"))
    {
        int m = GetAmmoCapacity(type);
        int expected = min(m, CheckActorInventory(tid, type) + amount);

        GiveActorInventory(tid, type, amount);
        TakeActorInventory(tid, type, CheckActorInventory(tid, type) - expected);
    }
    else
    {  
        GiveActorInventory(tid, type, amount);
    }
}

function int cond(int test, int trueRet, int falseRet)
{
    if (test) { return trueRet; }
    return falseRet;
}

function int condTrue(int test, int trueRet)
{
    if (test) { return trueRet; }
    return test;
}

function int condFalse(int test, int falseRet)
{
    if (test) { return test; }
    return falseRet;
}


function int onGround(int tid)
{
    return (GetActorZ(tid) - GetActorFloorZ(tid)) == 0;
}

function int ThingCounts(int start, int end)
{
    int i, ret = 0;

    if (start > end) { start ^= end; end ^= start; start ^= end; }
    for (i = start; i < end; i++) { ret += ThingCount(0, i); }

    return ret;
}

function int PlaceOnFloor(int tid)
{
    if (ThingCount(0, tid) != 1) { return 1; }
    
    SetActorPosition(tid, GetActorX(tid), GetActorY(tid), GetActorFloorZ(tid), 0);
    return 0;
}

#define DIR_E  1
#define DIR_NE 2
#define DIR_N  3
#define DIR_NW 4
#define DIR_W  5
#define DIR_SW 6
#define DIR_S  7
#define DIR_SE 8

function int getDirection(void)
{
    int sideMove = keyDown(BT_MOVERIGHT) - keyDown(BT_MOVELEFT);
    int forwMove = keyDown(BT_FORWARD) - keyDown(BT_BACK);

    if (sideMove || forwMove)
    {
        switch (sideMove)
        {
          case -1: 
            switch (forwMove)
            {
                case -1: return DIR_SW;
                case  0: return DIR_W;
                case  1: return DIR_NW;
            }
            break;

          case 0: 
            switch (forwMove)
            {
                case -1: return DIR_S;
                case  1: return DIR_N;
            }
            break;

          case 1: 
            switch (forwMove)
            {
                case -1: return DIR_SE;
                case  0: return DIR_E;
                case  1: return DIR_NE;
            }
            break;
        }
    }

    return 0;
}

function int isInvulnerable(void)
{
    int check1 = GetActorProperty(0, APROP_Invulnerable);
    int check2 = CheckInventory("PowerInvulnerable");

    return check1 || check2;
}

function int defaultTID(int def)
{
    return _defaulttid(def, 0);
}

function int _defaulttid(int def, int alwaysPropagate)
{
    if (ClassifyActor(0) & ACTOR_WORLD) { return 0; }

    int tid = ActivatorTID();

    if (tid == 0 || ThingCount(0, tid) > 1)
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

function int roundZero(int toround)
{
    int i = toround % 1.0;
    return ftoi(toround - i);
}

function int roundAway(int toround)
{
    int i = toround % 1.0;

    if (i == 0) { return ftoi(toround); }
    return ftoi(toround + (1.0 - i));
}

function int round(int toround)
{
    return ftoi(toround + 0.5);
}

function int ceil(int toround)
{
    return ftoi(toround + (1.0-1));
}

function int intFloat(int toround)
{
    return itof(ftoi(toround));
}

function int distance(int x1, int y1, int z1, int x2, int y2, int z2)
{
    return magnitudeThree_f(x2-x1, y2-y1, z2-z1);
}

function int distance_tid(int tid1, int tid2)
{
    int x1 = GetActorX(tid1);
    int y1 = GetActorY(tid1);
    int z1 = GetActorZ(tid1);

    int x2 = GetActorX(tid2);
    int y2 = GetActorY(tid2);
    int z2 = GetActorZ(tid2);

    return magnitudeThree_f(x2-x1, y2-y1, z2-z1);
}

function void printDebugInfo(void)
{
    int classify    = ClassifyActor(0);
    int dead        = classify & ACTOR_DEAD;
    int player      = classify & ACTOR_PLAYER;
    int pln         = PlayerNumber();
    int tid         = ActivatorTID();

    Log(s:"\n\n -- DEBUG INFO -- ");

    Log(s:"Name is ", n:0);
    Log(s:"Executed on tic ", d:Timer(), s:" on map ", d:GetLevelInfo(LEVELINFO_LEVELNUM));
    Log(s:"Has TID of ", d:tid, s:" - sharing with ", d:ThingCount(0, tid) - 1, s:" actors");
    Log(s:"Position: (", f:GetActorX(0), s:", ", f:GetActorY(0), s:", ", f:GetActorZ(0), s:")");

    if (classify & (ACTOR_PLAYER | ACTOR_MONSTER))
    {
        Log(s:"Script activator has ", d:GetActorProperty(0, APROP_Health), s:"/", d:getMaxHealth(), s:" HP");
    }

    if (player)
    {
        Log(s:"Is player ", d:pln, s:" with class number ", d:PlayerClass(pln));
    }

    Log(s:" -- END DEBUG -- \n\n");
}


function int PlayerTeamCount(int teamNo)
{
    int i, ret;
    for (i = 0; i < PLAYERMAX; i++)
    {
        if (GetPlayerInfo(i, PLAYERINFO_TEAM) == teamNo) { ret++; }
    }
    return ret;
}

function int lower(int chr)
{
    if (chr > 64 && chr < 91) { return chr+32; }
    return chr;
}

function int upper(int chr)
{
    if (chr > 90 && chr < 123) { return chr-32; }
    return chr;
}

function int strLower(int string)
{
    int ret = "";
    int len = StrLen(string);
    int i;

    for (i = 0; i < len; i++)
    {
        ret = StrParam(s:ret, c:lower(GetChar(string, i)));
    }

    return ret;
}

function int strUpper(int string)
{
    int ret = "";
    int len = StrLen(string);
    int i;

    for (i = 0; i < len; i++)
    {
        ret = StrParam(s:ret, c:upper(GetChar(string, i)));
    }

    return ret;
}

function int AddActorProperty(int tid, int prop, int amount)
{
    int newAmount = GetActorProperty(tid, prop) + amount;
    SetActorProperty(tid, prop, newAmount);
    return newAmount;
}

function int ClientCount(void)
{
    int ret, i;

    for (i = 0; i < PLAYERMAX; i++)
    {
        if (PlayerInGame(i) || PlayerIsSpectator(i)) { ret++; }
    }

    return ret;
}

function int HasRoom(int actorname, int x, int y, int z)
{
    int tid = unusedTID(40000, 50000);
    int ret = Spawn(actorname, x, y, z, tid);

    if (ret >= 1) { Thing_Remove(tid); }

    return ret;
}

function int RealPlayerCount(void)
{
    int ret, i;

    for (i = 0; i < PLAYERMAX; i++)
    {
        if (PlayerInGame(i) && !PlayerIsBot(i)) { ret++; }
    }

    return ret;
}

function int quadSlope(int orgX, int orgY, int pntX, int pntY, int floatY)
{
    int dist = abs(pntX - orgX);
    int height = pntY - orgY;
    int negative = 0;

    if (height == 0) { return 0; }

    if (height < 0)
    {
        negative = -1;
        height = -height;
    }
    
    int slope = cond(floatY, FixedSqrt(height), FixedSqrt(itof(height)));

    slope = (slope / dist);

    slope = FixedMul(slope, slope);

    if (negative) { return -slope; }
    return slope;
}

function int actorVelMagnitude(int tid)
{
    return magnitudeThree_f(GetActorVelX(tid), GetActorVelY(tid), GetActorVelZ(tid));
}

function int isAmmo(int name)
{
    return GetAmmoCapacity(name) > 0;
}

function int intcmp(int x, int y)
{
    if (x < y) { return -1; }
    if (x > y) { return  1; }
    return 0;
}


function int RaiseAmmoCapacity(int ammoname, int newcapacity, int raiseammo)
{
    int ammo = CheckInventory(ammoname);
    int capacity = GetAmmoCapacity(ammoname);

    if (capacity < newcapacity)
    {
        SetAmmoCapacity(ammoname, newcapacity);
        capacity = newcapacity;
    }

    if ((ammo < capacity) && raiseammo)
    {
        GiveAmmo(ammoname, capacity - ammo);
    }
    
    return CheckInventory(ammo);
}

// Shortest distance between two angles
function int angleDifference(int ang1, int ang2)
{
    ang1 = mod(ang1, 1.0);
    ang2 = mod(ang2, 1.0);

    int angLow  = min(ang1, ang2);
    int angHigh = max(ang1, ang2);

    int angDiff = angHigh - angLow;
    if (angDiff > 0.5) { angDiff = 1.0 - angDiff; }

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
function int stringBlank(int string)
{
    return (string == CommonFuncs_ZeroString) || (StrLen(string) == 0);
}
