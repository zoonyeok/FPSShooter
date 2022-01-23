// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZCoreTypes.h"
#include "ZBaseWeapon.generated.h"

UCLASS()
class FPSHOOTER_API AZBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AZBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();

	void ChangeClip();
	bool CanReload() const;

	FWeaponUIData GetUIData() const { return UIData; }
	FAmmoData GetAmmoData() const { return CurrentAmmo; }

	bool TryToAddAmmo(int32 ClipsAmount);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance; // 1 unreal unit = 1cm

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;
	
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	
	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& out_Location, FRotator& out_Rotation) const;
	FVector GetMuzzleLocation() const;
	
	void MakeHit(FHitResult& OutHit, const FVector& TraceStart, const FVector& TraceEnd);

	void DecreaseAmmo();
	bool IsAmmoEmpty() const;
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;
	
	void LogAmmo();

private:
	FAmmoData CurrentAmmo;
};
