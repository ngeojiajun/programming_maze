// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#include "CoreMinimal.h"
#include "Layout/Geometry.h"

//This file do not contain any unreal related stuffs
//Only contain some common utilities used across the project

namespace GeneralUtilities {
	//check weather the point is in the bounding box positioned at [pos] with size of [size]
	bool insideRect(const FVector2D& pos, const FVector2D& size, const FVector2D& point);
	//shorthand for the insideRect(.....)
	bool insideGeometry(const FGeometry& pos, const FVector2D& point);
	void Log(UObject* owner, const FString text);
	void LogVector2D(UObject* owner, const FVector2D& vec, const FString tag = TEXT("Vector2D"));
	void LogBoolean(UObject* owner, bool value, const FString tag = TEXT("Bool"));
}