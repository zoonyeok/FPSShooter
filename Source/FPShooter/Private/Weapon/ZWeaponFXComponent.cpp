// zoonyeok All Rights Reserved.


#include "Weapon/ZWeaponFXComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

UZWeaponFXComponent::UZWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UZWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;
	
	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

	/* Niagara */ 
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		ImpactData.NiagaraEffect,		//
		Hit.ImpactPoint,				//
		Hit.ImpactNormal.Rotation());   //

	/* Decal */
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
		ImpactData.DecalData.Material,			//
		ImpactData.DecalData.Size,				//
		Hit.ImpactPoint,						//
		(Hit.ImpactNormal * -1.0f).Rotation());	//

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeTime);
	}
}