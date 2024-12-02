// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Ferret.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AFerret : public AAnimal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret", meta = (AllowPrivateAccess = "true"))
	AActor* Burrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret", meta = (AllowPrivateAccess = "true"))
	float PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret", meta = (AllowPrivateAccess = "true"))
	float WaitDuration;

	bool bIsLookingForSpot = true;
	float WaitTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;

	void ApplyEffect(APawn* Player) override;
};
	
