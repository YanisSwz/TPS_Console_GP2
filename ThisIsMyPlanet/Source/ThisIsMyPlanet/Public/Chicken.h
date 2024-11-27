// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "AIController.h"
#include "Chicken.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AChicken : public AAnimal
{
	GENERATED_BODY()
protected:
	bool bIsLookingForSpot;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Survive() override;

	void Flee() override;

	void Sleep() override;

	void ApplyEffect() override;
	
};
