// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "CodeBlockNativeImpls.h"
#include "CodeBlockCPP.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GeneralUtilities.h"

FEvalResult UCodeBlockNativeImpls::IfBlockImpl(UCodeBlockCPP* block)
{
	//If code run the code if and only if the block is evaluated to true
	//lets evaluate that first
	FEvalResult result=FEvalResult::AsVoidResult();
	//get the slot from first child
	UCodeBlockBaseCPP* slot = block->Childs[0];
	if (!slot) {
		return FEvalResult::AsError(TEXT("Cannot evaluate the block due to one of its slot are empty"));
	}
	//evaluate it
	result = slot->eval();
	if (!!(result.AsBoolValue())) {
		result = runAll(block);
	}
	//return it so the error propogates to its parent
	return result;
}

FEvalResult UCodeBlockNativeImpls::WhileBlockImpl(UCodeBlockCPP* block)
{
	//while block is similar with the if block but it runs until the expression become false
	//lets evaluate that first
	FEvalResult result = FEvalResult::AsVoidResult();
	//get the slot from first child
	UCodeBlockBaseCPP* slot = block->Childs[0];
	if (!slot) {
		return FEvalResult::AsError(TEXT("Cannot evaluate the block due to one of its slot are empty"));
	}
	//evaluate it
	//the first one catchs any evaluation error in runAll
	//second part is the normal eval and check
	while (result.succeeded && (result = slot->eval(), !!(result.AsBoolValue()))) {
		result = runAll(block);
	}
	//return it so the error propogates to its parent
	return result;
}

FEvalResult UCodeBlockNativeImpls::StartBlockImpl(UCodeBlockCPP* block)
{
	//start block do not have any special meaning, just forward it to runAll
	return runAll(block);
}

FEvalResult UCodeBlockNativeImpls::ExitBlockImpl(UCodeBlockCPP* block)
{
	//this simply quit the game
	UKismetSystemLibrary::QuitGame(block, NULL, EQuitPreference::Quit, false);
	return FEvalResult::AsVoidResult(); //never return
}

FEvalResult UCodeBlockNativeImpls::NopImpl(UCodeBlockCPP* block)
{
	//do nothing but return Void/
	return FEvalResult::AsVoidResult();
}

void UCodeBlockNativeImpls::FillScrollPanel(UScrollBox* panel, TArray<UClass*> classes)
{
	if (!panel) return;
	//HACK: For some reason TArray<TSubclassOf<UCodeBlockBaseCPP>> will cause MSVC to fail
	//so we take the UClass as alternative but we enforce it inside to code
	for (int i = 0; i < classes.Num(); i++) {
		TSubclassOf<UCodeBlockBaseCPP> targetClass = classes[i];
		check(targetClass); //will crash the entire application if the class passed in is not a subclass of the UCodeBlockBaseCPP
		//Be noted that some block have some special behavior such as the the unamed variable and constant should not be cloned directly
		//Due to this it is not included in this call but will added manually whenever needed
		GeneralUtilities::Log(panel, FString::Printf(TEXT("Adding block named=%s"),*(targetClass->GetFName().ToString())));
		//construct the block
		UCodeBlockBaseCPP* block = NewObject<UCodeBlockBaseCPP>(panel, targetClass);
		//mark it as template block
		block->Template = true;
		AddToScrollPanel(panel, block);
	}
}

void UCodeBlockNativeImpls::AddToScrollPanel(UScrollBox* panel, UCodeBlockBaseCPP* block)
{
	if (!panel || !block)return;
	//we mimic the bahavior of the UCodeBlock::AddChildBlock(....,-1)
	//create the root
	UCanvasPanel* root = NewObject<UCanvasPanel>(panel);
	//attach to it
	root->AddChildToCanvas(block);
	//append to the panel
	UPanelSlot* slot = panel->AddChild(root);
	UScrollBoxSlot* box=Cast<UScrollBoxSlot>(slot);
	box->SetPadding(FMargin(0, 0, 0, 15));
	//set its render scale to a little bit larger
	block->setFinalRenderScale(FVector2D(1.25));
}

FEvalResult UCodeBlockNativeImpls::runAll(UCodeBlockCPP* block)
{
	//This is just a utility function that simply run all the child blocks
	//lets evaluate that first
	FEvalResult result = FEvalResult::AsVoidResult();
	//stop code execution when the exception happened
	for (int i = 0; i < block->Childs.Num() && result.succeeded; i++) {
		result = block->Childs[i]->eval();
	}
	return result;
}
