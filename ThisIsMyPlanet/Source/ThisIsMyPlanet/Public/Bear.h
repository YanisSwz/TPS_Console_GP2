// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Bush.h"
#include "AnimalController.h"
#include "Animation/AnimMontage.h"
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
	bool bIsNapping = false;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float HitForce = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	FVector HitForceBonus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation;
	
	UFUNCTION(BlueprintCallable)
	bool GetIsNapping();

	TArray<AActor*> bushList;
	AAnimalController* AnimalController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float AfterAttackTime;
	float AfterAttackTimer;
	bool bIsWaitingAfterAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float NapTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float ChaseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bear", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;

	void Survive() override;

	void Flee() override;

	//void Sleep() override;

	void ApplyEffect(APlayerCharacter* player) override;
	void PossessedBy(AController* NewController) override;
};
