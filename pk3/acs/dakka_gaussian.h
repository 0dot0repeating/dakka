script "Dakka_Gaussian" (int count, int high, int low)
{
    int ret = 0;
    int i;
    if (!low) { low = -high; }

    for (i = 0; i < count; i++) { ret += random(low, high); }
    SetResultValue(ret);
}

