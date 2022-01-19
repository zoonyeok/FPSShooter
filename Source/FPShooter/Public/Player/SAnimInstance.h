// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	USAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayDeathMontage();

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move | Axis", Meta = (AllowPrivateAccess = true))
	float Pitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move | Axis", Meta = (AllowPrivateAccess = true))
	float Roll;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move | Axis", Meta = (AllowPrivateAccess = true))
	float Yaw;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	bool bFullBody;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	FRotator RotationLastTick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	class APawn* Pawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
	class ASBaseCharacter* MyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Action, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Action, Meta = (AllowPrivateAccess = true))
	bool bIsAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Action, Meta = (AllowPrivateAccess = true))
	bool bShiftKeyDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Action, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;
	
};
