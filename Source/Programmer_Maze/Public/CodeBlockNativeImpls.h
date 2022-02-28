// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "CodeBlockDefs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScriptExecutionContext.h"
#include "CodeBlockNativeImpls.generated.h"

//forward defs
class UCodeBlockCPP;
class UExpressionCodeBlockCPP;

/**
 * This library contain the implementation of some blocks in C++
 * It also includes some function which are implemented in C++ but not enough to
 * form an individual C++ class
 */
UCLASS()
class PROGRAMMER_MAZE_API UCodeBlockNativeImpls : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FEvalResult IfBlockImpl(UCodeBlockCPP* block,UPARAM(ref) FScriptExecutionContext& ctx);
	UFUNCTION(BlueprintCallable)
	static FEvalResult WhileBlockImpl(UCodeBlockCPP* block,UPARAM(ref) FScriptExecutionContext& ctx);
	UFUNCTION(BlueprintCallable)
	static FEvalResult StartBlockImpl(UCodeBlockCPP* block,UPARAM(ref) FScriptExecutionContext& ctx);
	//The base function for move action blocks
	UFUNCTION(BlueprintCallable)
	static FEvalResult MoveBlockImpl(UCodeBlockCPP* block, UPARAM(ref) FScriptExecutionContext& ctx, FVector movementVector);
	//Just for the use by stubs
	UFUNCTION(BlueprintCallable)
	static FEvalResult NopImpl(UCodeBlockCPP* block);
	//Fill the scroll panel using the classes provided the [classes] parameter
	UFUNCTION(BlueprintCallable)
	static void FillScrollPanel(UScrollBox* panel, TArray<UClass*> classes);
	UFUNCTION(BlueprintCallable)
	static void AddToScrollPanel(UScrollBox* panel, UCodeBlockBaseCPP* ptr);
private:
	static FEvalResult runAll(UCodeBlockCPP* block,UPARAM(ref) FScriptExecutionContext& ctx);
};
