// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "ExpressionCodeBlockCPP.h"

UExpressionCodeBlockCPP::UExpressionCodeBlockCPP(const FObjectInitializer& init) :UCodeBlockBaseCPP(init) {
	Type = BlockType::Expression;
	Separator = FText::FromString(TEXT(","));
}

void UExpressionCodeBlockCPP::Resize()
{
	//The resize is simple for this kind of block because the only
	//thing to consider is its width
	//the height is determined by the text height
	//Overview
	//Get all the sizes of the slot childs then compute the final ratio
	//The calculation divided into two steps
	//First, calculate the ratio of [texts]:[slots]
	// Seconds, calculate the [slot1]:[slot2]
	//The final ratio will become 1:[slot 1*slots]:[slot2:slots]
}

bool UExpressionCodeBlockCPP::AddBlockIntoSlot(UCodeBlockBaseCPP* block, int at)
{
	unimplemented();
	return false;
}
