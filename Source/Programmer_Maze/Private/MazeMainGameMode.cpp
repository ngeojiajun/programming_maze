// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "MazeMainGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "BallPawn.h"
#include "CodeBlockCPP.h"
#include "GeneralUtilities.h"

AMazeMainGameMode::AMazeMainGameMode() :AGameModeBase(),evaluationRunning(false) {
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/IDE/IDE.IDE_C"));
	IDEWidgetClass = WidgetClassFinder.Class;
	DefaultPawnClass = ABallPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true; //enable ticking
}

void AMazeMainGameMode::showIDE()
{
	IDEWidgetHandle->SetVisibility(ESlateVisibility::Visible);
}

void AMazeMainGameMode::gogogo()
{
	if (!evaluationRunning) {
		context.sp = 0;
		context.contextRestore = false;
		context.yielding = false;
		evaluationRunning = true;
		//fire it
		IDEStartBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		IDEStartBlock->eval(context);
	}
}

void AMazeMainGameMode::executionDone(FEvalResult result)
{
	GeneralUtilities::LogBoolean(this, result.succeeded, TEXT("Execution succeeded"));
	GeneralUtilities::Log(this, result.strVal);
	evaluationRunning = false;
	IDEStartBlock->SetVisibility(ESlateVisibility::Visible);
}

void AMazeMainGameMode::TickScript()
{
	if (evaluationRunning && !context.yielding) {
		//the context is not longer yielding so execute it
		//set the contextRestore so all participant restore it
		context.sp = 0;
		context.contextRestore = true;
		IDEStartBlock->eval(context);
	}
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
	//Step5:
	//By reflection get the IDE::GoButton
	prop = FindFieldChecked<FObjectProperty>(IDEWidgetClass, FName(TEXT("GoButton")));
	IDEGogoButton = Cast<UButton>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(IDEWidgetHandle)));
	//Step6:
	//Attach onclick handle to it 
	IDEGogoButton->OnClicked.AddDynamic(this, &AMazeMainGameMode::gogogo);
	//Step6:
	//By reflection get the IDE::StartBlock
	prop = FindFieldChecked<FObjectProperty>(IDEWidgetClass, FName(TEXT("StartBlock")));
	IDEStartBlock = Cast<UCodeBlockCPP>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(IDEWidgetHandle)));
	//Step7:
	//Prefill the context
	context = FScriptExecutionContext();
	context.ptrGameMode = this;
	context.contextRestore = false;
	context.ptrPawn = Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
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
