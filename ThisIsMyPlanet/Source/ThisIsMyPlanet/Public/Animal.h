// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animal.generated.h"

UCLASS()
class THISISMYPLANET_API AAnimal : public ACharacter
{
	GENERATED_BODY()

	UFUNCTION()
	void OnAnimalHit(AActor* _SelfActor, AActor* _OtherActor, FVector _NormalImpulse, const FHitResult& _Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	UGrabbableComponent* GrabbableComp;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FVector BaseMeshOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	FQuat BaseMeshRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Sight", meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Sight", meta = (AllowPrivateAccess = "true"))
	float LoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Sight", meta = (AllowPrivateAccess = "true"))
	float PeripheralVisionAngleDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Sight", meta = (AllowPrivateAccess = "true"))
	float MaxAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Sight", meta = (AllowPrivateAccess = "true"))
	float AutoSuccessRangeFromLastSeenLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Hearing", meta = (AllowPrivateAccess = "true"))
	float HearingRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Perception|Hearing", meta = (AllowPrivateAccess = "true"))
	float HearingMaxAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Sleep", meta = (AllowPrivateAccess = "true"))
	float SleepDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Sleep", meta = (AllowPrivateAccess = "true"))
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float MinVelocityToApplyEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float MaxPlayerInvincibilityTimer = 0.2f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animal|Active", meta = (AllowPrivateAccess = "true"))
	bool bIsActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal|Points", meta = (AllowPrivateAccess = "true"))
	int PointValue;

	
	APlayerCharacter* LastGrabbedBy = nullptr;
	FVector TargetLocation;
	APlayerCharacter* ClosestPlayer;
	bool bIsSleeping;

protected:
	int Health;
	float SleepTimer;
	bool bIsGrabbed;
	bool bReachedDestination;
	bool bIsPlayerInvincible = false;
	float PlayerInvincibilityTimer;
	bool bHasHitPlayer = false;
	
public:
	// Sets default values for this character's properties
	AAnimal();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Survive();

	virtual void Flee();

	void Sleep();

	virtual void ApplyEffect(APlayerCharacter* player);

	void SetUngrabbed();
	void SetReachedDestination(bool bResult);
	void SetLastGrabbedBy(AActor* actor);
	void ResetMeshPos();
	void Sell();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
