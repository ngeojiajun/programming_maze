// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "MazeGameInstance.generated.h"

enum class SoundType {
	BGM,SFX,Master
};

/**
 * The game instances which this game will use
 */
UCLASS()
class PROGRAMMER_MAZE_API UMazeGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMazeGameInstance(const FObjectInitializer& ObjectInitializer);
	//initialize the instance
	virtual void Init();
	//adjust the volume
	void setVolume(SoundType type, float volume);
	//checkpoint id that the map shall teleports to
	UPROPERTY()
	int checkpointID;
private:
	//the global sound mixer for the volume control
	UPROPERTY()
	USoundMix* mixer;
	//sound class for identifing the sound type
	UPROPERTY()
	USoundClass* masterClass; //to be removed as we never use it in production, just for the PoC
	//BGM sound class
	UPROPERTY()
	USoundClass* BGMClass;
	//SFX sound class
	UPROPERTY()
	USoundClass* SFXClass;
	//weather the resolution succeeded
	bool initializationDone;
};
