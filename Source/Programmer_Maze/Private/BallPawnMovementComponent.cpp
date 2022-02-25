// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "BallPawnMovementComponent.h"
#include "BallPawn.h"

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

        if (Hit.IsValidBlockingHit())
        {
            ABallPawn* actor=Cast<ABallPawn>(GetOwner());
            if (actor) {
                actor->signalCompletation();
            }
        }
    }
}