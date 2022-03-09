// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "Maze/MazeMainGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Maze/BallPawn.h"
#include "Maze/ButtonInstantiations.h"
#include "Blocks/CodeBlockCPP.h"
#include "Components/MultiLineEditableText.h"
#include "ProgrammingMazeGameSaves.h"
#include "MazeGameInstance.h"
#include "GeneralUtilities.h"

AMazeMainGameMode::AMazeMainGameMode() :AGameModeBase(),evaluationRunning(false),lastCheckpointId(-1) {
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/IDE/IDE.IDE_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> HelpDialogClassFinder(TEXT("/Game/UI/Flow/DialogHelp.DialogHelp_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> DialogClassFinder(TEXT("/Game/UI/Flow/DialogBase.DialogBase_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuClassFinder(TEXT("/Game/UI/Flow/PauseMenu.PauseMenu_C"));
	IDEWidgetClass = WidgetClassFinder.Class;
	IDEHelpDialogClass = HelpDialogClassFinder.Class;
	IDEDialogClass = DialogClassFinder.Class;
	PauseMenuClass = PauseMenuClassFinder.Class;
	DefaultPawnClass = ABallPawn::StaticClass();
	currentLevelName = FName();
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
		context.forceUnwind = false;
		evaluationRunning = true;
		//prevent the movement
		IDEStartBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
		IDEGogoButton->SetIsEnabled(false);
		IDEHelpButton->SetIsEnabled(false);
		//reset the zoom and camera position
		context.ptrPawn->resetCameraPosition();
		context.ptrPawn->resetCameraZoom();
		//fire it
		IDEStartBlock->eval(context);
	}
}

void AMazeMainGameMode::terminateEvaluation()
{
	//this function should terminate the execution of the block
	//to do this
	//Stop signal must be sent to all the participants
	if (!this->evaluationRunning) {
		return; //nothing to do
	}
	//ask the ball to stop moving
	context.ptrPawn->stopMovement();
	//set the forceUnwind flag to notify all the functions to not continue
	context.forceUnwind = true;
	//unyield the script now
	context.yielding = false;
	//hide the pause menu
	PauseMenuHandle->SetVisibility(ESlateVisibility::Collapsed);
}

void AMazeMainGameMode::wrapPawnToLastCheckpoint()
{
	FVector initial = context.ptrPawn->getInitialLocation();
	if (lastCheckpointId < 0) {
		//invalid checkpoint just wrap to the initial point
		context.ptrPawn->TeleportTo(initial,FRotator(),false,true);
	}
	else {
		//find all classes with the ACheckpointPawn
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointPawn::StaticClass(), FoundActors);
		for (int i = 0; i < FoundActors.Num(); i++) {
			//check weather there are one with the said id
			ACheckpointPawn* button = (ACheckpointPawn*)FoundActors[i]; //always succeeded as the check is done by the iterator
			if (button->checkpointId == lastCheckpointId) {
				//if yes
				FVector vec = button->GetActorLocation(); //get the location
				//set the Z (height) to the initial height
				vec.Z = initial.Z;
				//teleport
				context.ptrPawn->TeleportTo(vec, FRotator(), false, true);
				return;
			}
		}
		//none found teleport to the initial
		context.ptrPawn->TeleportTo(initial, FRotator(), false, true);
	}
}

void AMazeMainGameMode::showDialog(FString name, FString content)
{
	if (IDEDialogHandle) {
		//get the handle, just set it directly
		FStrProperty* prop = FindFieldChecked<FStrProperty>(IDEDialogClass, FName(TEXT("Title")));
		prop->SetPropertyValue(prop->ContainerPtrToValuePtr<void*>(IDEDialogHandle), name);
		FObjectProperty* propObj = FindFieldChecked<FObjectProperty>(IDEDialogClass, FName(TEXT("ContextBox")));
		UMultiLineEditableText* contextBox= Cast<UMultiLineEditableText>(propObj->GetObjectPropertyValue(propObj->ContainerPtrToValuePtr<UObject>(IDEDialogHandle)));
		contextBox->SetText(FText::FromString(content));
		IDEDialogHandle->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		IDEDialogHandle = NewObject<UUserWidget>(this, IDEDialogClass);
		FStrProperty* prop = FindFieldChecked<FStrProperty>(IDEDialogClass, FName(TEXT("Title")));
		prop->SetPropertyValue(prop->ContainerPtrToValuePtr<void*>(IDEDialogHandle), name);
		prop = FindFieldChecked<FStrProperty>(IDEDialogClass, FName(TEXT("Content")));
		prop->SetPropertyValue(prop->ContainerPtrToValuePtr<void*>(IDEDialogHandle), content);
		IDEDialogHandle->SetVisibility(ESlateVisibility::Visible);
		IDEDialogHandle->AddToViewport(3);
	}
}

