#define DEC_GETLESSEFFECTS      0



script DAKKA_DECORATE_CLIENT (int mode, int a1, int a2)
{
    int pln = PlayerNumber();
    int ret = 0;

    // At least I'm using constants this time.
    switch (mode)
    {
      case DEC_GETLESSEFFECTS:
        ret = GetCVar("dakka_cl_lesseffects");
        break;
    }

    SetResultValue(ret);
}
