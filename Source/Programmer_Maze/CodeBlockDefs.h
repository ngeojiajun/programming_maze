// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CodeBlockDefs.generated.h"

/*
*The type of the block
*/
UENUM()
enum BlockType {
	Statement,
	Conditional,
	Iteration,
	Expression,
	Variable,
	Constant,
};

/*
*Value type of the return value
*/
UENUM()
enum ValueType {
	Boolean,
	String,
	Integer,
	Void
};

/*
* The execution result of the block
*/
USTRUCT(BlueprintType)
struct FEvalResult {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	bool succeeded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	bool hasRetValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	TEnumAsByte<ValueType> retType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	int intVal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	bool boolVal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	FString strVal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eval Result")
	FString errorMsg;

	//Constructors
	FEvalResult();
	FEvalResult(bool result);
	FEvalResult(FString result);
	FEvalResult(int result);
	static FEvalResult AsVoidResult();
	static FEvalResult AsError(FString err);

	//operators
	bool operator ==(const FEvalResult& rhs) const;
	bool operator !=(const FEvalResult& rhs) const;
	bool operator !() const;

	//cast
	FEvalResult AsStringValue() const;
	FEvalResult AsBoolValue() const;
	FEvalResult AsIntValue() const;

};

UENUM(BlueprintType)
enum EExecPath
{
	Succeeded,
	Failed
};

UCLASS()
class UCodeBlockDefs :public UBlueprintFunctionLibrary {
public:
	GENERATED_BODY()
	/*
	* Branch the code to run between the succeeded and failed
	*/
	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	static void BranchExecution(FEvalResult in, TEnumAsByte<EExecPath>& Branches,FEvalResult& result /*duplicate*/,FString& error);
	/*
	* Produce a failed result block
	*/
	UFUNCTION(BlueprintPure)
	static FEvalResult makeFailedResult(FString reason = "");
};