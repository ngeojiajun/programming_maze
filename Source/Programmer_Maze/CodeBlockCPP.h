// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "CodeBlockBaseCPP.h"
#include "CodeBlockCPP.generated.h"

/**
 * This class define the interface for the general code blocks which have following properties:
 * 1)Expression ,conditional and iterative will have at most one slot
 * 2)Other types do not have slot
 * This also implement support for the child blocks for the iterative and conditional
 */
UCLASS()
class PROGRAMMER_MAZE_API UCodeBlockCPP : public UCodeBlockBaseCPP {
	GENERATED_BODY()
public:
	UCodeBlockCPP(const FObjectInitializer&);
	/*
	*Bindings
	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UScaleBox* UI_BlockNameScaleBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* UI_MainGrid;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* UI_TitleGrid;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UGridPanel* UI_ChildGrid;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* UI_BlockName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UNamedSlot* UI_Expression;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBorder* UI_ExpressionBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UScrollBox* UI_Childs;
	/*
	*Core APIs
	*/
	/*
	*Recompute the actual size of this control
	*/
	virtual void Resize();
	/*
	*Add a block as the child
	*/
	UFUNCTION(BlueprintCallable)
	bool AddChildBlock(UCodeBlockBaseCPP* block);
};