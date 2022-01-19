// zoonyeok All Rights Reserved.

#include "Weapon/ZProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AZProjectile::AZProjectile()
	: DamageRadius(200.f), DamageAmount(50.f), bDoFullDamage(false), LifeSeconds(5.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 물리적 충돌만
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); // 모두 반응
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComponent"));
	MovementComponent->InitialSpeed = 2000.f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AZProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(CollisionComponent);
	check(MovementComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true); // 캐릭터 충돌방지
	CollisionComponent->OnComponentHit.AddDynamic(this, &AZProjectile::OnProjectileHit);
	SetLifeSpan(LifeSeconds);
}

void AZProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetWorld() == nullptr) return;

	MovementComponent->StopMovementImmediately();

	// TODO : make damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(), // 
		DamageAmount,								//
		GetActorLocation(),							//
		DamageRadius, UDamageType::StaticClass(),	//
		{GetOwner()},								//
		this,										//
		GetController(),							// 
		bDoFullDamage);								//

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);

	Destroy();
}

// GetOwner가 pawn인지 화인
AController* AZProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
