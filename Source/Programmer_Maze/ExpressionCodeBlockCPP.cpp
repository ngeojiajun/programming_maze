// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#include "ExpressionCodeBlockCPP.h"
#include "Components/TextBlock.h"
#include "Components/NamedSlot.h"
#include "Components/GridPanel.h"
#include "NodeDragDropOperation.h"
#include "GeneralUtilities.h"
#include "CodeBlockCPP.h" //we need some to do some special deals with the CodeBlockCPP


UExpressionCodeBlockCPP::UExpressionCodeBlockCPP(const FObjectInitializer& init) :UCodeBlockBaseCPP(init) {
	Type = BlockType::Expression;
	Separator = FText::FromString(TEXT(","));
	Childs.SetNum(2);
}

void UExpressionCodeBlockCPP::Resize()
{
	check(havingSlots());
	//reset the scale
	const float scaleFactor = 0.99;
	SetRenderScale(FVector2D(1));
	//The resize is simple for this kind of block because the only
	//thing to consider is its width
	//the height is determined by the text height
	//Overview
	//Get all the sizes of the slot childs then compute the final ratio
	//The calculation divided into two steps
	//First, calculate the ratio of [texts]:[slots]
	// Seconds, calculate the [slot1]:[slot2]
	//The final ratio will become 1.2:[slot 1*slots]:[slot2:slots]
	//set initial size to the size oof the block name
	FVector2D controlSize = UI_BlockNameBox->GetDesiredSize();
	//calculate the width of the block name
	const float textWidth = std::min<float>(controlSize.X, 70) + UI_Separator->GetDesiredSize().X;
	float Slot1_width = std::max<float>(controlSize.X,70), Slot2_width = Slot1_width;
	//check both slots and see weather it is sloted and take its measurements if
	// it is true
	if (UI_ExpressionSlot1->GetChildrenCount()) {
		UCodeBlockBaseCPP* child = Cast<UCodeBlockBaseCPP>(UI_ExpressionSlot1->GetChildAt(0));
		//ask child to resize
		child->Resize();
		//reset its scale
		child->SetRenderScale(FVector2D(scaleFactor * RenderTransform.Scale.X));
		//the child->size now hold the resolved size of the block
		Slot1_width = child->size.X;
	}
	if (UI_ExpressionSlot2->GetChildrenCount()) {
		UCodeBlockBaseCPP* child = Cast<UCodeBlockBaseCPP>(UI_ExpressionSlot2->GetChildAt(0));
		//ask child to resize
		child->Resize();
		//reset its scale
		child->SetRenderScale(FVector2D(scaleFactor * RenderTransform.Scale.X));
		//the child->size now hold the resolved size of the block
		Slot2_width = child->size.X;
	}
	//lets calculate the cofficients
	//ratio 1.2:$1:$2
	//1:($1/a):($2/a)
	//1.2:(1.2*$1/a):(1.2*$2/a)
	float coffiecientSlot_1 = 1.2 * Slot1_width / textWidth;
	float coffiecientSlot_2 = 1.2 * Slot2_width / textWidth;
	//update the grid cofficients
	UI_MainGrid->SetColumnFill(1, coffiecientSlot_1);
	UI_MainGrid->SetColumnFill(3, coffiecientSlot_2);
	//width is sum of the components width
	controlSize.X = textWidth + Slot1_width + Slot2_width;
	//When it is not slotted
	if (!(Slot && Slot->Parent->GetClass() == UNamedSlot::StaticClass()))
		controlSize.X = std::max<float>(100, controlSize.X);
	controlSize.X += 20; //offset the paddings
	GeneralUtilities::LogVector2D(this, controlSize, TEXT("size"));
	setControlSize(controlSize);
}

