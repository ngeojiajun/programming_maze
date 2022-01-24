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

FLinearColor UCodeBlockBaseCPP::GetSlotColor() const
{
	if (Childs.Num() > 1) {
		return FLinearColor(0.0625, 0.0625, 0.0625);
	}
	else {
		return FLinearColor(0.639216, 0.639216, 0.639216);
	}
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
	//execute any bound handlers or run the 'default' eval handler
	FEvalResult evalResult=FEvalResult::AsError(
		FString::Printf(
			TEXT("The native handler did not overwrite the result of this class"),*(GetClass()->GetName())
		)
	);
	bool proceeded = (nativeEval) && nativeEval(this, evalResult);
	if(!proceeded){
		if (Type != BlockType::Constant) {
			return UCodeBlockDefs::makeFailedResult("Unimplemented");
		}
		else {
			if (Name.EqualToCaseIgnored(FText::FromString(TEXT("TRUE")))) {
				return FEvalResult(true);
			}
			else if (Name.EqualToCaseIgnored(FText::FromString(TEXT("FALSE")))) {
				return FEvalResult(false);
			}
			else {
				if (Name.IsNumeric()) {
					return FEvalResult(Name.ToString()).AsIntValue();
				}
				else {
					return FEvalResult(Name.ToString());
				}
			}
		}
	}
	else {
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
			parent->rootResize();
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
					parent->rootResize();
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
