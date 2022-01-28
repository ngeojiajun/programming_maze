// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing


#include "CodeBlockNativeImpls.h"
#include "CodeBlockCPP.h"
#include "Kismet/KismetSystemLibrary.h"

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