bool UExpressionCodeBlockCPP::AddBlockIntoSlot(UCodeBlockBaseCPP* block, int at)
{
	if (!block) {
		return NULL; //dont allow null block
	}
	if (at < 0 || at >= 2) {
		return false; //out of range
	}
	//dont allow statement,iteration and iterative
	const BlockType disallowedTypes[] = { BlockType::Statement,BlockType::Iteration,BlockType::Conditional };
	if (GeneralUtilities::either<BlockType>(block->Type, ARRAY_T(disallowedTypes))) {
		return false;
	}
	//it must be detached from any slot
	if (block->GetParent()) {
		return false;
	}
	if (Childs[at]) {
		return false; //dont allow the sloting if it is already hacing something inside there
	}
	UNamedSlot* slots[] = { UI_ExpressionSlot1,UI_ExpressionSlot2 };
	//add it directly to the repsective slots
	slots[at]->AddChild(block);
	//register it inside the Childs
	Childs[at] = block;
	//ask the root block to perform the resize operation
	rootResize();
	return true;
}

bool UExpressionCodeBlockCPP::RemoveBlockFromSlot(UCodeBlockBaseCPP* blockToRemove)
{
	//if the block is NULL also ignore it
	if (!blockToRemove)return false;
	for (int i = 0; i < 2; i++) {
		UCodeBlockBaseCPP* block = Childs[i];
		if (block == blockToRemove) {
			//remove the element front the slot
			block->RemoveFromParent();
			Childs[i] = NULL;
			return true;
		}
	}
	return false;
}

bool UExpressionCodeBlockCPP::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//ignore the operation if it is template block
	if (this->Template) {
		return false;
	}
	//try to cast the InOperation into UNodeDragDropOperation
	UNodeDragDropOperation* operation = Cast<UNodeDragDropOperation>(InOperation);
	if (!operation) {
		return false;
	}
	FVector2D cursor = InDragDropEvent.GetScreenSpacePosition();
	UNamedSlot* slots[] = { UI_ExpressionSlot1,UI_ExpressionSlot2 };
	int i = 0;
	for (; i < 2; i++) {//check where the pointer was
		UWidget* border = slots[i]->GetParent(); //get the border that warps the slot, it is invisible from the code
		const FGeometry borderGeometry = border->GetCachedGeometry();
		if (GeneralUtilities::insideGeometry(borderGeometry, cursor)) {
			break; //break out from the loop if the check succeeded
		}
	}
	if (i >= 2) { //not caught by any slot so drop it
		return true;
	}
	UCodeBlockBaseCPP* block = operation->WidgetReference;
	//check weather this operation is valid
	const BlockType disallowedTypes[] = { BlockType::Statement,BlockType::Iteration,BlockType::Conditional };
	if (GeneralUtilities::either<BlockType>(block->Type, ARRAY_T(disallowedTypes))) {
		GeneralUtilities::Log(this, TEXT("Aborted the drag and drop because the block in question is not compactible"));
		return true;
	}
	//check the target slot weather is it possible to do it so
	if (Childs[i]) {
		//drop it as there are something inside the slot
		return true;
	}
	//try remove from parent
	if (!block->Template) { //whenever it is not a template block remove it from its parent container
		if (UCodeBlockBaseCPP* parent = block->getParentBlock()) {
			if (UCodeBlockCPP* parentBlock = Cast<UCodeBlockCPP>(parent)) {
				parentBlock->ClearSlot();
			}
			else if (UExpressionCodeBlockCPP* parentExprBlock = Cast<UExpressionCodeBlockCPP>(parent)) {
				parentExprBlock->RemoveBlockFromSlot(block);
			}
			else {
				//cannot cast but it have parent
				//reject
				GeneralUtilities::Log(this, TEXT("Aborted the drag and drop because the block in question is not under a known parent"));
				return true;
			}
		}
	}
	GeneralUtilities::Log(this, TEXT("Attempting to add the block into slot"));
	//now add the new block under this
	AddBlockIntoSlot(block->asUniqueBlock(),i);
	return true;
}
