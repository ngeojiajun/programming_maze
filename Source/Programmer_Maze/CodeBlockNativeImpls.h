// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "CodeBlockDefs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CodeBlockNativeImpls.generated.h"

//forward defs
class UCodeBlockCPP;
class UExpressionCodeBlockCPP;

/**
 * This library contain the implementation of some blocks in C++
 */
UCLASS()
class PROGRAMMER_MAZE_API UCodeBlockNativeImpls : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FEvalResult IfBlockImpl(UCodeBlockCPP* block);
	UFUNCTION(BlueprintCallable)
	static FEvalResult WhileBlockImpl(UCodeBlockCPP* block);
	UFUNCTION(BlueprintCallable)
	static FEvalResult StartBlockImpl(UCodeBlockCPP* block);
	UFUNCTION(BlueprintCallable)
	static FEvalResult ExitBlockImpl(UCodeBlockCPP* block);
private:
	static FEvalResult runAll(UCodeBlockCPP* block);
};
