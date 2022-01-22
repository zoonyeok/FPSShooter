// zoonyeok All Rights Reserved.

#include "UI/ZPlayerHUDWidget.h"
#include "Components/ZHealthComponent.h"
#include "ZWeaponComponent.h"
#include "ZUtils.h"

float UZPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = ZUtils::GetZPlayerComponentByClass<UZHealthComponent>(GetOwningPlayerPawn());
	if (HealthComponent == nullptr) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UZPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = ZUtils::GetZPlayerComponentByClass<UZWeaponComponent>(GetOwningPlayerPawn());
	if (WeaponComponent == nullptr) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UZPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = ZUtils::GetZPlayerComponentByClass<UZWeaponComponent>(GetOwningPlayerPawn());
	if (WeaponComponent == nullptr) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UZPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = ZUtils::GetZPlayerComponentByClass<UZHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UZPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == EName::NAME_Spectating;
}