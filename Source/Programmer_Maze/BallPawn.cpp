// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "BallPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABallPawn::ABallPawn()
{
 	//setup the stuffs
	//the root is the sphere component that will react to the physic
	const FVector size = FVector(1.0f);
	const float radius = 50.5f;
	USphereComponent* spherePhysic = CreateDefaultSubobject<USphereComponent>(TEXT("RootPhysic"));
	RootComponent = spherePhysic;
	spherePhysic->InitSphereRadius(radius);
	spherePhysic->SetCollisionProfileName(TEXT("Pawn"));
	//then create the UI here
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMaterial(TEXT("/Game/UI/Actors/BallMat.BallMat"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetWorldScale3D(size);
	}
	if (SphereMaterial.Succeeded()) {
		SphereVisual->SetMaterial(0, SphereMaterial.Object);
	}
	//then our camera
	UCameraComponent* camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->bUsePawnControlRotation = false;
	camera->SetRelativeLocation(FVector(0.f, 0.f, 430.f));
	camera->SetRelativeRotation(FRotator(-90.000000, 0, 0));
	camera->SetupAttachment(RootComponent);
	//finally create a movement component and register this to the the component
	movementComponent = CreateDefaultSubobject<UBallPawnMovementComponent>(TEXT("MovementComponent"));
	movementComponent->UpdatedComponent = RootComponent;
}

void ABallPawn::startProcessingInput()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABallPawn::stopProcessingInput()
{
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UPawnMovementComponent* ABallPawn::GetMovementComponent() const
{
	return movementComponent;
}

