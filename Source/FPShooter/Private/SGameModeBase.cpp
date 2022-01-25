// zoonyeok All Rights Reserved.


#include "SGameModeBase.h"
#include "ZSpectatorPawn.h"
#include "Player/SBaseCharacter.h"
#include "Player/SPlayerController.h"
#include "UI/ZHUD.h"

ASGameModeBase::ASGameModeBase()
{
	DefaultPawnClass = ASBaseCharacter::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	HUDClass = AZHUD::StaticClass();
}