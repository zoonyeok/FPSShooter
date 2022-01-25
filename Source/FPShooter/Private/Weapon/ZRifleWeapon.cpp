// zoonyeok All Rights Reserved.


#include "Weapon/ZRifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "SBaseCharacter.h"
#include "ZWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All)

AZRifleWeapon::AZRifleWeapon()
	: TimeBetweenShots(0.1f), BulletSpread(1.5f), DamageAmount(10.f), RecoilCoefficient(1.0f)
{
	WeaponFXComponent = CreateDefaultSubobject<UZWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void AZRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void AZRifleWeapon::StartFire()
{
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShortTimerHandle,this,&AZRifleWeapon::MakeShot, TimeBetweenShots,true);
	MakeShot();
}

void AZRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShortTimerHandle);
	SetMuzzleFXVisiblity(false);
}

void AZRifleWeapon::MakeShot()
{
	if (GetWorld() == nullptr || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	
	FVector TraceStart,TraceEnd;
	if (GetTraceData(TraceStart, TraceEnd) == false) 
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const bool bIsHit  = HitResult.bBlockingHit;

	const FVector TraceFXEnd = bIsHit ? HitResult.ImpactPoint : TraceEnd;
	if (bIsHit)
	{
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
		UE_LOG(LogRifleWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	}
	SpawnTraceFX(GetMuzzleLocation(), TraceFXEnd);
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

void AZRifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisiblity(true);
}

void AZRifleWeapon::SetMuzzleFXVisiblity(bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible,true);
	}
}

void AZRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TracFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX,TraceStart);
	if (TracFXComponent)
	{
		TracFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}