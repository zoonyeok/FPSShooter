// zoonyeok All Rights Reserved.


#include "UI/ZPlayerHUDWidget.h"
#include "Components/ZHealthComponent.h"
#include "ZWeaponComponent.h"

float UZPlayerHUDWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player == nullptr) return 0.0f;

	const auto Component = Player->GetComponentByClass(UZHealthComponent::StaticClass());
	const auto HealthComponent =  Cast<UZHealthComponent>(Component);
	if (HealthComponent == nullptr) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UZPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
	const auto Player = GetOwningPlayerPawn();
	if (Player == nullptr) return false;

	const auto Component = Player->GetComponentByClass(UZWeaponComponent::StaticClass());
	const auto WeaponComponent =  Cast<UZWeaponComponent>(Component);
	if (WeaponComponent == nullptr) return false;

	return WeaponComponent->GetWeaponUIData(UIData);
}
