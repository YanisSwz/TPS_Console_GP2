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
	float EatingTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float EatingTime;
	bool bIsEating = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float GrassSearchingRadius;
	bool bHasAttacked;
	bool bIsTurningLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float TurningInverseFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float StunTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float HitForce = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	FVector HitForceBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float AttackRange;
	TArray<AActor*> DeerList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float TurnTime;
	float TurnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deer", meta = (AllowPrivateAccess = "true"))
	float AfterAttackTime;
	float AfterAttackTimer;
	bool bIsWaitingAfterAttack;

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
