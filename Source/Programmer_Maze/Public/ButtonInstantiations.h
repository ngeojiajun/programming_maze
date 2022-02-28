// (C) 2022 Jia Jun Ngeo All Rights Reserved. This source is attached with the submission of the final year project for the BSc Multimedia Computing

#pragma once

#include "CoreMinimal.h"
#include "ButtonPawn.h"
#include "ButtonInstantiations.generated.h"

/*A example button which exits when the ball touches it*/
UCLASS()
class AExitButtonPawn :public AButtonPawn {
	GENERATED_BODY()
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