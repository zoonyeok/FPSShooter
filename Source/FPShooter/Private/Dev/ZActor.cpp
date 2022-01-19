// zoonyeok All Rights Reserved.


#include "Dev/ZActor.h"

#include <destructible/ModuleDestructible.h>

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZActor::AZActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("USceneComponent"));
	SetRootComponent(SceneComponent);
	
}

// Called when the game starts or when spawned
void AZActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(),GetActorLocation(),Radius, 24, SphereColor);
	UGameplayStatics::ApplyRadialDamage(GetWorld(),Damage, GetActorLocation(),
		Radius, nullptr, {}, this, nullptr,bDoFullDamage);
}

