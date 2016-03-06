#define DEC_ROTATESOUND     0
#define DEC_SCRAP_XYVEL     1
#define DEC_SCRAP_ZVEL      2

script DAKKA_DECORATE (int mode, int a1, int a2)
{
    int pln = PlayerNumber();

    int ret;

    switch (mode)
    {
      case DEC_SCRAP_XYVEL:
      case DEC_SCRAP_ZVEL:
        int x = GetActorVelX(0);
        int y = GetActorVelY(0);
        int z = GetActorVelZ(0);
        int mag = magnitudeThree_f(x, y, z);

        if (mag == 0)
        {
            if (mode == DEC_SCRAP_ZVEL) { ret = 0.0; }
            else { ret = 1.0; } 
        }
        else
        {
            if (mode == DEC_SCRAP_ZVEL) { ret = z; }
            else { ret = magnitudeTwo_f(x, y); }

            ret = FixedDiv(ret, mag);
        }

        break;
    }

    SetResultValue(ret);
}
