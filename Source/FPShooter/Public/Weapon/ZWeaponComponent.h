// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZWeaponComponent.generated.h"


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AZBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

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

	void SpawnWeapons();
	void AttachWeaponToSocket(AZBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
};