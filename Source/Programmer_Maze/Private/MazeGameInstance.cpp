// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "MazeGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GeneralUtilities.h"

#define GAME_SETTINGS_SLOT_NAME TEXT("PROGRAMMING_MAZE_SETTINGS")

UMazeGameInstance::UMazeGameInstance(const FObjectInitializer& ObjectInitializer):UGameInstance(ObjectInitializer) ,
initializationDone(false)
{
	static ConstructorHelpers::FObjectFinder<USoundMix> SC_Asset(TEXT("/Game/Sounds/Classes/SCMix_Master.SCMix_Master"));
	static ConstructorHelpers::FObjectFinder<USoundClass> SCMaster(TEXT("/Game/Sounds/Classes/SC_Master.SC_Master"));
	static ConstructorHelpers::FObjectFinder<USoundClass> SCBGM(TEXT("/Game/Sounds/Classes/SC_BGM.SC_BGM"));
	static ConstructorHelpers::FObjectFinder<USoundClass> SCSFX(TEXT("/Game/Sounds/Classes/SC_SFX.SC_SFX"));
	checkpointID = 0;
	if (SC_Asset.Succeeded()) {
		mixer = SC_Asset.Object;
	}
	if (SCMaster.Succeeded()) {
		masterClass = SCMaster.Object;
	}
	if (SCBGM.Succeeded()) {
		BGMClass = SCBGM.Object;
	}
	if (SCSFX.Succeeded()) {
		SFXClass = SCSFX.Object;
	}
}

void UMazeGameInstance::Init() {
	if (!mixer || !masterClass || !BGMClass || !SFXClass) {
		GeneralUtilities::Log(this, TEXT("Audio engine initialization failed... Volume control will not work"));
		GeneralUtilities::LogBoolean(this, !mixer, TEXT("!mixer"));
		GeneralUtilities::LogBoolean(this, !masterClass, TEXT("!masterClass"));
		GeneralUtilities::LogBoolean(this, !BGMClass, TEXT("!BGMClass"));
		GeneralUtilities::LogBoolean(this, !SFXClass, TEXT("!SFXClass"));
	}
	else {
		//set the mixer as the base mixer
		UGameplayStatics::SetBaseSoundMix(this, mixer);
		//mark the initialization as done
		initializationDone = true;
		//try load the game settings
		if (UGameplayStatics::DoesSaveGameExist(GAME_SETTINGS_SLOT_NAME, 0)) {
			USaveGame* baseData = UGameplayStatics::LoadGameFromSlot(GAME_SETTINGS_SLOT_NAME, 0);
			settings = Cast<UProgrammingMazeSettings>(baseData);
		}
		//if not exists create a new one
		if (!settings) {
			GeneralUtilities::Log(this, TEXT("Settings not found.... creating a new object"));
			settings = Cast<UProgrammingMazeSettings>(UGameplayStatics::CreateSaveGameObject(UProgrammingMazeSettings::StaticClass()));
		}
		//apply the settings to the current instance
		setVolume(ESoundType::Master, settings->MasterVolume);
		setVolume(ESoundType::SFX, settings->SFXVolume);
		setVolume(ESoundType::BGM, settings->BGMVolume);
	}
}

void UMazeGameInstance::Shutdown()
{
	//save the game when shutdown is initiated
	check(settings != NULL);
	UGameplayStatics::SaveGameToSlot(settings, GAME_SETTINGS_SLOT_NAME, 0);
}

void UMazeGameInstance::setVolume(TEnumAsByte<ESoundType> type, float volume)
{
	if (!initializationDone) {
		return;
	}
	USoundClass* clzz = NULL;
	switch (type) {
	case ESoundType::BGM:
		clzz = BGMClass;
		settings->BGMVolume = volume;
		break;
	case ESoundType::Master:
		clzz = masterClass;
		settings->MasterVolume = volume;
		break;
	case ESoundType::SFX:
		clzz = SFXClass;
		settings->SFXVolume = volume;
		break;
	default:
		checkNoEntry();
	}
	//set the override with the original values except the volume to the requested and the fade in time to zero to enforce it immediately
	UGameplayStatics::SetSoundMixClassOverride(this, mixer, clzz, volume, 1.0f, 0.0f, true);
}
