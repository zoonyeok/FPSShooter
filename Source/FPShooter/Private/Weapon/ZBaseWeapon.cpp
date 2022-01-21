// zoonyeok All Rights Reserved.

#include "Weapon/ZBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFrameWork/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

AZBaseWeapon::AZBaseWeapon()
	: MuzzleSocketName("MuzzleSocket"), TraceMaxDistance(1500.f), DefaultAmmo({15,10,false} )
{
	PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

void AZBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count error"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count error"));
	CurrentAmmo = DefaultAmmo;
}

void AZBaseWeapon::StartFire()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("BaseWeapon!"));
}

void AZBaseWeapon::StopFire()
{
}

void AZBaseWeapon::MakeShot()
{
	if (GetWorld() == nullptr || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	
	FVector TraceStart,TraceEnd;
	if (GetTraceData(TraceStart, TraceEnd) == false) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		//MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), GetMuzzleLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Green, false, 5.0f, 0, 2.0f);
		UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
	}

	DecreaseAmmo();
}

APlayerController* AZBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (Player == nullptr) return nullptr;

	return Player->GetController<APlayerController>();
}

bool AZBaseWeapon::GetPlayerViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	const auto Controller = GetPlayerController();
	if (Controller == nullptr) return false;
	Controller->GetPlayerViewPoint(out_Location, out_Rotation);
	return true;
}

FVector AZBaseWeapon::GetMuzzleLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool AZBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (GetPlayerViewPoint(ViewLocation, ViewRotation) == false) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection =ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void AZBaseWeapon::MakeHit(FHitResult& OutHit, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (GetWorld() == nullptr) return;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

void AZBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}
	CurrentAmmo.Bullets--;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast();
	}
}

bool AZBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool AZBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void AZBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("---------Change Clip---------"));
}

bool AZBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void AZBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}
