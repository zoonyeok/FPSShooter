// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ZBaseWeapon.h"
#include "ZRifleWeapon.generated.h"

class UZWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class FPSHOOTER_API AZRifleWeapon : public AZBaseWeapon
{
	GENERATED_BODY()
public:
	AZRifleWeapon();
	virtual void StartFire() override;
	virtual void StopFire() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RecoilCoefficient;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UZWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
	

private:
	FTimerHandle ShortTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;
	
	void MakeDamage(const FHitResult& HitResult);
	void InitMuzzleFX();
	void SetMuzzleFXVisiblity(bool Visible);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
