// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"


void APlayerCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(UAISense_Sight::StaticClass());
		StimulusSource->RegisterForSense(UAISense_Hearing::StaticClass());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GrabComp = CreateDefaultSubobject<UGrabberComponent>(TEXT("GrabberComponent"));
	GrabComp->OnUnGrab.BindUObject(this, &ThisClass::UnGrab);


	Weapon = CreateDefaultSubobject<UWeapon>(TEXT("Weapon"));

	SetupStimulusSource();
	if (CrouchSoundNoiseReduction <= 0.f)
		CrouchSoundNoiseReduction = 1.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	initialFieldofView = FollowCamera->FieldOfView;
	BaseCameraPos = FollowCamera->GetRelativeLocation();
	InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
	InitialSpeedCrouched = GetCharacterMovement()->MaxWalkSpeedCrouched;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsStunned)
		return;
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (isAiming != Aiming::NONE)
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		else
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (isAiming != Aiming::NONE)
		{
			FRotator ControlRotation = Controller->GetControlRotation();

			// Keep only Yaw (horizontal axe) to avoid tilting
			FRotator NewRotation = FRotator(0.0f, ControlRotation.Yaw, 0.0f);

			// Apply this rotation to character
			SetActorRotation(NewRotation);
		}
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void APlayerCharacter::StartCrouching()
{
	if (bIsStunned || GetCharacterMovement()->IsFalling())
		return;
	Crouch();
}

void APlayerCharacter::EndCrouching()
{
	if (bIsStunned)
		return;
	UnCrouch();
}

void APlayerCharacter::Aim()
{
	if (bIsStunned)
		return;
	if (bIsShooting)
	{
		isAiming = Aiming::WEAPON;
		Look(0);
	}
	else
	{
		if (!GrabComp->GetIsGrabbed(false) && isAiming == Aiming::NONE)
		{
			if (GrabLeftAnimation)
				PlayAnimMontage(GrabLeftAnimation);
			GrabComp->Grab(false);

			Look(0);
		}
		else if (isAiming == Aiming::NONE)
		{
			isAiming = Aiming::ANIMAL_LEFT;
			GrabComp->SwitchHand(false);
		}
	}
}

void APlayerCharacter::StopAim()
{
	if (bIsStunned)
		return;
	if (bIsShooting)
	{
		isAiming = Aiming::NONE;
	}
	else
	{
		if (GrabComp->GetIsGrabbed(false) && isAiming == Aiming::ANIMAL_LEFT)
		{
			if (ThrowLeftAnimation)
				PlayAnimMontage(ThrowLeftAnimation);
			GrabComp->Launch(false, baseLaunchPower);
			isAiming = Aiming::NONE;
		}
	}
}

void APlayerCharacter::Shoot()
{
	if (bIsStunned)
		return;
	if (bIsShooting)
	{
		if (bCanShoot && isAiming == Aiming::WEAPON)
		{
			if (ShootAnimation)
				PlayAnimMontage(ShootAnimation);
			Weapon->Fire(FollowCamera->GetForwardVector());
			bCanShoot = false;
			reloadTimer = maxReloadTimer;
		}
	}
	else
	{
		if (!GrabComp->GetIsGrabbed(true) && isAiming == Aiming::NONE)
		{
			if (GrabRightAnimation)
				PlayAnimMontage(GrabRightAnimation);
			GrabComp->Grab(true);
		}
		else if (isAiming == Aiming::NONE)
		{
			isAiming = Aiming::ANIMAL_RIGHT;
			GrabComp->SwitchHand(true);
		}
	}
}

void APlayerCharacter::StopShooting()
{
	if (bIsStunned)
		return;
	if (!bIsShooting)
	{
		if (GrabComp->GetIsGrabbed(true) && isAiming == Aiming::ANIMAL_RIGHT)
		{
			if (ThrowRightAnimation)
				PlayAnimMontage(ThrowRightAnimation);
			GrabComp->Launch(true, baseLaunchPower);
			isAiming = Aiming::NONE;
		}
	}
}

void APlayerCharacter::Switch()
{
	if (WeaponMesh)
	{
		WeaponMesh->SetVisibility(!WeaponMesh->IsVisible());
	}
	bIsShooting = !bIsShooting;
	isAiming = Aiming::NONE;
}

