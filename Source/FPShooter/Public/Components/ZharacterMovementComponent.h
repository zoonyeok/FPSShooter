// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API UZharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Movement, meta = (ClampMin = "1.5", ClamMax = "10.0"))
	float RunModifier = 2.0f;
	
	virtual float GetMaxSpeed() const override;
};
