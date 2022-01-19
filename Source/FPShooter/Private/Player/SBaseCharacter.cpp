// zoonyeok All Rights Reserved.
#include "Player/SBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ZharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ZHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Player/SAnimInstance.h"
#include "Weapon/ZBaseWeapon.h"
#include "Weapon/ZWeaponComponent.h"
#include "Components/CapsuleComponent.h"

// 로그 카테고리 생성
DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

// Sets default values
ASBaseCharacter::ASBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UZharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	,LandedDamage(FVector2D(10.f, 100.f)), LandedDamageVelocity(FVector2D(900.f, 1200.0f))
	,bShiftDown(false) ,IsMovingForward(false) 

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SrpingArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SocketOffset = FVector(0.0f,100.0f,80.f);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	// Collision
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);

	// Movement Setting
	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	HealthComponent = CreateDefaultSubobject<UZHealthComponent>(TEXT("HealthComponent"));
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	HealthTextComponent->SetupAttachment(RootComponent);
	HealthTextComponent->SetOwnerNoSee(true);
	HealthTextComponent->SetTextRenderColor(FColor::Red);
	HealthTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	HealthTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	HealthTextComponent->SetRelativeLocation(FVector(0.0f,10.f,100.f));
	
	WeaponComponent = CreateDefaultSubobject<UZWeaponComponent>(TEXT("WeaponComponent"));
}

void ASBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ASBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ASBaseCharacter::OnGroundLanded);
}

// Called when the game starts or when spawned
void ASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);
}

// Called every frame
void ASBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// const float DrawScale = 100.0f;
	// const float Thickness = 5.0f;
	//
	// FVector LineStart = GetActorLocation();
	// FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 500.0f);
	// DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Blue, false, 0.0f, 0, Thickness);
	//
	// FVector Velocity_LineStart = GetActorLocation();
	// FVector Velocity_LineEnd = Velocity_LineStart + ((GetVelocity() * 500.0f));
	// DrawDebugDirectionalArrow(GetWorld(), Velocity_LineStart, Velocity_LineEnd, DrawScale, FColor::Red, false, 0.0f, 0, Thickness);
	
}

// Called to bind functionality to input
void ASBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	// Weapon
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &UZWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &UZWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UZWeaponComponent::NextWeapon);
	
	// Action 
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ASBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ASBaseCharacter::OnStopRunning);
	
	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ASBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

void ASBaseCharacter::MoveForward(float Value)
{
	IsMovingForward = Value > 0.0f;
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation{ 0,Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void ASBaseCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation{ 0,Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void ASBaseCharacter::OnStartRunning()
{
	bShiftDown = true;
}

void ASBaseCharacter::OnStopRunning()
{
	bShiftDown = false;
}

void ASBaseCharacter::OnDeath()
{
	UE_LOG(LogBaseCharacter, Display, TEXT("Player is dead"), *GetName());

	AnimInstance->PlayDeathMontage();

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponComponent->StopFire();
}

void ASBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"),Health)));
}

bool ASBaseCharacter::IsRunning() const
{
	return bShiftDown && IsMovingForward && !GetVelocity().IsZero();
}

float ASBaseCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = UKismetMathLibrary::DegAcos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(),VelocityNormal);
	return AngleBetween * FMath::Sign(CrossProduct.Z);
}

void ASBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(LogBaseCharacter, Display, TEXT("On Landed: %f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(LogBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{},nullptr,nullptr);
}

void ASBaseCharacter::SetCameraRotation(const FRotator& Direction)
{
	Camera->SetWorldRotation(Direction);
}
