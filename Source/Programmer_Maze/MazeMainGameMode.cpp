// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "MazeMainGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GeneralUtilities.h"

AMazeMainGameMode::AMazeMainGameMode() :AGameModeBase() {
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/IDE/IDE.IDE_C"));
	static ConstructorHelpers::FClassFinder<AActor> ActorClassFinder(TEXT("/Game/UI/Actors/Ball.Ball_C"));
	IDEWidgetClass = WidgetClassFinder.Class;
	DefaultPawnClass = ActorClassFinder.Class;
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
}