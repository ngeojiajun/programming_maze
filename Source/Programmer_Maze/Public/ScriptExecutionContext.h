// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "ScriptExecutionContext.generated.h"

class ABallPawn;
class AMazeMainGameMode;

//the execution context
USTRUCT(Blueprintable)
struct FScriptExecutionContext{
  GENERATED_BODY()
  //The pawn it working with
  UPROPERTY()
  ABallPawn* ptrPawn;
  //the gane ut us hosted
  UPROPERTY()
  AMazeMainGameMode* ptrGameMode;
  //this member holds the required information on the coroutine modoki
  TArray<void*> runtimeStack;
};
