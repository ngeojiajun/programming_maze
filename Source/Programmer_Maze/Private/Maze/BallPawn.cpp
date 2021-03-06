// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "Maze/BallPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Maze/MazeMainGameMode.h"
#include "GeneralUtilities.h"

//the rate that the camera shall move per second
const float panRate = 2500.f;
const FVector cameraInitialPosition = FVector(0.f, 0.f, 430.f);

// Sets default values
ABallPawn::ABallPawn():APawn(),InPanGesture(false),Executing(false), isReceivingPanInput(false), resolutionFailed(false), groupID(-1)
{
 	//setup the stuffs
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	PrimaryActorTick.bCanEverTick = true;
	//no AI needed
	AutoPossessAI = EAutoPossessAI::Disabled;
	//the root is the sphere component that will react to the physic
	const FVector size = FVector(1.0f);
	const float radius = 50.5f;
	USphereComponent* spherePhysic = CreateDefaultSubobject<USphereComponent>(TEXT("RootPhysic"));
	RootComponent = spherePhysic;
	spherePhysic->InitSphereRadius(radius);
	spherePhysic->SetCollisionProfileName(TEXT("Pawn"));
	//then create the UI here
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMaterial(TEXT("/Game/UI/Actors/BallMat.BallMat"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetWorldScale3D(size);
	}
	if (SphereMaterial.Succeeded()) {
		defaultMaterial = SphereMaterial.Object;
		SphereVisual->SetMaterial(0, SphereMaterial.Object);
	}
	//then our camera
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->bUsePawnControlRotation = false;
	camera->SetRelativeLocation(cameraInitialPosition);
	camera->SetRelativeRotation(FRotator(-90.000000, 0, 0));
	camera->SetupAttachment(RootComponent);
	//then create a movement component and register this to the the component
	movementComponent = CreateDefaultSubobject<UBallPawnMovementComponent>(TEXT("MovementComponent"));
	movementComponent->UpdatedComponent = RootComponent;
	//then try to find our SFX and create the component if we have one
	static ConstructorHelpers::FObjectFinder<USoundBase> SFXAsset(TEXT("/Game/Sounds/ball_moving.ball_moving"));
	if (SFXAsset.Succeeded()) {
		audioSFXComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SFXAudioComponent"));
		audioSFXComponent->SetSound(SFXAsset.Object);
		audioSFXComponent->SetupAttachment(RootComponent);
	}
	//then try to find our BGM and create the component if we have one
	static ConstructorHelpers::FObjectFinder<USoundBase> BGMAsset(TEXT("/Game/Sounds/technophobia_amachagashi.technophobia_amachagashi"));
	if (BGMAsset.Succeeded()) {
		audioBGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
		audioBGMComponent->SetSound(BGMAsset.Object);
		audioBGMComponent->SetupAttachment(RootComponent);
	}
}

void ABallPawn::startProcessingInput()
{
	isReceivingPanInput = true;
}

void ABallPawn::stopProcessingInput()
{
	isReceivingPanInput = false;
}

void ABallPawn::resetCameraPosition()
{
	FVector cameraCurrent = camera->GetRelativeLocation();
	FVector newPosition = cameraInitialPosition;
	newPosition.Z = cameraCurrent.Z;
	camera->SetRelativeLocation(newPosition);
}

void ABallPawn::resetCameraZoom()
{
	FVector cameraCurrent = camera->GetRelativeLocation();
	cameraCurrent.Z = cameraInitialPosition.Z;
	camera->SetRelativeLocation(cameraCurrent);
}

void ABallPawn::startMovement(const FVector movement, FScriptExecutionContext& ctx)
{
	//as required by the latern protocol the function must save its local state before start yielding
	if (!ctx.contextRestore) {
		//this is the first hit
		Executing = true;
		//set the movement vector
		currentEffectiveMovement = movement;
		//trigger the yield
		ctx.yielding = true;
		//play the audio if found
		if (audioSFXComponent) {
			audioSFXComponent->Play(0);
		}
		GeneralUtilities::Log(this, TEXT("Yielding..."));
	}
	else {
		//the context restore should never happens if the operation not done yet
		check(currentEffectiveMovement.IsNearlyZero());
		Executing = false;
		//stop the audio if found
		if (audioSFXComponent) {
			audioSFXComponent->Stop();
		}
		FEvalResult result = FEvalResult::AsVoidResult();
		PUSH_FAR_VALUE(ctx, FEvalResult, result);
		ctx.contextRestore = false;
	}
}

void ABallPawn::stopMovement()
{
	currentEffectiveMovement=FVector(0.0f);
}

void ABallPawn::signalCompletation()
{
	//called from BallPawnMovementComponent when it met blocking object
	//this simply reset the vector and signal the event
	if (refGameMode) {
		currentEffectiveMovement = FVector(0.0f);
		refGameMode->context.yielding = false;
		GeneralUtilities::Log(this, TEXT("Unyielding..."));
	}
}

FVector ABallPawn::getInitialLocation()
{
	return initialPosition;
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
	//save a ref to external
	refGameMode = Cast<AMazeMainGameMode>(UGameplayStatics::GetGameMode(this));
	//copy the initial position
	initialPosition = this->GetActorLocation();
	if (refGameMode) {
		//load the current status from the mode
		onCharacterStatusChanged(refGameMode->currentConditionalGroupID);
		//register for the character status broadcast
		refGameMode->characterStatusBroadcast.AddDynamic(this, &ABallPawn::onCharacterStatusChanged);
	}
	if (audioSFXComponent) {
		audioSFXComponent->Stop();
	}
}

void ABallPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (audioBGMComponent) {
		audioBGMComponent->Stop();
	}
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!currentEffectiveMovement.IsNearlyZero()) {
		movementComponent->AddInputVector(currentEffectiveMovement);
	}
	if (refGameMode) {
		refGameMode->TickScript();
	}
	//bug fix: there are race condition for the actor's beginplay
	//making the operation unsafe to be done
	//so if there are any failed resolution
	//try to do it here
	if (resolutionFailed) {

		if (refGameMode->groupMaterialMap.Contains(groupID)) {
			//set the material into the one which are registered
			SphereVisual->SetMaterial(0, (refGameMode->groupMaterialMap[groupID]));
		}
		resolutionFailed = false;
	}
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
	//wheels
	PlayerInputComponent->BindKey(EKeys::MouseScrollUp, EInputEvent::IE_Pressed, this, &ABallPawn::onWheelUp);
	PlayerInputComponent->BindKey(EKeys::MouseScrollDown, EInputEvent::IE_Pressed, this, &ABallPawn::onWheelDown);
	//Escape Button
	PlayerInputComponent->BindKey(EKeys::Escape, EInputEvent::IE_Pressed, this, &ABallPawn::onEscapeDown);
}

