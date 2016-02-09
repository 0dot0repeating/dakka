# dakka\_startmode\_ammo

**Related CVars:** [dakka_startmode_weapons.md](dakka\_startmode\_weapons)

**Related files:** `/pk3/acs/dakka_levelstart.h`, `/pk3/acs/pickup_items_weapons.h`, `/pk3/acs/pickup/pickup_items_ammo.h`

**CVar type:** int

**Default value:** 1

## Description

`dakka_startmode_ammo` is a [../cvars.md](CVar) that determines the amount of ammo
you spawn with for each weapon in your inventory. The CVar `dakka_startmode_weapons`
is processed immediately before this CVar is. There are five modes for this CVar.

* **0:** Do nothing. You keep all the ammo you have.

* **1:** Every weapon in your inventory has its ammo reset to a default ammo value.
    Ammo types for weapons you don't have are stripped from you entirely.

* **2:** Same behaviour as mode 1, but the ammo is reset to its maximum capacity
    rather than a default ammo count.

* **3:** Same behaviour as mode 1, but only weapons that you would pistol start with
    (criteria below) have their ammo replenished to default values. All other weapons
    have their ammo stripped.

* **4:** Same behaviour as mode 3, but ammo is reset to its maximum capacity rather
    than a default ammo count.

A weapon is considered to be a pistol start weapon if it's listed in `Start_Weapons[]`
(see `/pk3/acs/pickup_items_weapons.h`) and its power rating in `Start_WeaponPowers[]` is
less than 3. Only weapons for your current class according to the pickup system
(see the `Pickup_ClassNumber()` function) are checked.

Default ammo values are located in `PKP_DefaultAmmoCount[]`, in `/pk3/acs/pickup_items_ammo.h`.
