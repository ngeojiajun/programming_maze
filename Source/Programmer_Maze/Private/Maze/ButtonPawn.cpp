// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "Maze/ButtonPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Maze/BallPawn.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AButtonPawn::AButtonPawn()
{
	root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	RootComponent = root;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderVisualAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CylinderMaterial(TEXT("/Game/UI/Actors/ButtonMat.ButtonMat"));
	if (CylinderVisualAsset.Succeeded()) {
		root->SetStaticMesh(CylinderVisualAsset.Object);
	}
	if (CylinderMaterial.Succeeded()) {
		root->SetMaterial(0, CylinderMaterial.Object);
	}
	root->SetRelativeScale3D(FVector(1, 1, 0.15));
	root->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	root->OnComponentBeginOverlap.AddDynamic(this, &AButtonPawn::OnBallEnter);
	//try to find our SFX and create the component if we have one
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXAsset(TEXT("/Game/Sounds/click_zapsplat.click_zapsplat"));
	if (SFXAsset.Succeeded()) {
		audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
		audioComponent->SetSound(SFXAsset.Object);
		audioComponent->SetupAttachment(RootComponent);
	}
	//explicitly disable the control possession
	AutoPossessAI = EAutoPossessAI::Disabled;
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

// Called when the game starts or when spawned
void AButtonPawn::BeginPlay()
{
	Super::BeginPlay();
	if (audioComponent) {
		audioComponent->Stop();
	}
}

// Called every frame
void AButtonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AButtonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AButtonPawn::onBallHit()
{
}

void AButtonPawn::OnBallEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass() == ABallPawn::StaticClass() && OtherComp->GetClass() != UStaticMeshComponent::StaticClass()) {
		//play the audio if here
		//do it here so every instance of this class can enjoy the benigit regardless of how it is implemented
		if (audioComponent) {
			audioComponent->Play(0);
		}
		onBallHit();
	}
}
