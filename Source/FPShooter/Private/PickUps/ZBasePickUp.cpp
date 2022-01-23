// zoonyeok All Rights Reserved.

#include "PickUps/ZBasePickUp.h"
#include "SBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickUP, All, All);

// Sets default values
AZBasePickUp::AZBasePickUp()
	: RespawnTime(5.0f) , bRotate(true), RotateYawValue(2.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 물리적 충돌만
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); // 모두 반응
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkeletalMeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
}

// Called when the game starts or when spawned
void AZBasePickUp::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);
	check(MeshComponent);
}

void AZBasePickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Pawn = Cast<APawn>(OtherActor);
	if(GivePickUpTo(Pawn))
	{
		PickUpWasTaken();
	}
}

void AZBasePickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeRotateYaw(RotateYawValue);
}

bool AZBasePickUp::GivePickUpTo(APawn* PlayerPawn)
{
	return false;
}

void AZBasePickUp::PickUpWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AZBasePickUp::Respawn, RespawnTime);
}

void AZBasePickUp::Respawn()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
}

void AZBasePickUp::MakeRotateYaw(float Value)
{
	if (bRotate)
	{
		MeshComponent->AddLocalRotation(FRotator(0.0f,Value,0.f));
	}
}
