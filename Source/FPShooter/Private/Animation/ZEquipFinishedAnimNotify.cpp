// zoonyeok All Rights Reserved.


#include "Animation/ZEquipFinishedAnimNotify.h"

void UZEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
