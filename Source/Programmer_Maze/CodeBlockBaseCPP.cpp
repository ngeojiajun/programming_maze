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
		return DEFINE_DIRECT_COLOR(176, 179, 0);
	case BlockType::Variable:
		return DEFINE_DIRECT_COLOR(24, 179, 0);
	case BlockType::Expression:
		return DEFINE_DIRECT_COLOR(25, 0, 138);
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
	//this->SetRenderScale(finalRenderScale);
	Resize();
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
		return false;
	default:
		return true;
	}
}
