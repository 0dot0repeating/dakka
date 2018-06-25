script "Dakka_LessEffects" (int destroy) clientside
{
    int ret = GetUserCVar(ConsolePlayerNumber(), "dakka_cl_lesseffects");
    
    if (destroy && ret) { Thing_Remove(0); }
    SetResultValue(ret);
}
