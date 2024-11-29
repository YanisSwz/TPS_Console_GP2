// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Deer.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API ADeer : public AAnimal
{
	GENERATED_BODY()

protected:
	bool bIsLookingForSpot = true;
	float eatingTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float eatingTime;
	bool bIsEating = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float berrySearchingRadius;
	bool bHasAttacked;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;

	//void Sleep() override;

	void ApplyEffect(APawn* player) override;
	
};
