//// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#include "Maze/ButtonInstantiations.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Maze/MazeMainGameMode.h"

AGoalPawn::AGoalPawn() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CylinderMaterial(TEXT("/Game/UI/Actors/GoalButtonMat.GoalButtonMat"));
	if (CylinderMaterial.Succeeded()) {
		root->SetMaterial(0, CylinderMaterial.Object);
	}
}

void AGoalPawn::onBallHit() {
	AMazeMainGameMode* refGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (refGameMode) {
		refGameMode->onLevelCompleted();
	}
}

ACheckpointPawn::ACheckpointPawn() :AButtonPawn(),checkpointId(0) {
	this->Tags.Add(FName("Checkpoint"));
}

void ACheckpointPawn::onBallHit()
{
	AMazeMainGameMode* refGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (refGameMode) {
		refGameMode->onCheckpointHit(checkpointId);
	}
}

AAnchorPawn::AAnchorPawn() :AButtonPawn(), groupID(-1) {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("/Game/UI/Actors/PinkWall.PinkWall"));
	if (CubeMaterial.Succeeded()) {
		baseMaterial = CubeMaterial.Object;
	}
}

void AAnchorPawn::BeginPlay()
{
	if (baseMaterial) {
		root->SetMaterial(0, baseMaterial);
	}
}

void AAnchorPawn::onBallHit()
{
	//try to broadcast the event if the group id is valid (more than or equal with 0)
	AMazeMainGameMode* ptrGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (ptrGameMode) {
		ptrGameMode->characterStatusBroadcast.Broadcast(groupID);
	}
}


