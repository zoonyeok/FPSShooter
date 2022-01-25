// zoonyeok All Rights Reserved.


#include "ZSpectatorPawn.h"
#include "Camera/CameraComponent.h"

AZSpectatorPawn::AZSpectatorPawn()
{
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->PostProcessSettings.ColorSaturation = FVector4(1.0f,1.0f,1.0f,0.0f);
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetRootComponent());
}