UPawnMovementComponent* ABallPawn::GetMovementComponent() const
{
	return movementComponent;
}

void ABallPawn::onCharacterStatusChanged(int group)
{
	groupID = group;
	if (group < 0 || !(refGameMode->groupMaterialMap.Contains(group))) {
		//show the default material if the groupID is not registered into the map or it is invalid
		if (defaultMaterial) {
			SphereVisual->SetMaterial(0, defaultMaterial);
		}
		//if the material is not resolvable queue it for the first frame update
		resolutionFailed = !(refGameMode->groupMaterialMap.Contains(group));
	}
	else {
		//otherwise set the material into the one which are registered
		SphereVisual->SetMaterial(0, (refGameMode->groupMaterialMap[group]));
		resolutionFailed = false;
	}
}

void ABallPawn::MouseXYAvis(FVector value)
{
	if (!isReceivingPanInput || !InPanGesture || !currentEffectiveMovement.IsNearlyZero()) {
		return;
	}
	FVector finalMovement = FVector(0);
	//forward Y
	//get the forward vector
	FRotator rot = UKismetMathLibrary::MakeRotFromX(GetActorForwardVector());
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(rot);
	//apply it to the final vector (invert Y vector
	finalMovement = forwardVector * (value.Y * -1);
	//right X
	FVector rightVector = GetActorRightVector();
	//invert the Y as the view should follow the mouse
	finalMovement += (rightVector * value.X *-1);
	//clear the Z
	finalMovement.Z = 0;
	//scale it with delta second and the rate
	float delta=UGameplayStatics::GetWorldDeltaSeconds(this);
	finalMovement *= (delta * panRate);
	//finally add that to the camera
	camera->AddRelativeLocation(finalMovement);
}

void ABallPawn::OnLMBDown()
{
	if (isReceivingPanInput && !Executing) {
		InPanGesture = true;
	}
}

void ABallPawn::OnLMBUp()
{
	if (isReceivingPanInput) {
		InPanGesture = false;
	}
}

void ABallPawn::OnRMBDown()
{
	//just show the IDE back
	if (isReceivingPanInput && refGameMode) {
		this->stopProcessingInput();
		refGameMode->showIDE();
	}
}

void ABallPawn::onWheelDown()
{
	if (!isReceivingPanInput) {
		return;
	}
	//zoom out increase Z by 50 cap to 2230
	FVector cameraCurrent = camera->GetRelativeLocation();
	cameraCurrent.Z = std::min<float>(2230,cameraCurrent.Z + 50);
	camera->SetRelativeLocation(cameraCurrent);
}

void ABallPawn::onEscapeDown()
{
	if (refGameMode) {
		refGameMode->showPauseMenu();
	}
}

void ABallPawn::onWheelUp()
{
	if (!isReceivingPanInput) {
		return;
	}
	//zoom in decrease Z by 50 cap to 230
	FVector cameraCurrent = camera->GetRelativeLocation();
	cameraCurrent.Z = std::max<float>(230, cameraCurrent.Z - 50);
	camera->SetRelativeLocation(cameraCurrent);
}
