// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "CodeBlockDefs.h"
#include "ScriptExecutionContext.h"
#include "MazeMainGameMode.generated.h"

class UCodeBlockCPP;
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
	UPROPERTY()
	FScriptExecutionContext context;
protected:
	//called when the game starts
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	virtual void hidePanel();
	UFUNCTION()
	void gogogo();
	//warp the player pawn to the last checkpoint that the engine has taken note
	void wrapPawnToLastCheckpoint();
	TSubclassOf<UUserWidget> IDEWidgetClass;
	UPROPERTY()
	UUserWidget* IDEWidgetHandle;
	UPROPERTY()
	UButton* IDECloseButton;
	UPROPERTY()
	UButton* IDEGogoButton;
	UPROPERTY()
	UCodeBlockCPP* IDEStartBlock;
	bool evaluationRunning;
	int lastCheckpointId;
};
