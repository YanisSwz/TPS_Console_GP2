// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Fox.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AFox : public AAnimal
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;

	void Sleep() override;

	void ApplyEffect() override;
};
