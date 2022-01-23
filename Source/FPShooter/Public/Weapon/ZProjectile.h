// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZProjectile.generated.h"

UCLASS()
class FPSHOOTER_API AZProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AZProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:

	UPROPERTY(VisibleAnywhere, Category= "Weapon")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category= "Weapon")
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool bDoFullDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float LifeSeconds;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	class UZWeaponFXComponent* WeaponFXComponent;
	
	virtual void BeginPlay() override;

private:
	
	FVector ShotDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
};
