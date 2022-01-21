// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZCoreTypes.h"
#include "ZHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSHOOTER_API UZHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZHealthComponent();
	
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category="Health")
	bool BIsDead() const { return FMath::IsNearlyZero(Health); }
	//bool BIsDead() const { return Health <= 0; }
	
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealUpdateTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float HealModifier;

	virtual void BeginPlay() override;

private:
	float Health;
	FTimerHandle HealGenTimerHandle;
	
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage,const class UDamageType* DamageType,class AController* InstigatedBy, AActor* DamageCauser);
	
	void HealthGenerate();
	void SetHealth(float NewHealth);
};
