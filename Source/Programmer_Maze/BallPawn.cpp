// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "BallPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MazeMainGameMode.h"

// Sets default values
ABallPawn::ABallPawn():APawn(),InPanGesture(false)
{
 	//setup the stuffs
	//by default this pawn should receive no input
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DisableInput(NULL);
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
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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
	EnableInput(NULL);
}

void ABallPawn::stopProcessingInput()
{
	DisableInput(NULL);
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
	//This actor use a special variant of input handling so some care need to be done
	//First of the foremost, there are a pan control for this character
	//Second the right click should make the view show again
	//mouse position
	PlayerInputComponent->BindVectorAxis(EKeys::Mouse2D, this, &ABallPawn::MouseXYAvis);
	//Left mouse buttons
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Pressed, this, &ABallPawn::OnLMBDown);
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, EInputEvent::IE_Released, this, &ABallPawn::OnLMBUp);
	//Right mouse buttons
	PlayerInputComponent->BindKey(EKeys::RightMouseButton, EInputEvent::IE_Pressed, this, &ABallPawn::OnRMBDown);
}

UPawnMovementComponent* ABallPawn::GetMovementComponent() const
{
	return movementComponent;
}

void ABallPawn::MouseXYAvis(FVector value)
{
	if (!InPanGesture) {
		return;
	}
}

void ABallPawn::OnLMBDown()
{
	InPanGesture = true;
}

void ABallPawn::OnLMBUp()
{
	InPanGesture = false;
}

void ABallPawn::OnRMBDown()
{
	//just show the IDE back
	AMazeMainGameMode* gameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode) {
		this->stopProcessingInput();
		gameMode->showIDE();
	}
}

