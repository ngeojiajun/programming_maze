//// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#include "ButtonInstantiations.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

void AExitButtonPawn::onBallHit() {
	//quit the game
	UKismetSystemLibrary::QuitGame(this, NULL, EQuitPreference::Quit, false);
}

ACheckpointPawn::ACheckpointPawn() :AButtonPawn() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CylinderMaterial(TEXT("/Game/UI/Actors/CheckpointButtonMat.CheckpointButtonMat"));
	if (CylinderMaterial.Succeeded()) {
		root->SetMaterial(0, CylinderMaterial.Object);
	}
	this->Tags.Add(FName("Checkpoint"));
}

void ACheckpointPawn::onBallHit()
{
	//TODO update the game mode that the checkpoint was hit
}
