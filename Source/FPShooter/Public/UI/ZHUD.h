// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API AZHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** The Main Draw loop for the hud.  Gets called before any messaging.  Should be subclassed */
	virtual void DrawHUD() override;

private:
	void DrawCrossHair();
	
};
