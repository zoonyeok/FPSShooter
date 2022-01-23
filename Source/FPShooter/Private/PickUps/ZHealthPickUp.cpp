// zoonyeok All Rights Reserved.


#include "PickUps/ZHealthPickUp.h"
#include "ZUtils.h"
#include "ZHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickUp, All, All);

AZHealthPickUp::AZHealthPickUp()
	: HealthAmount(50.f)
{
}

bool AZHealthPickUp::GivePickUpTo(APawn* PlayerPawn)
{
	UE_LOG(LogHealthPickUp, Display, TEXT("HealthPickup was taken"));
	
	const auto HealthComponent = ZUtils::GetZPlayerComponentByClass<UZHealthComponent>(PlayerPawn);
	if (HealthComponent == nullptr) return false;
	return HealthComponent->TryToAddHealth(HealthAmount);
}