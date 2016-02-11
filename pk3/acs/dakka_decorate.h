// This is used to run weapon sounds on different slots automagically.
int RotatingSoundIndex[PLAYERMAX];

#define DEC_ROTATESOUND     0
#define DEC_SCRAP_XYVEL     1
#define DEC_SCRAP_ZVEL      2

#define DEC_RL_ALTFIRE      7

script DAKKA_DECORATE (int mode, int a1, int a2)
{
    int pln = PlayerNumber();

    int ret;

    switch (mode)
    {
      case DEC_ROTATESOUND:
        int leftBound  = cond(a1 == 0, 4, a1);
        int rightBound = cond(a2 == 0, 7, a2);
        int indexRange = (rightBound - leftBound) + 1;

        ret = (RotatingSoundIndex[pln]++ % indexRange) + leftBound;
        break;

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

      case DEC_RL_ALTFIRE:
        ret = (CheckInventory("DakkaRockets") - CheckInventory("DakkaRocketsLoaded")) > 0;
        ret |= CheckInventory("DakkaInfiniteAmmo");
        break;
    }

    SetResultValue(ret);
}
