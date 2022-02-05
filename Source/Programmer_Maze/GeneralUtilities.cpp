// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#include "GeneralUtilities.h"
#include "UObject/Object.h"

bool GeneralUtilities::insideRect(const FVector2D& pos, const FVector2D& size, const FVector2D& point)
{
	FVector2D another = pos + size;
	return (point.X >= pos.X && point.X <= another.X)&&
		(point.Y >= pos.Y && point.Y <= another.Y);
}

bool GeneralUtilities::insideGeometry(const FGeometry& pos, const FVector2D& point)
{
	return insideRect(pos.GetAbsolutePosition(),pos.GetLocalSize(),point);
}

void GeneralUtilities::Log(UObject* owner, const FString text)
{
#ifndef NO_DEBUG
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *(owner->GetName()),*text);
#endif
}

void GeneralUtilities::LogVector2D(UObject* owner, const FVector2D& vec, const FString tag)
{
	Log(owner, FString::Printf(TEXT("%s = (%.3f,%.3f)"), *tag, vec.X, vec.Y));
}

void GeneralUtilities::LogBoolean(UObject* owner, bool value, const FString tag)
{
	Log(owner, FString::Printf(TEXT("%s = %s"), *tag, value ? TEXT("true") : TEXT("false")));
}

