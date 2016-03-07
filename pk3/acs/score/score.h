#include "score_defs.h"
#include "score_update.h"
#include "score_display.h"
#include "score_mults.h"
#include "score_onkill.h"
#include "score_weapons.h"
#include "score_reward.h"
#include "score_levelstart.h"



// Debug functions

script "Score_GiveRewards" (int rewardCount)
{
    Score_ModScore(PlayerNumber(), MapStart_FullHealPoints * max(rewardCount, 0));
}

script "Score_GiveLives" (int lifeCount)
{
    Score_ModExtraLives(PlayerNumber(), lifeCount);
}

script "Score_GiveRegen" (int regenCount)
{
    Score_ModRegenTimer(PlayerNumber(), regenCount);
}
