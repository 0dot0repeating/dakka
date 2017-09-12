// I can kinda read this and I'd recommend you don't.

script "Dakka_Recoil" (int degrees, int ticsup, int ticsdown) clientside
{
    if (degrees == 0) { terminate; }

    int recoilFactor = percFloat(0, middle(0, GetCVar("dakka_cl_recoil"), 1000));
    if (recoilFactor <= 0 && ticsup >= 0) { terminate; }
    if (GetCVar("sv_nofreelook")) { terminate; }

    degrees = itof(degrees);
    if (degrees < 0) { degrees /= -100; }

    degrees = FixedMul(degrees, recoilFactor);

    int qCurve, oldPitch, newPitch, pitchDiff, i;
    int amplitude = degrees / 2;
    int doDownRecoil = 1;
    ticsup = abs(ticsup);

    if (ticsdown < 0)
    {
        if (GetCVar("freelook")) { doDownRecoil = 0; }
        else { ticsdown = -ticsdown; }
    }

    if (ticsup > 0)
    {
        qCurve = degrees / pow(ticsup, 2);  // this is a
        newPitch = qCurve * pow(ticsup, 2);

        for (i = 0; i < ticsup; i++)
        {
            oldPitch = newPitch;
            // y = a(x-h)**2 + k
            newPitch = qCurve * pow((i+1)-ticsup, 2);
            pitchDiff = (newPitch - oldPitch) / 360;

            SetActorPitch(0, GetActorPitch(0) + pitchDiff);
            Delay(1);
        }
    }
    else
    {
        SetActorPitch(0, GetActorPitch(0) - (degrees / 360));
    }

    int totalDown = 0;

    if (doDownRecoil)
    {
        if (ticsdown > 0)
        {
            newPitch = 0;

            if (GetCVar("dakka_cl_jerkyrecoil"))
            {
                qCurve = -degrees / pow(ticsdown, 2); // this is also a
                newPitch = qCurve * pow(ticsdown, 2);

                for (i = 0; i < ticsdown; i++)
                {
                    oldPitch = newPitch;
                    // y = a(x-h)**2 + k
                    newPitch = qCurve * pow((i+1)-ticsdown, 2);
                    pitchDiff = (newPitch - oldPitch) / 360;
                    totalDown += pitchDiff;

                    SetActorPitch(0, GetActorPitch(0) + pitchDiff);
                    Delay(1);
                }
            }
            else
            {
                // y = (a/2) * sin(x) + (a/2)

                for (i = 0; i <= ticsdown; i++)
                {
                    oldPitch = newPitch;
                    newPitch = -FixedMul(amplitude, cos(itof(i) / (ticsdown*2))) + amplitude;
                    pitchDiff = (newPitch - oldPitch) / 360;
                    totalDown += pitchDiff;

                    SetActorPitch(0, GetActorPitch(0) + pitchDiff);
                    Delay(1);
                }
            }

            int pitchLeft = (degrees / 360) - totalDown;
            SetActorPitch(0, GetActorPitch(0) + pitchLeft);
        }
        else
        {
            SetActorPitch(0, GetActorPitch(0) + (degrees / 360));
        }
    }
}