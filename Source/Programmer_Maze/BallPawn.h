// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallPawnMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "BallPawn.generated.h"

UCLASS()
class PROGRAMMER_MAZE_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();
	//used in the input handlers which the pawn should take over the input processing
	void startProcessingInput();
	//used in the input handlers which the pawn should not take over the input processing
	void stopProcessingInput();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//return the movement component that this pawn is using
	virtual UPawnMovementComponent* GetMovementComponent() const override;
private:
	UPROPERTY()
	UBallPawnMovementComponent* movementComponent;
};
