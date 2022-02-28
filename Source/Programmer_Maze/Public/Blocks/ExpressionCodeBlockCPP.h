// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blocks/CodeBlockBaseCPP.h"
#include "ExpressionCodeBlockCPP.generated.h"

/**
 * Expression code block
 * This class inteprets its Childs member in a difference way
 * Childs - linear array of slots
 */
UCLASS()
class PROGRAMMER_MAZE_API UExpressionCodeBlockCPP : public UCodeBlockBaseCPP
{
	GENERATED_BODY()
public:
	UExpressionCodeBlockCPP(const FObjectInitializer&);
	/*The symbol used to seperate two operands*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText Separator;
	/*
	*Bindings
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UScaleBox* UI_TextScaleBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UGridPanel* UI_MainGrid;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* UI_BlockNameBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* UI_Separator;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UNamedSlot* UI_ExpressionSlot1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UNamedSlot* UI_ExpressionSlot2;
	/*
	*Core APIs
	*/
	/*
	*Recompute the actual size of this control
	*/
	virtual void Resize();
	/*
	*Add a block into the slot
	*/
	UFUNCTION(BlueprintCallable)
	bool AddBlockIntoSlot(UCodeBlockBaseCPP* block,int at);
	UFUNCTION(BlueprintCallable)
	bool RemoveBlockFromSlot(UCodeBlockBaseCPP* blockToRemove);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
