# dakka\_keepscore

**Related CVars:** none

**Related files:** `/pk3/acs/score/score_levelstart.h`, `/pk3/acs/score/score_reward.h`

**CVar type:** int

**Default value:** 1

## Description

`dakka_keepscore` is a [CVar](../cvars.md) that controls how your score transfers between maps. It works as follows:

- **0:** Transfer nothing. Reset everything score-related between maps.
- **1:** Transfer the amount of points you earned last map, but reset score rewards and the progress to the next reward.
- **2:** Transfer everything except the progress to the next reward.
- **3:** Transfer everything.
