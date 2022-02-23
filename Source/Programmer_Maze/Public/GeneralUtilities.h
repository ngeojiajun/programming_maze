// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing
#include "CoreMinimal.h"
#include "Layout/Geometry.h"

//This file do not contain any unreal related stuffs
//Only contain some common utilities used across the project

DECLARE_LOG_CATEGORY_EXTERN(LogGameCore, Log, All);

//automatically expand parameters as the constant array
#define ARRAY_T(values) (values),(UE_ARRAY_COUNT(values))

namespace GeneralUtilities {
	//check weather the point is in the bounding box positioned at [pos] with size of [size]
	bool insideRect(const FVector2D& pos, const FVector2D& size, const FVector2D& point);
	//shorthand for the insideRect(.....), test is the point inside the geometry
	bool insideGeometry(const FGeometry& pos, const FVector2D& point);
	//Log a warning statement tagged by the UObject's name
	void Log(UObject* owner, const FString text);
	//Log() variant that log the Vector2D value under given tag
	void LogVector2D(UObject* owner, const FVector2D& vec, const FString tag = TEXT("Vector2D"));
	//Log() variant that log the boolean value under given tag
	void LogBoolean(UObject* owner, bool value, const FString tag = TEXT("Bool"));
	//Test is the value either one of the members which is specified inside the list
	template<typename T>
	bool either(T value, const T* target, const uint64_t no) {
		for (uint64_t i = 0; i < no; i++) {
			if (value == target[i])return true;
		}
		return false;
	}
	//Test is the value is neither one of the members which specified inside the list
	template<typename T>
	bool neither(T value, const T* target, const uint64_t no) {
		return !either(value, target, no);
	}
}