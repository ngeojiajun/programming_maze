// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "CodeBlockNativeImpls.h"
#include "CodeBlockCPP.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GeneralUtilities.h"
//we need this because we need use void* as scalar
#pragma warning( disable : 4302 4311 4312)
/*
*State:
* result : Far<FEvalResult>
* insideBlock:Near<int>
* Push:
* result:Far<FEvalResult>
*/
FEvalResult UCodeBlockNativeImpls::IfBlockImpl(UCodeBlockCPP* block,FScriptExecutionContext& ctx)
{
	bool shouldRestore = ctx.contextRestore;
	//If code run the code if and only if the block is evaluated to true
	//lets evaluate that first
	FEvalResult result=FEvalResult::AsVoidResult();
	//get the slot from first child
	UCodeBlockBaseCPP* slot = block->Childs[0];
	if (!slot) {
		result = FEvalResult::AsError(TEXT("Cannot evaluate the block due to one of its slot are empty"));
		PUSH_FAR_VALUE(ctx, FEvalResult, result);
		return FEvalResult::AsError(TEXT("Cannot evaluate the block due to one of its slot are empty"));
	}
	if (shouldRestore) {
		//restore the values as we need
		//-------
		//result	|
		//...	\/
		TAKE_FAR_VALUE_ASSIGN(ctx, FEvalResult, result);
	}
	else {
		//evaluate it
		result = slot->eval(ctx);
	}
	if (!shouldRestore) {
		//push it when there are nothing unwinding
		PUSH_FAR_VALUE(ctx, FEvalResult, result);
	}
	shouldRestore = false;
	if (!!(result.AsBoolValue())) {
		result = runAll(block,ctx);
		//check if it is yielding up, if not clean up
		if (!ctx.yielding) {
			//ret value
			TAKE_FAR_VALUE_ASSIGN(ctx, FEvalResult, result);
			DESTROY_FAR_VALUE(ctx, FEvalResult);
			//
			DESTROY_FAR_VALUE(ctx, FEvalResult);
		}
		else {
			return FEvalResult::AsVoidResult(); //yielding up
		}
	}
	PUSH_FAR_VALUE(ctx, FEvalResult, result);
	//return it so the error propogates to its parent
	return result;
}
/*
*State:
* result : Far<FEvalResult>
* insideLoop:Near<int>
* result:Far<FEvalResult>
*/
FEvalResult UCodeBlockNativeImpls::WhileBlockImpl(UCodeBlockCPP* block,FScriptExecutionContext& ctx)
{
	bool shouldRestore = ctx.contextRestore;
	//while block is similar with the if block but it runs until the expression become false
	//lets evaluate that first
	FEvalResult result = FEvalResult::AsVoidResult();
	//get the slot from first child
	UCodeBlockBaseCPP* slot = block->Childs[0];
	if (!slot) {
		check(!shouldRestore);
		result = FEvalResult::AsError(TEXT("Cannot evaluate the block due to one of its slot are empty"));
		PUSH_FAR_VALUE(ctx, FEvalResult, result);
		return FEvalResult::AsError(TEXT("Cannot evaluate the block due to one of its slot are empty"));
	}
	//state:
	// -1:quit
	// 0:eval condition
	// 1:run
	//
	int state = 0;
	if (shouldRestore) {
		//restore the values as we need
		//-------
		//state	|
		//result	|
		//...	\/
		TAKE_NEAR_VALUE_ASSIGN(ctx, int, state);
		TAKE_FAR_VALUE_ASSIGN(ctx, FEvalResult, result);
	}
	do {
		switch (state) {
		case 0: {
			//no state storing here because the expression cannot be latern function
			state = result.succeeded && (result = slot->eval(ctx), !!(result.AsBoolValue())) ? 1 : -1;
			continue;
		}
		case 1: {
			state = 0;
			if (!shouldRestore) {
				//dont brother to mangle the stack because it should never happens
				//save the current status
				PUSH_NEAR_VALUE(ctx, state);
				PUSH_FAR_VALUE(ctx, FEvalResult, result);
			}
			shouldRestore = false;
			result = runAll(block, ctx);
			//check if it is yielding up, if not clean up
			if (!ctx.yielding) {
				//ret value
				TAKE_FAR_VALUE_ASSIGN(ctx, FEvalResult, result);
				DESTROY_FAR_VALUE(ctx, FEvalResult);
				//
				DESTROY_FAR_VALUE(ctx, FEvalResult);
				DESTROY_NEAR_VALUE(ctx);
			}
			else {
				return FEvalResult::AsVoidResult(); //yielding up
			}
			continue;
		}
		default: {
			checkNoEntry();
			break;
		}
		}
	} 
	while (state != -1);
	//return it so the error propogates to its parent
	if (!ctx.yielding) {
		PUSH_FAR_VALUE(ctx, FEvalResult, result);
	}
	return result;
}

/*
*State:none
*/
FEvalResult UCodeBlockNativeImpls::StartBlockImpl(UCodeBlockCPP* block,FScriptExecutionContext& ctx)
{
	//start block do not have any special meaning, just forward it to runAll
	ctx.sp = 0; //reset the SP
	if (ctx.yielding)return FEvalResult::AsVoidResult();
	return runAll(block,ctx);
}

/*
*State:none
*/
FEvalResult UCodeBlockNativeImpls::ExitBlockImpl(UCodeBlockCPP* block,FScriptExecutionContext& ctx)
{
	//this simply quit the game
	UKismetSystemLibrary::QuitGame(block, NULL, EQuitPreference::Quit, false);
	FEvalResult result = FEvalResult::AsVoidResult();
	PUSH_FAR_VALUE(ctx, FEvalResult, result);
	return FEvalResult::AsVoidResult(); //never return
}

/*
*State:none
*/
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
	box->SetPadding(FMargin(0, 0, 15, 15));
	//make it right aligned
	box->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	//set its render scale to a little bit larger
	block->setFinalRenderScale(FVector2D(1.5));
}

/*
*State:
* result : Far<FEvalResult>
* i : Near<int>
* result:Far<FEvalResult>
*/
FEvalResult UCodeBlockNativeImpls::runAll(UCodeBlockCPP* block,FScriptExecutionContext& ctx)
{
	//This is just a utility function that simply run all the child blocks
	//lets evaluate that first
	bool shouldRestore = ctx.contextRestore;
	int i = 0;
	FEvalResult result = FEvalResult::AsVoidResult();
	if (shouldRestore) {
		//restore the values as we need
		//-------
		//i		|
		//result	|
		//...	\/
		TAKE_NEAR_VALUE_ASSIGN(ctx, int, i);
		TAKE_FAR_VALUE_ASSIGN(ctx, FEvalResult, result);
	}
	//stop code execution when the exception happened
	for (; i < block->Childs.Num() && result.succeeded; i++) {
		if (!shouldRestore) {
			PUSH_NEAR_VALUE(ctx, i);
			PUSH_FAR_VALUE(ctx, FEvalResult, result);
		}
		result = block->Childs[i]->eval(ctx);
		//check if it is yielding up, if not clean up
		if (!ctx.yielding) {
			//ret value
			TAKE_FAR_VALUE_ASSIGN(ctx, FEvalResult, result);
			DESTROY_FAR_VALUE(ctx, FEvalResult);
			//
			DESTROY_FAR_VALUE(ctx, FEvalResult);
			DESTROY_NEAR_VALUE(ctx);
		}
		else {
			return FEvalResult::AsVoidResult(); //yielding up
		}
	}
	PUSH_FAR_VALUE(ctx, FEvalResult, result);
	return result;
}
