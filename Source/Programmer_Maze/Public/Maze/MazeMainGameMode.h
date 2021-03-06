// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blocks/CodeBlockDefs.h"
#include "Blocks/ScriptExecutionContext.h"
#include "MazeMainGameMode.generated.h"

class UCodeBlockCPP;
class AGoalPawn;
class UProgrammingMazeLevelSaves;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStatusChanged, int, groupId);
/**
 *
 */
UCLASS()
class PROGRAMMER_MAZE_API AMazeMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	//constructor
	AMazeMainGameMode();
	//reveal the IDE
	void showIDE();
	//callback from Start Block after the execution done
	void executionDone(FEvalResult result);
	//Chained with Tick() from ball pawn
	void TickScript();
	//for checkpoint buttons
	//called by the button when the ball passed through the checkpoint
	void onCheckpointHit(int id);
	//when the level complemeted
	void onLevelCompleted(AGoalPawn* buttonHit);
	//show the game pause menu
	void showPauseMenu();
	UPROPERTY()
	FScriptExecutionContext context;
	//not used by this class but to allow ball, buttons and walls to sync its state
	UPROPERTY()
	FOnCharacterStatusChanged characterStatusBroadcast;
	//this holds the association of the groupID with the material bound to it
	TMap<int, UMaterialInterface*> groupMaterialMap;
	UPROPERTY()
	UTextBlock* IDELevelNameBlock;
	FName currentLevelName;
	int currentConditionalGroupID;
protected:
	//called when the game starts
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void hidePanel();
	UFUNCTION()
	void showHelp();
	UFUNCTION()
	void gogogo();
	//terminate the block evaluation
	UFUNCTION()
	void terminateEvaluation();
	//listener for the characterStatusBroadcast
	UFUNCTION()
	void onCharacterStatusChanged(int group);
	//warp the player pawn to the last checkpoint that the engine has taken note
	void wrapPawnToLastCheckpoint();
	//show the dialog
	void showDialog(FString name, FString content);
	//Create a game save object representing current game state
	UProgrammingMazeLevelSaves* serializeCurrentGame();
	//save current state
	UFUNCTION()
	void saveCurrentGame();
	TSubclassOf<UUserWidget> IDEWidgetClass;
	TSubclassOf<UUserWidget> IDEHelpDialogClass;
	TSubclassOf<UUserWidget> IDEDialogClass;
	TSubclassOf<UUserWidget> PauseMenuClass;
	//UI handles
	UPROPERTY()
	UUserWidget* IDEWidgetHandle;
	UPROPERTY()
	UUserWidget* IDEHelpDialogHandle;
	UPROPERTY()
	UUserWidget* IDEDialogHandle;
	UPROPERTY()
	UUserWidget* PauseMenuHandle;
	//IDE controls
	UPROPERTY()
	UButton* IDECloseButton;
	UPROPERTY()
	UButton* IDEGogoButton;
	UPROPERTY()
	UButton* IDEHelpButton;
	UPROPERTY()
	UCodeBlockCPP* IDEStartBlock;
	//Pause Menu controls
	UPROPERTY()
	UButton* PauseMenuTerminateButton;
	UPROPERTY()
	UButton* PauseMenuSaveButton;
	bool evaluationRunning;
	int lastCheckpointId;
};
