// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#include "CodeBlockDefs.h"

void UCodeBlockDefs::BranchExecution(FEvalResult in, TEnumAsByte<EExecPath>& Branches, FEvalResult& result, FString& error)
{
	result = in;
	if (in.succeeded) {
		Branches = EExecPath::Succeeded;
	}
	else {
		error = result.errorMsg.Len() ? result.errorMsg : TEXT("Unknown error happened");
		Branches = EExecPath::Failed;
	}
}

FEvalResult UCodeBlockDefs::makeFailedResult(FString reason)
{
	FEvalResult ret;
	ret.succeeded = false;
	ret.errorMsg = reason;
	return ret;
}

FEvalResult::FEvalResult()
{
	succeeded = false;
	hasRetValue = false;
	intVal = -1;
	boolVal = false;
	strVal = "";
	retType = Void;
	errorMsg = "";
}
