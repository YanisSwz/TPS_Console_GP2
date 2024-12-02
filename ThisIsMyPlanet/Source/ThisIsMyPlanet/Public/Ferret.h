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
	float BuryDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Patrol", meta = (AllowPrivateAccess = "true"))
	float PatrolRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Patrol", meta = (AllowPrivateAccess = "true"))
	float WaitDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Patrol", meta = (AllowPrivateAccess = "true"))
	float MaxAggroRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Rotation", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Rotation", meta = (AllowPrivateAccess = "true"))
	float RotationDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Rotation", meta = (AllowPrivateAccess = "true"))
	int NumberOfRotations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Snare", meta = (AllowPrivateAccess = "true"))
	float SnareDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ferret|Snare", meta = (AllowPrivateAccess = "true"))
	float SlowAmount;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;

	void ApplyEffect(APlayerCharacter* Player) override;

	void Bury();

	void Emerge();

private:
	bool bIsLookingForSpot;
	bool bAppliedSnare;
	bool bIsBuried;
	bool bIsTurning;
	float WaitTimer;
	float BuryTimer;
	float AngleAxis;
	FVector AxisVector;
	int RotationCount;
};
	
