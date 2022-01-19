// zoonyeok All Rights Reserved.


#include "Player/SAnimInstance.h"
#include "Player/SBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnimInstance, All, All)

USAnimInstance::USAnimInstance()
	: Speed(0.f), Pitch(0.f), Roll(0.f), Yaw(0.f), YawDelta(0.f), 
	bIsInAir(false), bIsAccelerating(false), bFullBody(false), RotationLastTick(FRotator::ZeroRotator), Direction(0.f),
	bIsAttack(false), bShiftKeyDown(false)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("AnimMontage'/Game/Player/Animation/AM_Death.AM_Death'"));
	if (DEATH_MONTAGE.Succeeded())
	{
		DeathMontage = DEATH_MONTAGE.Object;
	}

}

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (::IsValid(Pawn))
		{
			MyCharacter = Cast<ASBaseCharacter>(Pawn);
		}
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Pawn)) return;
	
	Speed = Pawn->GetVelocity().Size();
		
	if (!IsValid(MyCharacter)) return;
		
	// Set bIsinAir
	bIsInAir = MyCharacter->GetMovementComponent()->IsFalling();
	
	float Acceleration = MyCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size();
	if (Acceleration > 0.f)
	{
		bIsAccelerating = true;
	}

	// Set YawDelta
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick,MyCharacter->GetActorRotation());
	float DeletaYaw = DeltaRotator.Yaw;
	float Target = (DeletaYaw / GetWorld()->DeltaTimeSeconds) / 7.0f;
	YawDelta = UKismetMathLibrary::FInterpTo(YawDelta,Target,GetWorld()->DeltaTimeSeconds,6.0f);
	
	if(MyCharacter->GetShiftDown())
	{
		bShiftKeyDown = true;
	}
	else
	{
		bShiftKeyDown = false;
	}

	// Set Direction 
	Direction = CalculateDirection(MyCharacter->GetActorForwardVector(),MyCharacter->GetVelocity().Rotation());
	Yaw = MyCharacter->GetBaseAimRotation().Yaw;
	Pitch = MyCharacter->GetBaseAimRotation().Pitch;
}

void USAnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage);
	}
}