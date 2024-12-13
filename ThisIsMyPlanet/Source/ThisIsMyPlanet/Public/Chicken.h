// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Chicken.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AChicken : public AAnimal
{
	GENERATED_BODY()
protected:
	bool bIsLookingForSpot = true;
	bool bLaunched = false;
	float EatingTimer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chicken", meta = (AllowPrivateAccess = "true"))
	float EatingTime;
	bool bIsEating = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chicken", meta = (AllowPrivateAccess = "true"))
	float SeedSearchingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chicken", meta = (AllowPrivateAccess = "true"))
	float FlashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chicken", meta = (AllowPrivateAccess = "true"))
	float FlyForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chicken", meta = (AllowPrivateAccess = "true"))
	float HorizontalImpulse;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;
	void ApplyEffect(APlayerCharacter* player) override;
	
};
