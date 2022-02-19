// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "BallPawnMovementComponent.h"

//refer Unreal Engine tutorial for this
void UBallPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Make sure that everything is still valid, and that we are allowed to move.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    //emulate the gravity here
    {
        FHitResult ignored;
        SafeMoveUpdatedComponent(FVector(0,0,-1), UpdatedComponent->GetComponentRotation(), true, ignored);
    }

    // Get (and then clear) the movement vector that 
    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.0f;
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // If we bumped into something, try to slide along it
        if (Hit.IsValidBlockingHit())
        {
            //TODO: signal the actor that the actor is hit to something
            //this is required to signal the end of the execution of function as all
            //movement functions are based on this event
        }
    }
}