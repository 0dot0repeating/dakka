// This is used to run weapon sounds on different slots automagically.
int RotatingSoundIndex[PLAYERMAX];

script "Dakka_RotateSound" (int left, int right)
{
    int pln = PlayerNumber();

    int leftBound  = cond(left  == 0, 4, left);
    int rightBound = cond(right == 0, 7, right);
    int indexRange = (rightBound - leftBound) + 1;

    SetResultValue((RotatingSoundIndex[pln]++ % indexRange) + leftBound);
}
