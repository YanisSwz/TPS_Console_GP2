// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Chicken.h"
#include "Animation/AnimMontage.h"
#include "Fox.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AFox : public AAnimal
{
	GENERATED_BODY()

protected:
	float ScoutStepAngle;
	bool bIsTurningLeft;
	AChicken* ChasedChicken;
	float HungerTimer;
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float FleeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float TurningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float TurningInverseFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float JumpingInverseFrequency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float HungerTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float StunTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float EatingDistance = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAnimation;
	float SoundTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float MaxSoundTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float SoundTimerRandOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	USoundBase* FoxAmbientSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	USoundAttenuation* FoxSoundAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float ChickenSpottingRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fox", meta = (AllowPrivateAccess = "true"))
	float MoveStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fox", meta = (AllowPrivateAccess = "true"))
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
	void ApplyEffect(APlayerCharacter* player) override;
};
