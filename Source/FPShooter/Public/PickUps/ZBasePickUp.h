// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZBasePickUp.generated.h"

UCLASS()
class FPSHOOTER_API AZBasePickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	AZBasePickUp();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	float RespawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	bool bRotate;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	float RotateYawValue;
	
	virtual bool GivePickUpTo(APawn* PlayerPawn);
	void PickUpWasTaken();
	void Respawn();
	void MakeRotateYaw(float Value);
};

