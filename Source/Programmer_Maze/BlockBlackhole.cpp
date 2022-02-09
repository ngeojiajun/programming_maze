// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#include "BlockBlackhole.h"
#include "CodeBlockCPP.h"
#include "ExpressionCodeBlockCPP.h"
#include "NodeDragDropOperation.h"
#include "GeneralUtilities.h"

bool UBlockBlackhole::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//try to cast the InOperation into UNodeDragDropOperation
	UNodeDragDropOperation* operation = Cast<UNodeDragDropOperation>(InOperation);
	if (!operation) {
		return false;
	}
	UCodeBlockBaseCPP* block = operation->WidgetReference;
	//ignore the template blocks
	if (block->Template) {
		return true;
	}
	switch (block->Type) {
	case BlockType::Conditional:
	case BlockType::Iteration:
	case BlockType::Statement:{
		//slotted to UI_Childs
		if (UCodeBlockBaseCPP* parent = block->getParentBlock()) {
			UCodeBlockCPP* parentBlock = Cast<UCodeBlockCPP>(parent);
			if (!parentBlock) {
				//cannot cast but it have parent
				//reject
				GeneralUtilities::Log(this, TEXT("Aborted the removal because the block in question is not under a known parent"));
			}
			parentBlock->RemoveChildBlock(block);
		}
		return true;
	}
	case BlockType::Expression:
	case BlockType::Variable:
	case BlockType::Constant: {
		//slotted to expression
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
				GeneralUtilities::Log(this, TEXT("Aborted the removal because the block in question is not under a known parent"));
			}
		}
		return true;
	}
	case BlockType::Start:
	default: {
		//never reachable as is this happen then the check in UCodeBlockBaseCPP is broke
		checkNoEntry();
		return true;
	}
	}
}
