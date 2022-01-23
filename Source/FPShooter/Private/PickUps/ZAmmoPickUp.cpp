// zoonyeok All Rights Reserved.


#include "PickUps/ZAmmoPickUp.h"
#include "Components/ZHealthComponent.h"
#include "Weapon/ZWeaponComponent.h"
#include "ZUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseAmmoPickUp, All, All);

AZAmmoPickUp::AZAmmoPickUp()
	: ClipsAmount(10)
{
}

bool AZAmmoPickUp::GivePickUpTo(APawn* PlayerPawn)
{
	const auto HealthComponent = ZUtils::GetZPlayerComponentByClass<UZHealthComponent>(PlayerPawn);
	if (HealthComponent == nullptr || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = ZUtils::GetZPlayerComponentByClass<UZWeaponComponent>(PlayerPawn);
	if (WeaponComponent == nullptr) return false;
	
	UE_LOG(LogBaseAmmoPickUp, Display, TEXT("Ammo was taken"));
	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}