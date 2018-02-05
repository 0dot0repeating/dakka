// quadratic:
//
//  m = peak of recoil
//  t = time to reach peak
//  f(x) = a(x-h)**2 + k
//  g(x) = a * x**2
//
//  so we want f(0) = 0, and f(t) = m
//
//  or in other words, since g(x) is f(x) without the coordinate offsets,
//  we want g(0) = 0, g(t) = -m   (and also g(-t) = -m)
//
//  g(t) = a * t**2 = -m
//  a = -m / t**2
//
//  g(0) = (-m / t**2) * (0 ** 2) = 0
//
//  and then the offsets:
//
//  h = t
//  k = m
//
//  f(0) = a(0-h)**2 + k
//  f(0) = (-m / t**2) * (0-t)**2 + m
//       = (-m * t**2) / (t**2) + m
//       = -m + m
//       = 0
//
//  f(t) = a(t-h)**2 + k
//       = (-m / t**2) * (t-t)**2 + m
//       = (-m / t**2) * 0 + m
//       = m


#define RECOILMETHODS   3

#define RMETHOD_LINEAR      0
#define RMETHOD_QUADRATIC   1
#define RMETHOD_SINUSOIDAL  2

script "Dakka_Recoil" (int degrees_raw, int ticsup, int ticsdown) clientside
{
    int pln = PlayerNumber();
    if (IsZandronum && (pln != ConsolePlayerNumber())) { terminate; }

    if (degrees_raw == 0) { terminate; }
    if (GetCVar("sv_nofreelook") || !GetCVar("freelook")) { terminate; }


    int upFactor    = itof(middle(0, GetUserCVar(pln, "dakka_cl_recoil"),     1000)) / 100;
    int downFactor  = itof(middle(0, GetUserCVar(pln, "dakka_cl_recoildown"), 100))  / 100;
    int interpolate = !GetUserCVar(pln, "dakka_cl_norecoilinterp");

    if (upFactor == 0) { terminate; }

    int upMethod   = RMETHOD_QUADRATIC;
    int downMethod = middle(0, GetUserCVar(pln, "dakka_cl_recoilmodedown"), RECOILMETHODS-1);

    // scale up recoil reset time so 100%+ recoils are more bearable
    ticsdown = FixedMul(ticsdown, FixedSqrt(upFactor));

    // we multiply the degrees by 10 for precision purposes; Dakka_RecoilPitch converts it to turns
    int degrees     = cond(degrees_raw < 0, itof(-degrees_raw) / 10, itof(degrees_raw) * 10);
    int degreesUp   = -FixedMul(degrees, upFactor);   // negative pitches up
    int degreesDown =  FixedMul(degrees, FixedMul(upFactor, downFactor));

    int prevPitch, newPitch = 0, pitchDiff;
    int totalUp = 0, totalDown = 0;
    int i;

    int linear_m;
    int quad_a, quad_h, quad_k;
    int cos_a,  cos_f;

    if (ticsUp > 1)
    {
        linear_m = degreesUp / ticsup;              // slope

        quad_a = -degreesUp / (ticsup * ticsup);    // fixed; degreesUp is fixed, ticsup is int
        quad_h = ticsup;                            // int;   since the x in f(x) is int, this can be int too
        quad_k = degreesUp;                         // fixed; since a(x-h)**2 is fixed, this has to be too

        cos_a  = degreesUp / 2;                     // amplitude
        cos_f  = ticsup * 2;                        // frequency

        for (i = 1; i <= ticsup; i++)
        {
            prevPitch = newPitch;

            switch (upMethod)
            {
                case RMETHOD_LINEAR:        newPitch  = linear_m * i; break;
                case RMETHOD_QUADRATIC:     newPitch  = quad_a * pow(i - quad_h, 2) + quad_k; break;
                case RMETHOD_SINUSOIDAL:    newPitch  = FixedMul(-cos_a, cos(itof(i) / cos_f)) + cos_a; break;
            }

            pitchDiff = newPitch - prevPitch;
            totalUp  += pitchDiff;

            Dakka_RecoilPitch(pitchDiff, interpolate);
            Delay(1);
        }
    }
    else
    {
        totalUp = degreesUp;
        Dakka_RecoilPitch(totalUp, interpolate && (ticsup == 1));
        if (ticsup == 1) { Delay(1); }
    }


    newPitch = 0;

    if (downFactor > 0)
    {
        if (ticsDown > 0)
        {
            linear_m = degreesDown / ticsdown;

            quad_a = -degreesDown / (ticsdown * ticsdown);
            quad_h = ticsdown;
            quad_k = degreesDown;

            cos_a  = degreesDown / 2;
            cos_f  = ticsdown * 2;

            for (i = 1; i <= ticsdown; i++)
            {
                prevPitch  = newPitch;

                switch (downMethod)
                {
                    case RMETHOD_LINEAR:        newPitch  = linear_m * i; break;
                    case RMETHOD_QUADRATIC:     newPitch  = quad_a * pow(i - quad_h, 2) + quad_k; break;
                    case RMETHOD_SINUSOIDAL:    newPitch  = FixedMul(-cos_a, cos(itof(i) / cos_f)) + cos_a; break;
                }

                pitchDiff  = newPitch - prevPitch;
                totalDown += pitchDiff;

                Dakka_RecoilPitch(pitchDiff, interpolate);
                Delay(1);
            }
        }
        else
        {
            totalDown = degreesDown;
            Dakka_RecoilPitch(totalDown, interpolate && (ticsdown == 1));
        }

        if (downFactor == 1.0)
        {
            Dakka_RecoilPitch(-(totalUp + totalDown), interpolate);
        }
    }
}


// convenience mainly
// uses 3600 instead of 360 since the recoil script multiplies degrees by 10
function void Dakka_RecoilPitch(int pitchDiff, int interpolate)
{
    int curPitch = GetActorPitch(0);
    ChangeActorPitch(0, curPitch + (pitchDiff / 3600), interpolate);
}