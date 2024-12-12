// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GrabberComponent.h"
#include "Weapon.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Animation/AnimMontage.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class Aiming : uint8
{
	NONE,
	WEAPON,
	ANIMAL_LEFT,
	ANIMAL_RIGHT
};

UCLASS()
class THISISMYPLANET_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Grabber Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	UGrabberComponent* GrabComp;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Aim Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;
	
	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Switch between Shoot and Grab/Throw Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float baseLaunchPower = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FVector CameraZoomGrabPosition = FVector(60, 80, 80);

	FVector BaseCameraPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraZoomSpeed = 0.3f;

	float StunTimer = 0.f;
	bool bIsStunned = false;
	float SnareTimer = 0.f;
	bool bIsSnared = false;

	// Grab/Shoot
	bool bIsShooting = true;

	bool bCanShoot = true;
	float reloadTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float maxReloadTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundBase* Footsteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	float FootstepsRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	float CrouchNoiseReduction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FVector BaseMeshOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FQuat BaseMeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AimingJumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GrabRightAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GrabLeftAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThrowRightAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThrowLeftAnimation;

private:
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	void SetupStimulusSource();

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	Aiming isAiming = Aiming::NONE;

	float initialFieldofView;
	float zoomedFieldOfView = 45.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Aim();
	void StopAim();
	void Shoot();
	void StopShooting();
	void Switch();
	void StartCrouching();
	void EndCrouching();
	void StartJumping(const FInputActionValue& Value);
	void EndJumping(const FInputActionValue& Value);

	UFUNCTION()
	void UnGrab(bool bIsRightHand);


private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Flash(float Duration);

	UFUNCTION(BlueprintCallable)
	Aiming GetAimState();

	void Stun(float Duration);
	void Snare(float Duration, float SlowAmount);
	float InitialSpeed;
	float InitialSpeedCrouched;
};