void APlayerCharacter::StartJumping(const FInputActionValue& Value)
{
	if (bIsStunned || bIsCrouched)
		return;
	ACharacter::Jump();
	switch (isAiming)
	{
	case Aiming::WEAPON:
		if (AimingJumpAnimation != nullptr)
			PlayAnimMontage(AimingJumpAnimation);
		break;
	case Aiming::ANIMAL_RIGHT:
		if (GrabRightJumpAnimation != nullptr)
			PlayAnimMontage(GrabRightJumpAnimation);
		break;
	case Aiming::ANIMAL_LEFT:
		if (GrabLeftJumpAnimation != nullptr)
			PlayAnimMontage(GrabLeftJumpAnimation);
		break;
	default:
		if (JumpAnimation != nullptr)
			PlayAnimMontage(JumpAnimation);
		break;
	}
}

void APlayerCharacter::EndJumping(const FInputActionValue& Value)
{
	if (bIsStunned)
		return;
	ACharacter::StopJumping();
}

void APlayerCharacter::UnGrab(bool bIsRightHand)
{
	if (bIsRightHand && isAiming == Aiming::ANIMAL_RIGHT)
		isAiming = Aiming::NONE;
	else if (!bIsRightHand && isAiming == Aiming::ANIMAL_LEFT)
		isAiming = Aiming::NONE;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanShoot)
	{
		reloadTimer -= DeltaTime;
		if (reloadTimer <= 0.f)
			bCanShoot = true;
	}

	if (bIsStunned)
	{
		StunTimer -= GetWorld()->DeltaTimeSeconds;

		if (StunTimer <= 0.f)
		{
			FVector inertia = GetMesh()->GetBoneLinearVelocity("pelvis");
			bIsStunned = false;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetCharacterMovement()->GravityScale = 1;
			GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", false);
			GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true));
			GetMesh()->SetRelativeLocationAndRotation(-BaseMeshOffset, BaseMeshRotation);

			GetCharacterMovement()->Velocity = inertia;
		}
		else
		{
			GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("pelvis") + BaseMeshOffset);
			GetCapsuleComponent()->ResetSceneVelocity();
		}
	}

	if (bIsSnared)
	{
		SnareTimer -= GetWorld()->DeltaTimeSeconds;

		if (SnareTimer <= 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = InitialSpeedCrouched;
			bIsSnared = false;
		}
	}

	switch (isAiming)
	{
	case Aiming::NONE:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, initialFieldofView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), BaseCameraPos, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	case Aiming::WEAPON:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), BaseCameraPos, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	case Aiming::ANIMAL_LEFT:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), FVector(CameraZoomGrabPosition.X, -CameraZoomGrabPosition.Y, CameraZoomGrabPosition.Z), CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	case Aiming::ANIMAL_RIGHT:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), CameraZoomGrabPosition, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	default:
		break;
	}

	if (GetActorLocation().X > 10000.0f || GetActorLocation().X < -10000.0f || GetActorLocation().Y > 10000.0f || GetActorLocation().Y < -10000.0f)
	{
		SetActorLocation(FVector(-500.0f, -500.0f, 3000.0f));
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::StartJumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndCrouching);

		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAim);

		// Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &APlayerCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopShooting);

		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Started, this, &APlayerCharacter::Switch);
	}

}

Aiming APlayerCharacter::GetAimState()
{
	return isAiming;
}

void APlayerCharacter::Stun(float Duration)
{
	StunTimer = Duration;
	bIsStunned = true;
	isAiming = Aiming::NONE;

	if (GrabComp->GetIsGrabbed(true))
	{
		GrabComp->Launch(true, 0);
	}
	if (GrabComp->GetIsGrabbed(false))
	{
		GrabComp->Launch(false, 0);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector::Zero();
	UnCrouch();
	GetCapsuleComponent()->ResetSceneVelocity();

	GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("pelvis", 1.f);

	UGameplayStatics::PlaySoundAtLocation(this, StunSound, this->GetActorLocation(), 1.f, FMath::RandRange(0.8f, 1.2f));
}

void APlayerCharacter::Snare(float Duration, float SlowAmount)
{
	if (SlowAmount <= 0.f)
		return;
	SnareTimer = Duration;
	GetCharacterMovement()->MaxWalkSpeed /= SlowAmount;
	GetCharacterMovement()->MaxWalkSpeedCrouched /= SlowAmount;
	bIsSnared = true;
}

