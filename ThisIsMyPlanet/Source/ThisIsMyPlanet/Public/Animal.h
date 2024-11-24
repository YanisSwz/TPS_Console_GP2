// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "Animal.generated.h"

UCLASS()
class THISISMYPLANET_API AAnimal : public ACharacter
{
	GENERATED_BODY()




protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animal", meta = (AllowPrivateAccess = "true"))
	float speed;

	int health;
	float pointValue;
	float sleepTimer;
	FNavLocation targetLocation;
	bool isSleeping;
	
public:
	// Sets default values for this character's properties
	AAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Survive();

	virtual void Flee();

	virtual void Sleep();

	virtual void ApplyEffect();

};
