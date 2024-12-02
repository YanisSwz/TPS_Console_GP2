// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Chicken.h"
#include "Fox.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AFox : public AAnimal
{
	GENERATED_BODY()

protected:
	float scoutStepAngle;
	bool bIsTurningLeft;
	AChicken* chasedChicken;
	float hungerTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float turningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float turningInverseFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float jumpingInverseFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float hungerTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float StunTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;
	void ApplyEffect(APlayerCharacter* player) override;
};
