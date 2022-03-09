// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "ProgrammingMazeGameSaves.h"
#include "LoadGameMenuMode.generated.h"

/**
 * Game mode just for displaying the continue menu wwwwww
 */
UCLASS()
class PROGRAMMER_MAZE_API ALoadGameMenuMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALoadGameMenuMode();
	virtual void BeginPlay() override;
private:
	TSubclassOf<UUserWidget> ContinueMenuClass;
	UPROPERTY()
	UObject* fontToUse;
	UPROPERTY()
	UUserWidget* ContinueMenuHandle;
	void fillPanel(UGridPanel* panel);
	void loadSaveInternal(UProgrammingMazeLevelSaves* save);
	TArray<UProgrammingMazeLevelSaves*> loadedSaves;
	UFUNCTION()
	void LoadSave0();
	UFUNCTION()
	void LoadSave1();
	UFUNCTION()
	void LoadSave2();
	UFUNCTION()
	void LoadSave3();
	UFUNCTION()
	void LoadSave4();
	UFUNCTION()
	void LoadSave5();
	UFUNCTION()
	void LoadSave6();
	UFUNCTION()
	void LoadSave7();
};
