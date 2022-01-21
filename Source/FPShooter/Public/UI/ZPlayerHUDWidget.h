// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZCoreTypes.h"
#include "ZPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API UZPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category= "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category= "UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category= "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category= "UI")
	bool IsPlayerAlive() const;
	
	UFUNCTION(BlueprintCallable, Category= "UI")
	bool IsPlayerSpectating() const;

private:
	class UZWeaponComponent* GetWeaponComponent() const;
	class UZHealthComponent* GetHealthComponent() const;
};
