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

	SetupStimulusSource();
	//Weapon = CreateDefaultSubobject<UActorComponent>(TEXT("Weapon"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	initialFieldofView = FollowCamera->FieldOfView;
	BaseCameraPos = FollowCamera->GetRelativeLocation();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (bIsStun)
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
			if (!bIsCrouched || CrouchNoiseReduction <= 0.f)
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

			// Conserver uniquement le Yaw (axe horizontal) pour éviter les inclinaisons
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
	if (bIsStun)
		return;
	Crouch();
}

void APlayerCharacter::EndCrouching()
{
	if (bIsStun)
		return;
	UnCrouch();
}

void APlayerCharacter::Aim()
{
	if (bIsStun)
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
		if (!bIsLaunchingLeft && isAiming == NONE)
		{
			if (GrabComp->Grab(false))
				bIsLaunchingLeft = true;
		}
		else if (isAiming == NONE)
		{
			isAiming = ANIMAL_LEFT;
		}
	}
}

void APlayerCharacter::StopAim()
{
	if (bIsStun)
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
		if (bIsLaunchingLeft && isAiming == ANIMAL_LEFT)
		{
			GrabComp->Launch(false, baseLaunchPower);
			bIsLaunchingLeft = false;
			isAiming = NONE;
		}
	}
}

void APlayerCharacter::Shoot()
{
	if (bIsStun)
		return;
	if (bIsShooting)
	{
		//Weapon->Shoot;
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Shoot");
	}
	else 
	{
		if (!bIsLaunchingRight && isAiming == NONE)
		{
			if (GrabComp->Grab(true))
				bIsLaunchingRight = true;
		}
		else if(isAiming == NONE)
		{
			isAiming = ANIMAL_RIGHT;
		}
	}
}

void APlayerCharacter::StopShooting()
{
	if (bIsStun)
		return;
	if (!bIsShooting)
	{
		if (bIsLaunchingRight && isAiming == ANIMAL_RIGHT)
		{
			GrabComp->Launch(true, baseLaunchPower);
			bIsLaunchingRight = false;
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
	if (bIsStun)
		return;
	ACharacter::Jump();
}

void APlayerCharacter::EndJumping(const FInputActionValue& Value)
{
	if (bIsStun)
		return;
	ACharacter::StopJumping();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStun)
	{
		StunTimer -= GetWorld()->DeltaTimeSeconds;

		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::SanitizeFloat(StunTimer));

		if (StunTimer <= 0.f)
			bIsStun = false;
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

void APlayerCharacter::Stun(float _time)
{
	StunTimer = _time;
	bIsStun = true;
	isAiming = NONE;
}

