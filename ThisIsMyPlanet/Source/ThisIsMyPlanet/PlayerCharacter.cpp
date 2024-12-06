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
	if(StimulusSource)
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

	Weapon = CreateDefaultSubobject<UWeapon>(TEXT("Weapon"));

	SetupStimulusSource();
	if (CrouchNoiseReduction <= 0.f)
		CrouchNoiseReduction = 1.f;
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

		if (isAiming)
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

		if (Footsteps->CurrentPlayCount.Num() == 0)
		{
			if (!bIsCrouched)
			{
				UGameplayStatics::PlaySoundAtLocation(this, Footsteps, this->GetActorLocation(), 1.f);
				UAISense_Hearing::ReportNoiseEvent(this, this->GetActorLocation(), 1.f, this, FootstepsRange);
			}
			else 
			{
				UGameplayStatics::PlaySoundAtLocation(this, Footsteps, this->GetActorLocation(), 1.f / CrouchNoiseReduction);
				UAISense_Hearing::ReportNoiseEvent(this, this->GetActorLocation(), 1.f / CrouchNoiseReduction, this, FootstepsRange);
			}
		}

	}

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (isAiming)
		{
			//this->GetActorForwardVector().X = FollowCamera->GetForwardVector().X;

			FRotator ControlRotation = Controller->GetControlRotation();

			// Conserver uniquement le Yaw (axe horizontal) pour �viter les inclinaisons
			FRotator NewRotation = FRotator(0.0f, ControlRotation.Yaw, 0.0f);

			// Appliquer cette rotation au personnage
			SetActorRotation(NewRotation);
		}
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void APlayerCharacter::StartCrouching()
{
	if (bIsStunned)
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
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Aim");

		isAiming = WEAPON;
		//FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, 0.1f);

		Look(0);
	}
	else
	{
		if (!GrabComp->GetIsGrabbed(false) && isAiming == NONE)
		{
			GrabComp->Grab(false);
		}
		else if (isAiming == NONE)
		{
			isAiming = ANIMAL_LEFT;
		}
	}
}

void APlayerCharacter::StopAim()
{
	if (bIsStunned)
		return;
	if (bIsShooting)
	{
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Stop");

		isAiming = NONE;
		//FollowCamera->FieldOfView = initialFieldofView;
	}
	else
	{
		if (GrabComp->GetIsGrabbed(false) && isAiming == ANIMAL_LEFT)
		{
			GrabComp->Launch(false, baseLaunchPower);
			isAiming = NONE;
		}
	}
}

void APlayerCharacter::Shoot()
{
	if (bIsStunned)
		return;
	if (bIsShooting)
	{
		if (bCanShoot && isAiming == WEAPON)
		{
			Weapon->Fire(FollowCamera->GetForwardVector());
			bCanShoot = false;
			reloadTimer = maxReloadTimer;
		}
	}
	else 
	{
		if (!GrabComp->GetIsGrabbed(true) && isAiming == NONE)
		{
			GrabComp->Grab(true);
		}
		else if(isAiming == NONE)
		{
			isAiming = ANIMAL_RIGHT;
		}
	}
}

void APlayerCharacter::StopShooting()
{
	if (bIsStunned)
		return;
	if (!bIsShooting)
	{
		if (GrabComp->GetIsGrabbed(true) && isAiming == ANIMAL_RIGHT)
		{
			GrabComp->Launch(true, baseLaunchPower);
			isAiming = NONE;
		}
	}
}

void APlayerCharacter::Switch()
{
	bIsShooting = !bIsShooting;
	isAiming = NONE;
}

void APlayerCharacter::StartJumping(const FInputActionValue& Value)
{
	if (bIsStunned)
		return;
	ACharacter::Jump();
}

void APlayerCharacter::EndJumping(const FInputActionValue& Value)
{
	if (bIsStunned)
		return;
	ACharacter::StopJumping();
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
	
	if(bIsSnared)
	{
		SnareTimer -= GetWorld()->DeltaTimeSeconds;

		if(SnareTimer <= 0.f)
		{
			GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = InitialSpeedCrouched;
			bIsSnared = false;
		}
	}

	switch (isAiming)
	{
	case APlayerCharacter::NONE:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, initialFieldofView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), BaseCameraPos, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	case APlayerCharacter::WEAPON:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), BaseCameraPos, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	case APlayerCharacter::ANIMAL_LEFT:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), FVector(CameraZoomGrabPosition.X, -CameraZoomGrabPosition.Y, CameraZoomGrabPosition.Z), CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	case APlayerCharacter::ANIMAL_RIGHT:
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds);
		FollowCamera->SetRelativeLocation(FMath::Lerp(FollowCamera->GetRelativeLocation(), CameraZoomGrabPosition, CameraZoomSpeed * GetWorld()->DeltaTimeSeconds));
		break;
	default:
		break;
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

void APlayerCharacter::Stun(float Duration)
{
	StunTimer = Duration;
	bIsStunned = true;
	isAiming = NONE;

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
	GetCapsuleComponent()->ResetSceneVelocity();

	GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("pelvis", 1.f);
	
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

