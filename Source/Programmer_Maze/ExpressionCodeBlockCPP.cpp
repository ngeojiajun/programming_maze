// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#include "ExpressionCodeBlockCPP.h"
#include "Components/TextBlock.h"
#include "Components/NamedSlot.h"
#include "Components/GridPanel.h"


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
	UE_LOG(LogTemp, Warning, TEXT("%s: size=(%f,%f)"), *(GetName()), controlSize.X, controlSize.Y);
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
	if (block->Type == BlockType::Statement || block->Type == BlockType::Iteration || block->Type == BlockType::Conditional) {
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
	return false;
}