UProgrammingMazeLevelSaves* AMazeMainGameMode::serializeCurrentGame()
{
	UProgrammingMazeLevelSaves* retVal= Cast<UProgrammingMazeLevelSaves>(UGameplayStatics::CreateSaveGameObject(UProgrammingMazeLevelSaves::StaticClass()));
	//save current level name
	retVal->LevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
	//save current checkpoint ID
	retVal->checkpointID = this->lastCheckpointId;
	//save the current time
	retVal->SaveTime = FDateTime::Now();
	//return it to caller
	return retVal;
}

void AMazeMainGameMode::saveCurrentGame()
{
	//serialize the data
	UProgrammingMazeLevelSaves* data = serializeCurrentGame();
	//instance
	UMazeGameInstance* instance = Cast<UMazeGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (instance) {
		//file name here
		FString name = FString::Printf(TEXT("PROGRAMMING_MAZE_SAV_%d"), instance->settings->savefileIndex);
		//save it
		bool ok = UGameplayStatics::SaveGameToSlot(data, name, 0);
		if (ok) {
			//update the index when it is done succeessfully
			//cap it to eight
			instance->settings->savefileIndex = (instance->settings->savefileIndex + 1) %8;
			//disable the save button and write saved to the button
			PauseMenuSaveButton->SetIsEnabled(false);
			UTextBlock* txt = Cast<UTextBlock>(PauseMenuSaveButton->GetChildAt(0));
			txt->SetText(FText::FromString(TEXT("Saved")));
		}
	}
}

