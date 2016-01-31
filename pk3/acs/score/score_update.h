function void Air_UpdateZHeight(void)
{
    int pln = PlayerNumber();

    int ground = onGround(0);

    if (ground)
    {
        Air_FloorHeight[pln]    = GetActorZ(0);
        Air_JumpHeight[pln]     = 0;
    }
    else
    {
        int grav    = GetActorProperty(0, APROP_Gravity);
        int jumpVel = GetActorVelZ(0) + (grav / 2);

        int curMaxHeight = GetActorZ(0) + HeightFromJumpZ(jumpVel, grav);

        int adjustedMax  = min(curMaxHeight, GetActorCeilingZ(0)) - Air_FloorHeight[pln];

        Air_JumpHeight[pln] = max(Air_JumpHeight[pln], adjustedMax);
    }
}
