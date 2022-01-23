// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PickUps/ZBasePickUp.h"
#include "ZAmmoPickUp.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API AZAmmoPickUp : public AZBasePickUp
{
	GENERATED_BODY()
public:
	AZAmmoPickUp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickUp", meta = (ClampMin = "1.0", ClampMax = "10"))
	int32 ClipsAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
	TSubclassOf<class AZBaseWeapon> WeaponType;
	
private:
	virtual bool GivePickUpTo(APawn* PlayerPawn);
};
