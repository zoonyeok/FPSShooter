// zoonyeok All Rights Reserved.

#include "Components/ZHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShake.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All , All)

UZHealthComponent::UZHealthComponent()
	: MaxHealth(100.0f) , AutoHeal(true), HealUpdateTime(1.0f), HealDelay(3.0f), HealModifier(1.0f)
	,Health(0.0f)
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UZHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UZHealthComponent::OnTakeAnyDamage);
	}
}

void UZHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	//UE_LOG(LogHealthComponent,Display,TEXT("Damage: %f"),Damage);
	if (Damage <= 0.0f || IsDead() || (!GetWorld()) ) return;
	
	//Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	SetHealth(Health - Damage);
	// 피격시 힐 젠 중지
	GetWorld()->GetTimerManager().ClearTimer(HealGenTimerHandle);
	
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealGenTimerHandle, this, &UZHealthComponent::HealthGenerate,HealUpdateTime,true, HealDelay);
	}

	PlayCameraShake();
}

void UZHealthComponent::HealthGenerate()
{
	SetHealth(Health + HealModifier);

	// Health == MaxHealth
	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealGenTimerHandle);
	}
}

void UZHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth,0.0f,MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

bool UZHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsHealthFull() || IsDead()) return false;
	SetHealth(Health + HealthAmount);
	return true;
}

bool UZHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health,MaxHealth);
}

void UZHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (Player == nullptr) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}