void AMazeMainGameMode::executionDone(FEvalResult result)
{
	GeneralUtilities::LogBoolean(this, result.succeeded, TEXT("Execution succeeded"));
	GeneralUtilities::Log(this, result.strVal);
	evaluationRunning = false;
	if (result.succeeded) {
		wrapPawnToLastCheckpoint();
	}
	else {
		//show the error dialog
		showDialog(TEXT("Error"),result.errorMsg);
		//force reset the game state when the script error was thrown by the block
		FVector initial = context.ptrPawn->getInitialLocation();
		context.ptrPawn->TeleportTo(initial, FRotator(), false, true);
		characterStatusBroadcast.Broadcast(-1);
	}
	IDEStartBlock->SetVisibility(ESlateVisibility::Visible);
	IDEGogoButton->SetIsEnabled(true);
	IDEHelpButton->SetIsEnabled(true);
	//reset the zoom
	context.ptrPawn->resetCameraZoom();
	//ensure the IDE is shown
	context.ptrPawn->stopProcessingInput();
	showIDE();
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

void AMazeMainGameMode::onCheckpointHit(int id)
{
	//note it down the id of the checkpoint the ball last hit
	this->lastCheckpointId = id;
}

void AMazeMainGameMode::onLevelCompleted(AGoalPawn* buttonHit)
{
	if (buttonHit->nextLevelID.IsNone()) {
		UGameplayStatics::OpenLevel(this, FName("CongratsLevel"));
	}
	else {
		UGameplayStatics::OpenLevel(this, buttonHit->nextLevelID);
	}
}

void AMazeMainGameMode::showPauseMenu()
{
	if (PauseMenuHandle->GetVisibility() == ESlateVisibility::Visible) {
		//hide it if it is shown currently
		PauseMenuHandle->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		//restore thg original text of the save button
		PauseMenuSaveButton->SetIsEnabled(!evaluationRunning); //allow save only if it is not executing
		UTextBlock* txt = Cast<UTextBlock>(PauseMenuSaveButton->GetChildAt(0));
		txt->SetText(FText::FromString(TEXT("Save Game")));
		PauseMenuHandle->SetVisibility(ESlateVisibility::Visible);
		//allow terminate only if it is executing
		PauseMenuTerminateButton->SetIsEnabled(evaluationRunning);
	}
}

void AMazeMainGameMode::BeginPlay() {
	Super::BeginPlay();
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
	//By reflection get the IDE::HelpButton
	prop = FindFieldChecked<FObjectProperty>(IDEWidgetClass, FName(TEXT("HelpButton")));
	IDEHelpButton = Cast<UButton>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(IDEWidgetHandle)));
	//Step8:
	//Attach onclick handle to it
	IDEHelpButton->OnClicked.AddDynamic(this, &AMazeMainGameMode::showHelp);
	//Step9:
	//Prefill the context
	context = FScriptExecutionContext();
	context.ptrGameMode = this;
	context.contextRestore = false;
	context.ptrPawn = Cast<ABallPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	//Step10:
	//Broadcast a characterStatusBroadcast (-1) to force all walls to be deassociated
	characterStatusBroadcast.Broadcast(-1);
	//Step11:
	//Construct the help dialog then add it into the tree but make it hidden by default
	IDEHelpDialogHandle = NewObject<UUserWidget>(this, IDEHelpDialogClass);
	IDEHelpDialogHandle->SetVisibility(ESlateVisibility::Collapsed);
	IDEHelpDialogHandle->AddToViewport(1);
	//Step12:
	//By reflection get the IDE::LevelName
	prop = FindFieldChecked<FObjectProperty>(IDEWidgetClass, FName(TEXT("LevelName")));
	IDELevelNameBlock = Cast<UTextBlock>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(IDEWidgetHandle)));
	//Step 13: If the level name is set by the GoalButton then set it here (RACE CONDITION)
	if (!currentLevelName.IsNone()) {
		IDELevelNameBlock->SetText(FText::FromName(currentLevelName));
	}
	//Step 14:
	//Create pause menu and add it to the screen as hidden
	PauseMenuHandle = NewObject<UUserWidget>(this, PauseMenuClass);
	PauseMenuHandle->SetVisibility(ESlateVisibility::Collapsed);
	PauseMenuHandle->AddToViewport(3);
	//Step 15:
	//By reflection get the PauseMenu::ButtonTerminate
	prop = FindFieldChecked<FObjectProperty>(PauseMenuClass, FName(TEXT("ButtonTerminate")));
	PauseMenuTerminateButton = Cast<UButton>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(PauseMenuHandle)));
	//Step 16:
	//By reflection get the PauseMenu::SaveGameButton
	prop = FindFieldChecked<FObjectProperty>(PauseMenuClass, FName(TEXT("SaveGameButton")));
	PauseMenuSaveButton = Cast<UButton>(prop->GetObjectPropertyValue(prop->ContainerPtrToValuePtr<UObject>(PauseMenuHandle)));
	//Step 17:
	//Add event handler to the SaveGameButton
	PauseMenuSaveButton->OnClicked.AddDynamic(this, &AMazeMainGameMode::saveCurrentGame);
	//Step 18:
	//Sync the checkpointID from the game instance
	UMazeGameInstance* instance = Cast<UMazeGameInstance>(UGameplayStatics::GetGameInstance(this));
	lastCheckpointId = instance->checkpointID;
	if (lastCheckpointId >= 0) { //if the id is valid wrap the ball to there
		wrapPawnToLastCheckpoint();
	}
	//clear the checkpointID of the game instance
	instance->checkpointID = -1;
	//Step 19:
	//Add event handler to ButtonTerminate
	PauseMenuTerminateButton->OnClicked.AddDynamic(this,&AMazeMainGameMode::terminateEvaluation);
}

void AMazeMainGameMode::hidePanel()
{
	//when the close button clicked
	//hide the panel
	IDEWidgetHandle->SetVisibility(ESlateVisibility::Collapsed);
	//ask the pawn to start process input
	context.ptrPawn->startProcessingInput();
}

void AMazeMainGameMode::showHelp()
{
	IDEHelpDialogHandle->SetVisibility(ESlateVisibility::Visible);
}
