// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "MazeGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GeneralUtilities.h"

UMazeGameInstance::UMazeGameInstance(const FObjectInitializer& ObjectInitializer):UGameInstance(ObjectInitializer) {
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
		UGameplayStatics::SetBaseSoundMix(this, mixer);
		initializationDone = true;
	}
}

void UMazeGameInstance::setVolume(SoundType type, float volume)
{
	if (!initializationDone) {
		return;
	}
	USoundClass* clzz = NULL;
	switch (type) {
	case SoundType::BGM:
		clzz = BGMClass;
		break;
	case SoundType::Master:
		clzz = masterClass;
		break;
	case SoundType::SFX:
		clzz = SFXClass;
		break;
	default:
		checkNoEntry();
	}
	//set the override with the original values except the volume to the requested and the fade in time to zero to enforce it immediately
	UGameplayStatics::SetSoundMixClassOverride(this, mixer, clzz, volume, 1.0f, 0.0f, true);
	//TODO save this setting
}
