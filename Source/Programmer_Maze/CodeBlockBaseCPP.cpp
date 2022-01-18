// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "CodeBlockBaseCPP.h"
#include "Components/ScaleBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/GridPanel.h"
#include "Components/NamedSlot.h"
#include "Components/GridSlot.h"
#include "Components/Border.h"

UCodeBlockBaseCPP::UCodeBlockBaseCPP(const FObjectInitializer& initializer) :UUserWidget(initializer) {
	Name = FText::FromString(TEXT("Eval"));
	Type = BlockType::Statement;
	finalRenderScale = FVector2D(1);
	Childs.Empty();
}

FLinearColor UCodeBlockBaseCPP::GetCurrentBGColor() const
{
#define DEFINE_DIRECT_COLOR(r,g,b) FLinearColor((r)/255.0f,(g)/255.0f,(b)/255.0f);
	switch (Type) {
	case BlockType::Statement:
		return DEFINE_DIRECT_COLOR(0, 20, 255);
	case BlockType::Conditional:
		return DEFINE_DIRECT_COLOR(148, 0, 156);
	case BlockType::Iteration:
		return DEFINE_DIRECT_COLOR(95, 97, 0);
	case BlockType::Variable:
		return DEFINE_DIRECT_COLOR(24, 179, 0);
	case BlockType::Expression:
		return DEFINE_DIRECT_COLOR(25, 0, 138);
	case BlockType::Constant:
		return DEFINE_DIRECT_COLOR(161, 0, 11);
	}
	//This should never happen
	return FColor(0,0,0); //Fuck you
#undef DEFINE_DIRECT_COLOR
}

void UCodeBlockBaseCPP::Resize()
{
}

void UCodeBlockBaseCPP::setFinalRenderScale(FVector2D scale)
{
	finalRenderScale = scale;
	//trigger the set event
	Resize();
}

UCodeBlockBaseCPP* UCodeBlockBaseCPP::clone_Implementation()
{
	unimplemented();
	return nullptr;
}

FEvalResult UCodeBlockBaseCPP::eval_Implementation()
{
	if (Type != BlockType::Constant) {
		return UCodeBlockDefs::makeFailedResult("Unimplemented");
	}
	else {
		//test if it is boolean
		FEvalResult evalResult;
		evalResult.hasRetValue = true;
		evalResult.succeeded = true;
		if (Name.EqualToCaseIgnored(FText::FromString(TEXT("TRUE")))) {
			evalResult.boolVal = true;
			evalResult.intVal = 1;
			evalResult.strVal = "true";
		}
		else if (Name.EqualToCaseIgnored(FText::FromString(TEXT("FALSE")))) {
			evalResult.boolVal = false;
			evalResult.intVal = 0;
			evalResult.strVal = "false";
		}
		else {
			evalResult.boolVal = Name.ToString().Len() > 0;
			evalResult.strVal = Name.ToString();
			if (Name.IsNumeric()) {
				evalResult.intVal = FCString::Atoi(*(Name.ToString()));
			}
		}
		return evalResult;
	}
}

void UCodeBlockBaseCPP::NativeConstruct()
{
	ForceLayoutPrepass();
	Resize();
}

void UCodeBlockBaseCPP::setControlSize(const FVector2D newSize)
{
	FVector2D finalSize = FVector2D(newSize.X * finalRenderScale.X, newSize.Y * finalRenderScale.Y);
	this->size = finalSize;
	if (Slot) {
		UCanvasPanelSlot* Panel = Cast<UCanvasPanelSlot>(Slot);
		if (Panel) {
			Panel->SetSize(finalSize);
			return;
		}
	}
}

bool UCodeBlockBaseCPP::havingChilds() const
{
	switch (Type) {
	case BlockType::Conditional:
	case BlockType::Iteration:
		return true;
	default:
		return false;
	}
}

bool UCodeBlockBaseCPP::havingSlots() const
{
	switch (Type) {
	case BlockType::Statement:
	case BlockType::Variable:
	case BlockType::Constant:
		return false;
	default:
		return true;
	}
}

void UCodeBlockBaseCPP::rootResize()
{
	//This function only work for the controls slotted under the child slot
	if (Slot) {
		//directly cast this to UCanvasPanelSlot to determine weather it is added through
		//the child slot
		//We know that the outer oubject for the Canvas Panel is always instance of
		//UCodeBlockCPP
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Slot);
		UCodeBlockBaseCPP* parent = slot ? Cast<UCodeBlockBaseCPP>(slot->Parent->GetOuter()) : NULL;
		if (parent) {
			parent->Resize();
		}
		else {
			//if that failed lets try the slot mechanism
			//the slot mechanism places the child element (this) inside the widget
			//typed NamedSlot
			//but this do not have any specific meanings to attempt to direct cast to UNamedSlotPanel
			if (Slot->Parent) { //if the slot dont have parent then just ignore it
				UObject* _parent = Slot->Parent->GetOuter(); //get the outer object that own the instance
				while (_parent) {
					//attempt a cast to the UCodeBlockBaseCPP and see weather is that possible
					//if yes then break it off since we get our parent
					parent = Cast<UCodeBlockBaseCPP>(_parent);
					if (parent)break;
					_parent = _parent->GetOuter();
				}
				if (parent) {
					parent->Resize();
				}
				else {
					Resize();
				}
			}
			Resize();
		}
	}
	else {
		Resize();
	}
}
