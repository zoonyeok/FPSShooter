// zoonyeok All Rights Reserved.

#include "UI/ZPlayerHUDWidget.h"
#include "Components/ZHealthComponent.h"
#include "ZWeaponComponent.h"
#include "ZBaseWeapon.h"

float UZPlayerHUDWidget::GetHealthPercent() const
{
	const auto HealthComponent = GetHealthComponent();
	if (HealthComponent == nullptr) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UZPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent =  GetWeaponComponent();
	if (WeaponComponent == nullptr) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UZPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent =  GetWeaponComponent();
	if (WeaponComponent == nullptr) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UZPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GetHealthComponent();
	return HealthComponent && !HealthComponent->IsDead();
}

bool UZPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == EName::NAME_Spectating;
}

UZWeaponComponent* UZPlayerHUDWidget::GetWeaponComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player == nullptr) return nullptr;
	
	const auto Component = Player->GetComponentByClass(UZWeaponComponent::StaticClass());
	const auto WeaponComponent =  Cast<UZWeaponComponent>(Component);
	return WeaponComponent;
}

UZHealthComponent* UZPlayerHUDWidget::GetHealthComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player == nullptr) return nullptr;
	
	const auto Component = Player->GetComponentByClass(UZHealthComponent::StaticClass());
	const auto HealthComponent =  Cast<UZHealthComponent>(Component);
	return HealthComponent;
}