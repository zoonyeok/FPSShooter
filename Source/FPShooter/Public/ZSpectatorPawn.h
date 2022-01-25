// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "ZSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API AZSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:
	AZSpectatorPawn();

private:
	class UCameraComponent* Camera;
};
