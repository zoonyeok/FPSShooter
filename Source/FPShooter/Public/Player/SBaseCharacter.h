// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZBaseWeapon.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "SBaseCharacter.generated.h"

class UZHealthComponent;
class UZWeaponComponent;
UCLASS()
class FPSHOOTER_API ASBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	UZHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	class UTextRenderComponent* HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AZBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UZWeaponComponent* WeaponComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category= Movement)
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category= Movement)
	float GetMovementDirection() const;
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USAnimInstance* AnimInstance;
	
	UPROPERTY()
	bool bShiftDown;
	bool IsMovingForward;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnStartRunning();
	void OnStopRunning();

	void OnDeath();
	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

public:
	bool GetShiftDown () const { return bShiftDown; }
	void SetCameraRotation(const FRotator& Direction);
};