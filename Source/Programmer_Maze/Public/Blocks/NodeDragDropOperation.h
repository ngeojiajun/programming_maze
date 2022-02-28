// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Blocks/CodeBlockBaseCPP.h"
#include "NodeDragDropOperation.generated.h"


/**
 * This class describe some metadata used on implementing the drag and drop operation
 * Reference: https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/UMG/HowTo/DragAndDrop/
 */
UCLASS()
class PROGRAMMER_MAZE_API UNodeDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	UCodeBlockBaseCPP* WidgetReference;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FVector2D DragOffset;
};

/*
*This represents the drag widget in the tutorial
*/
UCLASS()
class PROGRAMMER_MAZE_API UNodeDragWidgetCPP :public UUserWidget {
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	UCodeBlockBaseCPP* WidgetReference;
};
