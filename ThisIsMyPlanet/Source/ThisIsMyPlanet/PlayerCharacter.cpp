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
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
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

		if (bIsAiming)
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
		if (bIsAiming)
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
	Crouch();
	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Oui");
}

void APlayerCharacter::EndCrouching()
{
	UnCrouch();
	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Non");
}

void APlayerCharacter::Aim()
{
	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Aim");
	
	bIsAiming = true;
	//FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, 0.1f);

	Look(0);
}

void APlayerCharacter::StopAim()
{
	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "Stop");

	bIsAiming = false;
	//FollowCamera->FieldOfView = initialFieldofView;
}

void APlayerCharacter::Shoot()
{
	if (bIsShooting)
	{
		//Weapon->Shoot;
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Shoot");
	}
	else
	{
		if (!bIsLaunchingRight)
		{
			if (GrabComp->Grab(true))
				bIsLaunchingRight = true;
		}
		else
		{
			GrabComp->Launch(true, baseLaunchPower);
			bIsLaunchingRight = false;
		}
	}
}

void APlayerCharacter::Switch()
{
	bIsShooting = !bIsShooting;
}

void APlayerCharacter::StartJumping(const FInputActionValue& Value)
{
	ACharacter::Jump();
}

void APlayerCharacter::EndJumping(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsAiming)
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, zoomedFieldOfView, 0.3f);
	else
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, initialFieldofView, 0.3f);
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

		EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Started, this, &APlayerCharacter::Switch);
	}

}

