// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeBlockBaseCPP.generated.h"

/*
*The type of the block
*/
UENUM()
enum BlockType{
	Statement,
	Conditional,
	Iteration,
	Expression,
	Variable,
};

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
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TEnumAsByte<BlockType> Type;
	/*
	*The child nodes associated with this block
	* For iterative and conditional it have following layout
	* [0] = Condition slot
	* [1+] = child nodes
	* 
	*For expression it is literally a flat array of slots
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
	void setFinalRenderScale(FVector2D);
	/*
	* Computed size of the control
	*/
	FVector2D size;
protected:
	//The construction/init in CPP
	virtual void NativeConstruct() override;
	void setControlSize(const FVector2D size);
	bool havingChilds() const;
	bool havingSlots() const;
	FVector2D finalRenderScale;
};
