// We need two pairs of angles, one for the current arcer so it faces the new
//  arcer, and one for the new arcer so we know what direction it should face.
//
// The arcer's new angle will be somewhere between the direction the current
//  arcer is facing, and the direction the new target is in. Where exactly is
//  controlled by ADATA_INT_FACINGWEIGHT.
//
// A weight of 0 means "face the same direction as the current arcer". In effect,
//  go straight forward.
//
// A weight of 1 means "face the target". In effect, bounce around like a fool.
//
// Note that you aren't limited to values between 0 and 1. If for some reason
//  you want the new arc to *overshoot* the direction to face the target, you
//  can - use a value greater than 1. If you want it to face further away from
//  the target (effectively curving inward), use a value less than 0.
// 

#define ANG_TARGET  0
#define ANG_NEW     1

#define ANG_ANGLE   0
#define ANG_PITCH   1

int Arc_NewAngles[2][2];

function void Arc_CalcNewAngle(int arcType, int arcerTID, int targetTID)
{
    int faceWeight = INT_ArcData[arcType][ADATA_INT_FACINGWEIGHT];
    int faceFirer  = INT_ArcData[arcType][ADATA_INT_FACINGFIRER];

    // Stealing the firer's direction instead of the current arcer's? Sure.
    if (faceFirer)
    {
        SetActivator(arcerTID, AAPTR_TARGET);
    }

    int arcerAngle = GetActorAngle(0);
    int arcerPitch = GetActorPitch(0);
    
    SetActivator(arcerTID);

    int dX = GetActorX(targetTID) - GetActorX(arcerTID);
    int dY = GetActorY(targetTID) - GetActorY(arcerTID);
    int dZ = GetActorZ(targetTID) - GetActorZ(arcerTID);

    // Remember trigonometry class!
    //
    // The angle of a 2D vector is the arc tangent defined by its X and Y values.
    //
    //      /|
    //     / |           Y/X = tan o
    //  Z /  | Y    atan Y/X =     o
    //   /   |
    //  /    |   And that's what VectorAngle is basically; the atan function.
    // o_____'   Bam, angles.
    //    X
    //
    // Also, a pitch looking up is negative, hence -dZ.

    int targetAngle = VectorAngle(dX, dY);
    int targetPitch = VectorAngle(magnitudeTwo_f(dX, dY), -dZ);
    
    // We aren't necessarily taking the shortest path from one angle to another
    //  right now. If we're going from arcer angle 0 to target angle 0.9, the
    //  below line suggests 0.9, not -0.1.
    //
    // Similarly, going from arcer angle 0.6 to target angle 0, the below line
    //  suggests -0.6 and not 0.4.

    int angleDiff = targetAngle - arcerAngle;

    // This corrects that.
    if (angleDiff < -0.5) { angleDiff += 1.0; }
    if (angleDiff >  0.5) { angleDiff -= 1.0; }


    // Pitches can only be between -0.25 and 0.25 anyway, so no big deal here.
    int pitchDiff = targetPitch - arcerPitch;


    // Get the in between...
    int middleAngle = arcerAngle + FixedMul(faceWeight, angleDiff);
    int middlePitch = arcerPitch + FixedMul(faceWeight, pitchDiff);


    // And assign shit!
    
    Arc_NewAngles[ANG_TARGET][ANG_ANGLE] = targetAngle;
    Arc_NewAngles[ANG_TARGET][ANG_PITCH] = targetPitch;
    
    Arc_NewAngles[ANG_NEW][ANG_ANGLE] = middleAngle;
    Arc_NewAngles[ANG_NEW][ANG_PITCH] = middlePitch;
}

