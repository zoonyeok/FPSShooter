// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZWeaponComponent.generated.h"

class AZBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSHOOTER_API UZWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<AZBaseWeapon>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName;

	UPROPERTY(EditDefaultsOnly, Category= "Animation")
	UAnimMontage* EquipAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category= "Animation")
	class USAnimInstance* AnimInstance;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	AZBaseWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<AZBaseWeapon*> Weapons;

	int32 CurrentWeaponIndex;
	bool EquipAnimInProgress;

	void SpawnWeapons();
	void AttachWeaponToSocket(AZBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
};