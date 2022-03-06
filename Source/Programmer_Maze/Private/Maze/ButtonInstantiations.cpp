//// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#include "Maze/ButtonInstantiations.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Maze/MazeMainGameMode.h"
#include "GeneralUtilities.h"

AGoalPawn::AGoalPawn() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CylinderMaterial(TEXT("/Game/UI/Actors/GoalButtonMat.GoalButtonMat"));
	if (CylinderMaterial.Succeeded()) {
		root->SetMaterial(0, CylinderMaterial.Object);
	}
	nextLevelID = TEXT("");
	currentLevelName = TEXT("Unknown Level");
}

void AGoalPawn::BeginPlay() {
	Super::BeginPlay();
	AMazeMainGameMode* refGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (refGameMode&&refGameMode->IDELevelNameBlock) {
		refGameMode->IDELevelNameBlock->SetText(FText::FromName(currentLevelName));
	}
	else {
		refGameMode->currentLevelName = currentLevelName;
	}
}

void AGoalPawn::onBallHit() {
	AMazeMainGameMode* refGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (refGameMode) {
		refGameMode->onLevelCompleted(this);
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

AAnchorPawn::AAnchorPawn() :ACheckpointPawn(), groupID(-1) {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("/Game/UI/Actors/PinkWall.PinkWall"));
	if (CubeMaterial.Succeeded()) {
		baseMaterial = CubeMaterial.Object;
	}
}

void AAnchorPawn::BeginPlay()
{
	Super::BeginPlay();
	if (baseMaterial) {
		root->SetMaterial(0, baseMaterial);
	}
	//register the material to the map
	if(groupID>=0){
		AMazeMainGameMode* ptrGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
		if (ptrGameMode->groupMaterialMap.Contains(groupID)) {
			//ensure it is same
			if (ptrGameMode->groupMaterialMap[groupID] != baseMaterial) {
				GeneralUtilities::Log(this, FString::Printf(
					TEXT("Detected inconsistency on the materials used inside this map (groupID=%d), this might results in a glitch in display")
					,groupID
				));
			}
		}
		else {
			if (baseMaterial) {
				//register it otherwise
				ptrGameMode->groupMaterialMap.Add(groupID, baseMaterial);
			}
		}
	}
}

void AAnchorPawn::onBallHit()
{
	//ensure the checkpoint behavior works
	Super::onBallHit();
	//try to broadcast the event if the group id is valid (more than or equal with 0)
	if (groupID >= 0) {
		AMazeMainGameMode* ptrGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
		if (ptrGameMode) {
			ptrGameMode->characterStatusBroadcast.Broadcast(groupID);
		}
	}
}


