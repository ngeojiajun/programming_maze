// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "Maze/ButtonPawn.h"
#include "ButtonInstantiations.generated.h"

/*A example button which exits when the ball touches it*/
UCLASS()
class AGoalPawn :public AButtonPawn {
	GENERATED_BODY()
public:
	AGoalPawn();
protected:
	virtual void onBallHit() override;
};

/*A button which acts as a checkpoint when hit*/
UCLASS()
class ACheckpointPawn :public AButtonPawn {
	GENERATED_BODY()
public:
	ACheckpointPawn();
	/*the id of the checkpoint inside the map*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int checkpointId;
protected:
	virtual void onBallHit() override;
};

/*A button which "modifies" the maps when hit*/
UCLASS()
class AAnchorPawn :public AButtonPawn {
	GENERATED_BODY()
public:
	AAnchorPawn();
	//actually update the button to use the said material
	virtual void BeginPlay() override;
	//the base material used to render the button
	UPROPERTY(EditAnywhere)
	UMaterialInterface* baseMaterial;
	//the anchor group id
	UPROPERTY(EditAnywhere)
	int groupID;
protected:
	virtual void onBallHit() override;
};