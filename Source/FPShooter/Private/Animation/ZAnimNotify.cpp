// zoonyeok All Rights Reserved.


#include "Animation/ZAnimNotify.h"

void UZAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}