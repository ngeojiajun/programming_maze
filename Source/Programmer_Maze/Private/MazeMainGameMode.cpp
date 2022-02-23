// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "MazeMainGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BallPawn.h"
#include "GeneralUtilities.h"

AMazeMainGameMode::AMazeMainGameMode() :AGameModeBase() {
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/IDE/IDE.IDE_C"));
	IDEWidgetClass = WidgetClassFinder.Class;
	DefaultPawnClass = ABallPawn::StaticClass();
}

void AMazeMainGameMode::showIDE()
{
	IDEWidgetHandle->SetVisibility(ESlateVisibility::Visible);
}

void AMazeMainGameMode::executionDone(FEvalResult result)
{
	GeneralUtilities::LogBoolean(this, result.succeeded, TEXT("Execution succeeded"));
}

void AMazeMainGameMode::BeginPlay() {
	//Step 1:
	//Construct the IDE widget and add it to the viewport
	IDEWidgetHandle = NewObject<UUserWidget>(this, IDEWidgetClass);
	IDEWidgetHandle->SetVisibility(ESlateVisibility::Visible);
	IDEWidgetHandle->AddToViewport();
	//Step 2:
	//Enable mouse cursor
	APlayerController* controller=UGameplayStatics::GetPlayerController(this, 0);
	controller->SetShowMouseCursor(true);
	//Step3:
	//By reflection get the IDE::CloseButton
	FObjectProperty* prop=FindFieldChecked<FObjectProperty>(IDEWidgetClass, FName(TEXT("CloseButton")));
	IDECloseButton=Cast<UButton>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(IDEWidgetHandle)));
	//Step4:
	//Attach onclick handle to it to hide the panel when it is clicked
	IDECloseButton->OnClicked.AddDynamic(this,&AMazeMainGameMode::hidePanel);
}

void AMazeMainGameMode::hidePanel()
{
	//when the close button clicked
	//hide the panel
	IDEWidgetHandle->SetVisibility(ESlateVisibility::Collapsed);
	//ask the pawn to start process input
	ABallPawn* target=Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	target->startProcessingInput();
}
