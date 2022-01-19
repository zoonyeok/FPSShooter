// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZProjectile.h"
#include "Weapon/ZBaseWeapon.h"
#include "ZLauncherWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API AZLauncherWeapon : public AZBaseWeapon
{
	GENERATED_BODY()
public:
	AZLauncherWeapon();
	virtual void StartFire() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AZProjectile> ProjectileClass;
	
	virtual void MakeShot() override;

private:
	FTimerHandle ShortTimerHandle;
};
