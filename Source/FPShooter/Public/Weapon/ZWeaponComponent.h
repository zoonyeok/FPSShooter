// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZBaseWeapon.h"
#include "Components/ActorComponent.h"
#include "ZCoreTypes.h"
#include "ZWeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSHOOTER_API UZWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<AZBaseWeapon> WeaponType, int32 ClipsAmount);

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName;

	UPROPERTY(EditDefaultsOnly, Category= "Animation")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category= "Animation")
	class USAnimInstance* AnimInstance;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	AZBaseWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<AZBaseWeapon*> Weapons;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage;

	int32 CurrentWeaponIndex;
	bool EquipAnimInProgress;
	bool ReloadAnimInProgress;

	void SpawnWeapons();
	void AttachWeaponToSocket(AZBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	void OnEmptyClip(AZBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
};