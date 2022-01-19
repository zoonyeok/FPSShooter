// zoonyeok All Rights Reserved.

#include "Weapon/ZWeaponComponent.h"
#include "Weapon/ZBaseWeapon.h"
#include "Engine/World.h"
#include "GameFrameWork/Character.h"
#include "Animation/ZEquipFinishedAnimNotify.h"
#include "Player/SAnimInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All , All)

UZWeaponComponent::UZWeaponComponent()
	: WeaponEquipSocketName("WeaponSocket"),WeaponArmorySocketName("ArmorySocket"), CurrentWeapon(nullptr)
	,CurrentWeaponIndex(0), EquipAnimInProgress(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UZWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

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

	for(auto WeaponClass : WeaponClasses)
	{
		auto Weapon = GetWorld()->SpawnActor<AZBaseWeapon>(WeaponClass);
		if (Weapon == nullptr) continue;

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
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character == nullptr) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	
	CurrentWeapon = Weapons[WeaponIndex];
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
	if (EquipAnimMontage == nullptr) return;
	
	const auto NotifyEvents = EquipAnimMontage->Notifies;
	for(auto NotifyEvent : NotifyEvents)
	{
		auto EquipFinishedNofify = Cast<UZEquipFinishedAnimNotify>(NotifyEvent.Notify);
		if(EquipFinishedNofify)
		{
			EquipFinishedNofify->OnNotified.AddUObject(this,&UZWeaponComponent::OnEquipFinished);
			break;
		}
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
	return CurrentWeapon && (!EquipAnimInProgress);
}

bool UZWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress;
}
