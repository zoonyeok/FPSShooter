// zoonyeok All Rights Reserved.

#include "Weapon/ZWeaponComponent.h"
#include "Weapon/ZBaseWeapon.h"
#include "Engine/World.h"
#include "GameFrameWork/Character.h"
#include "Animation/ZEquipFinishedAnimNotify.h"
#include "Animation/ZReloadFinishedAnimNotify.h"
#include "Player/SAnimInstance.h"
#include "Animation/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All , All)

constexpr static int32 WeaponNum = 2;

UZWeaponComponent::UZWeaponComponent()
	: WeaponEquipSocketName("WeaponSocket"),WeaponArmorySocketName("ArmorySocket"), CurrentWeapon(nullptr)
	,CurrentReloadAnimMontage(nullptr), CurrentWeaponIndex(0), EquipAnimInProgress(false), ReloadAnimInProgress(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UZWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our Character can hold only %d weapons items"), WeaponNum);

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UZWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for(auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UZWeaponComponent::SpawnWeapons()
{
	if (GetWorld() == nullptr) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) return;

	for(auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<AZBaseWeapon>(OneWeaponData.WeaponClass);
		if (Weapon == nullptr) continue;

		Weapon->OnClipEmpty.AddUObject(this, &UZWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	// TODO
	// 상자Mesh 만들고 거기에 담기
}

void UZWeaponComponent::AttachWeaponToSocket(AZBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (Weapon == nullptr || SceneComponent == nullptr) return;
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentTransformRules, SocketName);
}

void UZWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	
	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data){	//
		return Data.WeaponClass == CurrentWeapon->GetClass();											//
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon,Character->GetMesh(), WeaponEquipSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UZWeaponComponent::StartFire()
{
	if (CanFire() == false) return;
	CurrentWeapon->StartFire();
}

void UZWeaponComponent::StopFire()
{
	if (CurrentWeapon == nullptr) return;
	CurrentWeapon->StopFire();
}

void UZWeaponComponent::NextWeapon()
{
	if(!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
	PlayAnimMontage(EquipAnimMontage);
}

void UZWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) return;
	Character->PlayAnimMontage(Animation);
}

void UZWeaponComponent::InitAnimations()
{
	auto EquipFinishedNofify = AnimUtils::FindNotifyByClass<UZEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNofify)
	{
		EquipFinishedNofify->OnNotified.AddUObject(this,&UZWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UZReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UZWeaponComponent::OnReloadFinished);
	}
	
}

void UZWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr || MeshComponent != Character->GetMesh()) return;

	EquipAnimInProgress = false;
	//UE_LOG(LogWeaponComponent, Display, TEXT("Equip Finished"));
}

bool UZWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UZWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

void UZWeaponComponent::Reload()
{
	ChangeClip();
}

void UZWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr || MeshComponent != Character->GetMesh()) return;

	ReloadAnimInProgress = false;
	UE_LOG(LogWeaponComponent, Display, TEXT("Reload Finished"));
}

bool UZWeaponComponent::CanReload() const
{
	return CurrentWeapon//
		&& (!EquipAnimInProgress)//
		&& (!ReloadAnimInProgress)//
		&& CurrentWeapon->CanReload();
}

void UZWeaponComponent::OnEmptyClip()
{
	ChangeClip();
}

void UZWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UZWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;	
	}
	return false;
}

bool UZWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;	
	}
	return false;
}