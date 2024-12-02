// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Bear.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API ABear : public AAnimal
{
	GENERATED_BODY()

protected:
	bool bIsLookingForSpot = true;
	float EatingTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float EatingTime;
	bool bIsEating = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float BerrySearchingRadius;
	bool bHasAttacked;
	int BerryCount;
	float NapTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float StunTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;

	//void Sleep() override;

	void ApplyEffect(APlayerCharacter* player) override;
	
};
