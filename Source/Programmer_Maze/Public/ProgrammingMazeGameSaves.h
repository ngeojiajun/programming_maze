// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProgrammingMazeGameSaves.generated.h"

/**
 * This class hold the information specific to the level
 */
UCLASS()
class PROGRAMMER_MAZE_API UProgrammingMazeLevelSaves : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName LevelName;
	UPROPERTY()
	int checkpointID;
};

/**
 * This class hold the information regarding the game settings
 */
UCLASS()
class PROGRAMMER_MAZE_API UProgrammingMazeSettings : public USaveGame
{
	GENERATED_BODY()
public:
	UProgrammingMazeSettings(const FObjectInitializer& ObjectInitializer);
	UPROPERTY()
	float MasterVolume;
	UPROPERTY()
	float BGMVolume;
	UPROPERTY()
	float SFXVolume;
};