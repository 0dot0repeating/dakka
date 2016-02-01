// This simulates VISIBILITYPULSE.
script 493 (void)
{
    if (DISP_ScriptArgs[DPASS_DOCLEANUP])
    {
        TakeInventory("DakkaVisPulse", 0x7FFFFFFF);
        terminate;
    }

    int timerPulse = CheckInventory("DakkaVisPulse") % 48;
    GiveInventory("DakkaVisPulse", 1);

    if (timerPulse > 24) { timerPulse = 48 - timerPulse; }

    int newAlpha = 0.25 + ((1.0 / 24) * timerPulse);

    SetActorProperty(0, APROP_RenderStyle,  STYLE_Translucent);
    SetActorProperty(0, APROP_Alpha,        newAlpha);
}

