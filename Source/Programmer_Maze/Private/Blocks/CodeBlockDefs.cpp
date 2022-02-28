// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#include "Blocks/CodeBlockDefs.h"

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
	return FEvalResult::AsError(reason);
}

FEvalResult::FEvalResult()
{
	succeeded = false;
	hasRetValue = false;
	intVal = -1;
	boolVal = false;
	strVal = "";
	retType = ValueType::Void;
	errorMsg = "";
}

FEvalResult::FEvalResult(bool result):FEvalResult()
{
	succeeded = true;
	hasRetValue = true;
	retType = ValueType::Boolean;
	boolVal = result;
}

FEvalResult::FEvalResult(FString result) :FEvalResult()
{
	succeeded = true;
	hasRetValue = true;
	retType = ValueType::String;
	strVal = result;
}

FEvalResult::FEvalResult(int result) : FEvalResult()
{
	succeeded = true;
	hasRetValue = true;
	retType = ValueType::Integer;
	intVal = result;
}

FEvalResult FEvalResult::AsVoidResult()
{
	FEvalResult ret;
	ret.succeeded = true;
	return ret;
}

FEvalResult FEvalResult::AsError(FString err)
{
	FEvalResult ret;
	ret.errorMsg = err;
	return ret;
}

bool FEvalResult::operator==(const FEvalResult& rhs) const
{
	if (retType == rhs.retType) {
		switch (retType) {
		case ValueType::Void:
			return true;
		case ValueType::Boolean:
			return boolVal == rhs.boolVal;
		case ValueType::Integer:
			return intVal == rhs.intVal;
		case ValueType::String:
			return strVal.Equals(rhs.strVal);
		}
		checkNoEntry();
		return false;
	}
	else {
		return false;
	}
}

bool FEvalResult::operator!=(const FEvalResult& rhs) const
{
	return !(*this==rhs);
}

bool FEvalResult::operator!() const
{
	if (!succeeded||retType == ValueType::Void) {
		return true;
	}
	switch (retType) {
	case ValueType::Boolean:
		return !boolVal;
	case ValueType::Integer:
		return !intVal;
	case ValueType::String:
		return strVal.Len()<=0;
	}
	checkNoEntry();
	return false;
}

FEvalResult FEvalResult::AsStringValue() const
{
	if (!succeeded || retType == ValueType::Void) {
		return FEvalResult::AsError(succeeded ? TEXT("Cannot cast from Void to String") : errorMsg);
	}
	switch (retType) {
	case ValueType::Boolean:
		return FEvalResult(FString(boolVal?TEXT("true"):TEXT("false")));
	case ValueType::Integer:
		return FEvalResult(FString::Printf(TEXT("%d"),intVal));
	case ValueType::String:
		return FEvalResult(strVal);
	}
	checkNoEntry();
	return FEvalResult::AsError(TEXT("Unreachable code reached"));
}

FEvalResult FEvalResult::AsBoolValue() const
{
	if (!succeeded || retType == ValueType::Void) {
		return FEvalResult::AsError(succeeded ? TEXT("Cannot cast from Void to Boolean") : errorMsg);
	}
	switch (retType) {
	case ValueType::Boolean:
		return FEvalResult(boolVal);
	case ValueType::Integer:
		return FEvalResult(intVal!=0);
	case ValueType::String:
		return FEvalResult(strVal.Len()!=0);
	}
	checkNoEntry();
	return FEvalResult::AsError(TEXT("Unreachable code reached"));
}

FEvalResult FEvalResult::AsIntValue() const
{
	if (!succeeded || retType == ValueType::Void) {
		return FEvalResult::AsError(succeeded ? TEXT("Cannot cast from Void to Int") : errorMsg);
	}
	switch (retType) {
	case ValueType::Boolean:
		return FEvalResult((boolVal ? 1 : 0));
	case ValueType::Integer:
		return FEvalResult(intVal);
	case ValueType::String: {
		if (strVal.IsNumeric()) {
			return FEvalResult(FCString::Atoi(*strVal));
		}
		else {
			FEvalResult::AsError(TEXT("Cannot cast from String to Int due to the invalid value"));
		}
	}
	}
	checkNoEntry();
	return FEvalResult::AsError(TEXT("Unreachable code reached"));
}
