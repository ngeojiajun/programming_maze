// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once


#include <functional>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeBlockDefs.h"
#include "CodeBlockBaseCPP.generated.h"

class UNodeDragWidgetCPP;

/**
 * This class define the common interface between the code blocks
 */
UCLASS()
class PROGRAMMER_MAZE_API UCodeBlockBaseCPP : public UUserWidget
{
	GENERATED_BODY()
public:
	UCodeBlockBaseCPP(const FObjectInitializer&);
	/*
	*Properties
	*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FText Name;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	TEnumAsByte<BlockType> Type;
	/*
	*The child nodes associated with this block
	* [0] = Condition slot
	* [1+] = child nodes
	*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<UCodeBlockBaseCPP*> Childs;
	/*
	*Getters
	*/
	/*
	*Get the Background color that this control should have
	*/
	UFUNCTION(BlueprintCallable)
	FLinearColor GetCurrentBGColor() const;
	UFUNCTION(BlueprintCallable)
	FLinearColor GetSlotColor() const;
	/*
	* Event handlers for native code outside the owning class
	*/
	std::function<bool(UCodeBlockBaseCPP*,FEvalResult&)> nativeEval;
	/*
	*Some of core API
	*/
	/*
	*Recompute the actual size of this control
	*/
	UFUNCTION(BlueprintCallable)
	virtual void Resize();
	/*
	*Set the final render scale
	*/
	UFUNCTION(BlueprintCallable)
	void setFinalRenderScale(FVector2D scale);
	/*
	* Computed size of the control
	*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FVector2D size;
	/*
	*The render scale used finally in after the siz is calculated
	*/
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, BlueprintSetter=setFinalRenderScale)
	FVector2D finalRenderScale;
	/*
	*Note this function must be implemented by call the blocks that can be cloned
	*we cannot implement it simply by creating ourself because the settings and other.
	*Childs are not cloned because the controls only created when it is attached.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UCodeBlockBaseCPP* clone();
	virtual UCodeBlockBaseCPP* clone_Implementation();
	/*
	*Execute this block
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FEvalResult eval();
	virtual FEvalResult eval_Implementation();
	protected:
	//The construction/init in CPP
	virtual void NativeConstruct() override;
	void setControlSize(const FVector2D size);
	bool havingChilds() const;
	bool havingSlots() const;
	//For internal use only. used to allow the root object to resize for performance
	void rootResize();
	//drag and drop
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
private:
	FVector2D DragOffset;
	UNodeDragWidgetCPP* DraggedWidget;
	TSubclassOf<UNodeDragWidgetCPP> DragWidgetClass;
};
