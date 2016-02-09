# dakka\_scrapfactor

**Related CVars:** none

**Related files:** `/pk3/acs/dakka_scrap.h`

**CVar type:** int

**Default value:** 100

## Description

`dakka_scrapfactor` is a [CVar](../cvars.md) that controls the conversion
ratio of spent ammo to scrap, as a percentage of normal scrap conversion -
eg. 100 is 100%, 500 is 500%, etc. The CVar's effect is capped between 0%
and 10000%.

By default, ammo is converted to scrap at the ratios (format shots:scrap):

* Pistol rounds: 2:1
* Shells and gyro shells: 1:4
* Chaingun/minigun rounds: 10:7
* Flamer fuel: 3:8, but scrap is given in batches of 16
* Cells: 1:1, but scrap is given in batches of 20
