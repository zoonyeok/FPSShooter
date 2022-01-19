// zoonyeok All Rights Reserved.


#include "Dev/ZDamageActor.h"
#include <destructible/ModuleDestructible.h>
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZDamageActor::AZDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AZDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(),GetActorLocation(),Radius,24,Color);
	UGameplayStatics::ApplyRadialDamage(GetWorld(),Damage,GetActorLocation(),
		Radius,DamageType,{},this,nullptr,bDoFullDamage);
}

