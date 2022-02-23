// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BallPawnMovementComponent.generated.h"

/**
 * A simple pawn movement component that allow the ball move in a way which physics aware
 * Reference: https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/Components/
 * Note: sliding is not used as that might cause code behavior varies
 */
UCLASS()
class PROGRAMMER_MAZE_API UBallPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
