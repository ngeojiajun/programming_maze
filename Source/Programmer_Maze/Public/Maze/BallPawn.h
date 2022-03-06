// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Maze/BallPawnMovementComponent.h"
#include "Blocks/ScriptExecutionContext.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "BallPawn.generated.h"

class AMazeMainGameCode;

UCLASS()
class PROGRAMMER_MAZE_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();
	//used in the input handlers which the pawn should take over the input processing for panning
	void startProcessingInput();
	//used in the input handlers which the pawn should not take over the input processing for panning
	void stopProcessingInput();
	//reset the camera
	void resetCameraPosition();
	//reset the zoom
	void resetCameraZoom();
	//start the movement
	//this function is latern protocol compliant (but not returning anything)
	void startMovement(const FVector movement, FScriptExecutionContext& ctx);
	//terminate the movement
	//Not required by the latern protocol but needed to reset the view
	void stopMovement();
	void signalCompletation();
	//checkpoint
	//get the initial position of the pawn when there are no checkpoint found
	FVector getInitialLocation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when being removed from a level
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
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
	UPROPERTY()
	UCameraComponent* camera;
	UPROPERTY()
	UStaticMeshComponent* SphereVisual;
	UPROPERTY()
	AMazeMainGameMode* refGameMode;
	UPROPERTY()
	UMaterialInterface* defaultMaterial;
	UPROPERTY()
	UAudioComponent* audioSFXComponent;
	UPROPERTY()
	UAudioComponent* audioBGMComponent;
	//is the panning is in progress
	bool InPanGesture;
	bool Executing;
	bool isReceivingPanInput;
	//listener for the characterStatusBroadcast
	UFUNCTION()
	void onCharacterStatusChanged(int group);
	//handle the mouse coords
	void MouseXYAvis(FVector value);
	//mouse button handlers
	void OnLMBDown();
	void OnLMBUp();
	void OnRMBDown();
	void onWheelUp();
	void onWheelDown();
	void onEscapeDown();
	//scripting: the movement that the ball is possessing
	FVector currentEffectiveMovement;
	//original position for resetting it later
	FVector initialPosition;
};
