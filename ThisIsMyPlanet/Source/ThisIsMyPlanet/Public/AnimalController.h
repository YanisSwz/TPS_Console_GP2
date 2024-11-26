// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Animal.h"
#include "../PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "AnimalController.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API AAnimalController : public AAIController
{
	GENERATED_BODY()

public:
	AAnimalController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BT;

	APawn* Player1;
	APawn* Player2;
	AAnimal* ControlledAnimal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Survive();
	void Flee();
	void Sleep();

private:
	class UAISenseConfig_Sight* SightConfig;

	void CreatePerceptionSystem();
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
