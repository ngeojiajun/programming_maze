// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ConditionalWall.generated.h"

UCLASS()
class PROGRAMMER_MAZE_API AConditionalWall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AConditionalWall();

	//initialize the the real mat ptr
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	UMaterialInterface* baseMaterial;
	UPROPERTY(EditAnywhere)
	int groupID;
private:
	//listener for the characterStatusBroadcast
	UFUNCTION()
	void onCharacterStatusChanged(int group);
	UPROPERTY()
	UStaticMeshComponent* root;
	UPROPERTY()
	UMaterialInstanceDynamic* matPointer;
};