// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "CodeBlockCPP.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/Border.h"
#include "Components/NamedSlot.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

const float gapBetweenBlocks = 15;

UCodeBlockCPP::UCodeBlockCPP(const FObjectInitializer& init):UCodeBlockBaseCPP(init) {
}

void UCodeBlockCPP::Resize()
{
	//reset the scale
	const float scaleFactor = 0.99;
	SetRenderScale(FVector2D(1));
	//Stage 1: Reconfigurate the grids base on the information provided now
	//1) Whenever it do not have child, there are no extra grids just make it a simple [canvas>grid>grid>scale>text]
	//2) If it contains slot then the title grid is divided into 1:2 propotion
	//3) If it contains child then the main grid is divided into 1:4:1 propotion
	//DONT forget to remove the child controls from its parent as it is appearantly the grid panel do not follow the specification
	//STEP one
	if (!havingChilds()) {
		/*
		*Collapse that into 1 row 1 column
		*/
		UI_MainGrid->SetRowFill(1, 0);
		//NOTE this assignmet might not needed as a weird behavior is observed that
		//the row after the row which has its coffcient is zero is assumed to be zero
		//too
		UI_MainGrid->SetRowFill(2, 0);
		//remove the holder from it
		UI_ChildGrid->RemoveFromParent();
		UI_ChildGrid->ClearChildren();
		//clear the array except the first one for the slot
		Childs.SetNumZeroed(1);
	}
	else {
		//STEP three
		UI_MainGrid->SetRowFill(1, 4);
		UI_MainGrid->SetRowFill(2, 1);
		if (!UI_ChildGrid->GetParent()) {
			UI_MainGrid->AddChildToGrid(UI_ChildGrid, 1, 0);
		}
	}
	//STEP two
	if (!havingSlots()) {
		UI_TitleGrid->SetColumnFill(1, 0);
		//remove the border from it
		UI_ExpressionBlock->RemoveFromParent();
		UI_ExpressionBlock->ClearChildren();
	}
	else {
		UI_TitleGrid->SetColumnFill(1, 2);
		if (!UI_ExpressionBlock->GetParent()) {
			UI_TitleGrid->AddChildToGrid(UI_ExpressionBlock, 0, 1);
		}
	}
	//
	//Stage 2: Recalculate the space required
	//
	//When it is a simple block its size is the min between the text and the scale box
	FVector2D controlSize;
	FVector2D textSize = UI_BlockName->GetDesiredSize();
	controlSize.X = std::min<float>(textSize.X, 70);
	if (havingSlots()) {
		//get the size of the child first
		if (UI_Expression->GetChildrenCount()) {
			UCodeBlockBaseCPP* child = Cast<UCodeBlockBaseCPP>(UI_Expression->GetChildAt(0));
			child->Resize(); //recalculate the size
			child->SetRenderScale(FVector2D(scaleFactor * RenderTransform.Scale.X));
			//the child->size hold the size of the child slot
			//now we need to reestablish the cofficient
			//provided that [0]=textSize.X [1]=chid->size.X
			//we can get the R=[0]/[1] where R is the ratio [0] to [1]
			//provided R[0] always 1 so the formulae become [1]/[0]
			float AR = floor(child->size.X / textSize.X);
			//adjust the ratio to avoid zero and round up
			AR += AR < 1 ? 2 : 1;
			UI_TitleGrid->SetColumnFill(1, AR);
			//the size is become the sum of those
			controlSize.X *= AR+1;
		}
		else {
			//when it is nothing slotted in then the slot are triple of the size of text
			controlSize.X *= 3;
		}
		//When it is not slotted
		if(!(Slot&&Slot->Parent->GetClass()==UNamedSlot::StaticClass()))
			controlSize.X = std::max<float>(100, controlSize.X);
	}
	controlSize.Y = textSize.Y;
	if (havingChilds()) {
		//NOTE appearantly scroll box do not implement GetDesiredSize()
		//we calculate the height of the child slot (Note: extra 10px is for users to append)
		float slotHeight = 10;
		//so we calculate that by our own
		float resolvedSlotWidth = 0;
		for (int i = 1; i < Childs.Num(); i++) {
			UCodeBlockBaseCPP* element = Childs[i];
			if (element) {
				//inherit the render scale
				element->setFinalRenderScale(finalRenderScale);
				//ask child to resize itself
				element->Resize();
				//add its height into height
				slotHeight += element->size.Y;
				if (i > 1) {
					slotHeight += gapBetweenBlocks;
				}
				resolvedSlotWidth = std::max(resolvedSlotWidth, element->size.X);
			}
		}
		if (Childs.Num() < 1) {
			slotHeight = 100;
		}
		//---------------------------------------------------------
		//The height is computed based on these rules
		//first he borders have 2 units (one for top and one for bottom)
		//the content is based on the scroll box (min 100)
		//using those we can calculate the desirable cofficient for both
		//---------------------------------------------------------
		const float heightBorder = textSize.Y * 2;
		float AR = floor(slotHeight / heightBorder);
		//adjust the ratio to avoid zero and round up
		AR += AR < 1 ? 2 : 1;
		//multiply it with two because the base is not one but two
		AR *= 2;
		//set it
		UI_MainGrid->SetRowFill(1, AR);
		//the height is sum of those
		controlSize.Y = heightBorder + slotHeight;
		//---------------------------------------------------------
		//Width:
		// It is found that the left border become too thick when the control's width are too wide
		// So it is best for us to clamp those into at most 30px
		// Also if the slots are filled then we have to adjust it too
		//---------------------------------------------------------
		const float currentWidth = controlSize.X;
		float minWidth = 30 + resolvedSlotWidth;
		if (minWidth > currentWidth) {
			//extend the control when it is not enough
			controlSize.X = minWidth;
		}
		else {
			resolvedSlotWidth = currentWidth - 30;
		}
		//now lets calculate the ratio
		AR = floor(resolvedSlotWidth/30);
		//adjust the ratio to avoid zero and round up
		AR += AR < 1 ? 2 : 1;
		UI_ChildGrid->SetColumnFill(1, AR);	
	}
	controlSize.X += 25;
	UE_LOG(LogTemp, Warning, TEXT("%s: size=(%f,%f)"), *(GetName()), controlSize.X,controlSize.Y);
	setControlSize(controlSize);
}

