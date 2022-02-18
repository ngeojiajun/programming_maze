// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "MazeMainGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AMazeMainGameMode::AMazeMainGameMode() :AGameModeBase() {
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/IDE/IDE.IDE_C"));
	IDEWidgetClass = WidgetClassFinder.Class;
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
}