# dakka\_startmode\_health

**Related CVars:** [dakka\_startmode\_weapons](dakka_startmode_weapons.md), [dakka\_startmode\_ammo](dakka_startmode_ammo.md)

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/pickup_items.h`

**CVar type:** int

**Default value:** 1

## Description

`dakka_startmode_health` is a [CVar](../cvars.md) that determines how you start out
a map with regards to health and armor. There are currently five modes for this CVar.

* **0:** Do nothing. You keep all the ammo you have.

* **1:** Start with 100% health.

* **2:** Start with 100% health, and a green armor pickup.

* **3:** Start with 100% health, a soulsphere pickup, and a blue armor pickup.

* **4:** Start with 100% health, and a megasphere pickup.

The reason why I state this in terms of pickups is because that's exactly how it's
handled in the code. It doesn't give you the armor directly - it uses the pickup system
to give you armor and health pickups appropriately. For example, modes 3 and 4 are
equivalent for the Doomguy, but have a 100 HP difference for the Dakkaguy.

The pickup definitions are in `/pk3/acs/pickup_items.h`.
