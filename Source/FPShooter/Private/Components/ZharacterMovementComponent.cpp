// zoonyeok All Rights Reserved.


#include "Components/ZharacterMovementComponent.h"
#include "Player/SBaseCharacter.h"


float UZharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed =  Super::GetMaxSpeed();
	const ASBaseCharacter* Player = Cast<ASBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed ;
}
