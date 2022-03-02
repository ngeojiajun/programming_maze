// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#include "Maze/ConditionalWall.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Maze/MazeMainGameMode.h"

AConditionalWall::AConditionalWall() :APawn(),groupID(0) {
	//setup
	root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = root;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("/Game/UI/Actors/PinkWall.PinkWall"));
	if (CubeVisualAsset.Succeeded()) {
		root->SetStaticMesh(CubeVisualAsset.Object);
	}
	if (CubeMaterial.Succeeded()) {
		baseMaterial = CubeMaterial.Object;
	}
	root->SetRelativeScale3D(FVector(1, 1, 1));
	root->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	//explicitly disable the control possession
	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void AConditionalWall::BeginPlay()
{
	//when the level starts setup the pointer and materials
	if (baseMaterial && !matPointer) {
		matPointer = UMaterialInstanceDynamic::Create(baseMaterial, this);
		matPointer->SetScalarParameterValue(FName("Transparency"),1.0f);
	}
	if (matPointer) {
		root->SetMaterial(0, matPointer);
	}
	//then try to listen to the characterStatusBroadcast whenever possible
	AMazeMainGameMode* ptrGameMode= Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (ptrGameMode) {
		ptrGameMode->characterStatusBroadcast.AddDynamic(this, &AConditionalWall::onCharacterStatusChanged);
	}
}

void AConditionalWall::onCharacterStatusChanged(int group)
{
	//set the block as preneteratable if the id match or else otherwise
	if (this->groupID == group) {
		root->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
		matPointer->SetScalarParameterValue(FName("Transparency"), 0.3f);
	}
	else {
		root->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		matPointer->SetScalarParameterValue(FName("Transparency"), 1.0f);
	}
}
