// zoonyeok All Rights Reserved.


#include "Weapon/ZRifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "SBaseCharacter.h"
#include "GameFrameWork/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All)

AZRifleWeapon::AZRifleWeapon()
	: TimeBetweenShots(0.1f), BulletSpread(1.5f), DamageAmount(10.f), RecoilCoefficient(1.0f)
{
}

void AZRifleWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShortTimerHandle,this,&AZRifleWeapon::MakeShot, TimeBetweenShots,true);
}

void AZRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShortTimerHandle);
}

void AZRifleWeapon::MakeShot()
{
	if (GetWorld() == nullptr || IsAmmoEmpty()) return;
	
	FVector TraceStart,TraceEnd;
	if (GetTraceData(TraceStart, TraceEnd) == false) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	
	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Green, false, 5.0f, 0, 2.0f);
		UE_LOG(LogRifleWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

bool AZRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (GetPlayerViewPoint(ViewLocation, ViewRotation) == false) return false;
	
	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const auto ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	//Set Recoil
	const auto PlayerController = GetPlayerController();
	if (PlayerController == nullptr) return false;
	PlayerController->SetControlRotation(FMath::Lerp(PlayerController->GetControlRotation(),ShootDirection.Rotation(),RecoilCoefficient));
	
	return true;
}

void AZRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (DamagedActor == nullptr) return;

	//Head Shot
	if(HitResult.BoneName == "b_head")
	{
		DamagedActor->TakeDamage(DamageAmount * 2, FDamageEvent(), GetPlayerController(), this);
	}
	else
	{
		DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	}
}