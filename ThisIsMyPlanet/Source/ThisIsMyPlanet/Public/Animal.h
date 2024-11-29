// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animal.generated.h"

UCLASS()
class THISISMYPLANET_API AAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	bool bIsSleeping;
	FVector targetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal", meta = (AllowPrivateAccess = "true"))
	float Speed;

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

protected:

	int health;
	float pointValue;
	float sleepTimer;
	
	
	
public:
	// Sets default values for this character's properties
	AAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Survive();

	virtual void Flee();

	void Sleep();

	virtual void ApplyEffect();

};
