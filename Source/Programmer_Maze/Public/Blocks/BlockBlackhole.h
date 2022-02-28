// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlockBlackhole.generated.h"

/**
 * This widget is just an utility widget to remove unused blocks
 * by dragging onto it
 */
UCLASS()
class PROGRAMMER_MAZE_API UBlockBlackhole : public UUserWidget
{
	GENERATED_BODY()
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
