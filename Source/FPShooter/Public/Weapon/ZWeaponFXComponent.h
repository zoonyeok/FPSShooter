// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZCoreTypes.h"
#include "ZWeaponFXComponent.generated.h"

class UPhysicalMaterial;
class UNiagaraSystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSHOOTER_API UZWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UZWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	FImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};
