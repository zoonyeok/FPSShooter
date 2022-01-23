// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/ZBasePickUp.h"
#include "ZHealthPickUp.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API AZHealthPickUp : public AZBasePickUp
{
	GENERATED_BODY()
public:
	AZHealthPickUp();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float HealthAmount;
	
private:
	virtual bool GivePickUpTo(APawn* PlayerPawn);
};
