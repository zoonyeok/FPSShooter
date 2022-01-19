// zoonyeok All Rights Reserved.

#include "Weapon/ZLauncherWeapon.h"
#include "Weapon/ZProjectile.h"
#include "Kismet/GameplayStatics.h"

AZLauncherWeapon::AZLauncherWeapon()
{
}

void AZLauncherWeapon::StartFire()
{
	MakeShot();
}

void AZLauncherWeapon::MakeShot()
{
	if (GetWorld() == nullptr || IsAmmoEmpty()) return;
	
	FVector TraceStart,TraceEnd;
	if (GetTraceData(TraceStart, TraceEnd) == false) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleLocation()).GetSafeNormal();
	
	const FTransform SpawnTransform(FRotator::ZeroRotator,GetMuzzleLocation());
	AZProjectile* Projectile = GetWorld()->SpawnActorDeferred<AZProjectile>(ProjectileClass,SpawnTransform);
	if(Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
	// **Temp Spawn Projectile
	// auto TempProjectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),ProjectileClass,SpawnTransform);
	// UGameplayStatics::FinishSpawningActor(Projectile,SpawnTransform);
}