bool UCodeBlockCPP::AddChildBlock(UCodeBlockBaseCPP* block,int at)
{
	//fail direct on NULL
	if (block == NULL)return false;
	//fail if the block do not accept any child
	if (!havingChilds())return false;
	//only allow statement,iteration and iterative
	if (block->Type == BlockType::Expression || block->Type == BlockType::Variable || block->Type == BlockType::Start) {
		return false;
	}
	//it must be detached from any slot
	if (block->GetParent()) {
		return false;
	}
	//
	//Adding a child to the slot involves four steps at least
	//1) Creating the canvas panel for holding the block
	//2) Attach the target into the newly created canvas panel
	//3) Attach the canvas panel into the list
	//4) Call Resize() to recalculate the sizes
	if (!Childs.Num()) {
		Childs.Add(NULL); //In case the child is added before the slot is filled in
	}
	//create the root
	UCanvasPanel* root = NewObject<UCanvasPanel>(this);
	//attach to it
	root->AddChildToCanvas(block);
	//set the offsets so it do not stick together
	if (Childs.Num() > 1) {
		Cast<UCanvasPanelSlot>(root->GetSlots()[0])->SetPosition(FVector2D(30, gapBetweenBlocks));
	}
	else {
		Cast<UCanvasPanelSlot>(root->GetSlots()[0])->SetPosition(FVector2D(30, 0));
	}
	//after that add this to the root
	if (at < 0) {
		UI_Childs->AddChild(root);
		//add those into the array too
		Childs.Push(block);

	}
	else {
		Childs.EmplaceAt(at, block);
		//InsertChildAt dont works so need some hackery here
		//copy the current array
		TArray<UWidget*> currentList = UI_Childs->GetAllChildren();
		//clear it
		UI_Childs->ClearChildren();
		//push the required one there
		currentList.EmplaceAt(at, root);
		//add all bacl
		for (UWidget* widget : currentList) {
			UI_Childs->AddChild(widget);
		}
	}
	//ask the layout engine to recalculate the size of this control
	rootResize();
	return true; //SUCCESS
}

bool UCodeBlockCPP::AddBlockIntoSlot(UCodeBlockBaseCPP* block)
{
	//fail direct on NULL
	if (block == NULL)return false;
	//fail if the block do not accept any child
	if (!havingSlots())return false;
	//dont allow statement,iteration and iterative
	if (block->Type == BlockType::Statement || block->Type == BlockType::Iteration || block->Type==BlockType::Conditional || block->Type == BlockType::Start) {
		return false;
	}
	//it must be detached from any slot
	if (block->GetParent()) {
		return false;
	}
	if ((Childs.Num() && Childs[0])||UI_Expression->GetChildrenCount()) {
		//something is slotted there
		return false;
	}
	//just add it as the children for the UI_Expression
	UI_Expression->AddChild(block);
	//adjust the block size as needed
	rootResize();
	return true;
}

bool UCodeBlockCPP::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
