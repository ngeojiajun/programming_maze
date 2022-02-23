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
  //should the code restore the data from the context
  bool contextRestore;
  bool yielding;
  //current sp that the engine is restoring
  int sp;
};

#define PUSH_NEAR_VALUE(ctx,v) ctx.runtimeStack.Push((void*)v)
#define PUSH_FAR_VALUE(ctx,t,v) ctx.runtimeStack.Push(new t(v))
#define TAKE_NEAR_VALUE(ctx,t,name) t name=(t)ctx.runtimeStack[ctx.sp++]
#define TAKE_FAR_VALUE(ctx,t,name) t name;{t* tmp=(t*)ctx.runtimeStack[ctx.sp++]; name=*tmp;}
#define TAKE_NEAR_VALUE_ASSIGN(ctx,t,name) name=(t)ctx.runtimeStack[ctx.sp++]
#define TAKE_FAR_VALUE_ASSIGN(ctx,t,name) {t* tmp=(t*)ctx.runtimeStack[ctx.sp++]; name=*tmp;}
#define DESTROY_NEAR_VALUE(ctx) ctx.runtimeStack.Pop();ctx.sp--;
#define DESTROY_FAR_VALUE(ctx,t) {t* tmp=(t*)ctx.runtimeStack.Pop(); delete tmp;ctx.sp--;}