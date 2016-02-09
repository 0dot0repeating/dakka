# dakka\_cl_jerkyrecoil

**Related CVars:** [dakka\_cl\_recoil](dakka_cl_recoil.md)

**Related files:** `/pk3/acs/dakka_recoil.h`

**CVar type:** bool

**Default value:** false

## Description

`dakka_cl_jerkyrecoil` is a clientside [CVar](../cvars.md) that, when enabled,
causes the lowering part of the recoil animation to jerk back downwards rather
than smoothly drop down. It's a holdover from Samsara, when this CVar was inverted
and called samsara_cl_sinerecoil instead. I don't think anyone understood what I
meant by that.

Here, have a comparison image:

![recoil comparison](recoilimage.png "graaaaphs")
