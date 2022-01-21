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
	bool GetWeaponUIData(FWeaponUIData& UIData) const; 
};
