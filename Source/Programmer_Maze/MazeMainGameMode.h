// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "MazeMainGameMode.generated.h"

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
protected:
	//called when the game starts
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	virtual void hidePanel();
private:
	TSubclassOf<UUserWidget> IDEWidgetClass;
	UPROPERTY()
	UUserWidget* IDEWidgetHandle;
	UPROPERTY()
	UButton* IDECloseButton;
};
