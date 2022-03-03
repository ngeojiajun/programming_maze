// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MazeGameInstance.generated.h"

/**
 * The game instances which this game will use
 */
UCLASS()
class PROGRAMMER_MAZE_API UMazeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//checkpoint id that the map shall teleports to
	UPROPERTY()
	int checkpointID;
};
