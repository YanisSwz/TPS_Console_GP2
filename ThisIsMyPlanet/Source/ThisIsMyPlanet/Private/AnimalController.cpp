// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalController.h"

AAnimalController::AAnimalController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAnimalController::BeginPlay()
{
	Super::BeginPlay();
	Player1 = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Player2 = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);
}

void AAnimalController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledAnimal = Cast<AAnimal>(InPawn);

	UBlackboardComponent* TempBlackboard;
	UseBlackboard(BT->GetBlackboardAsset(), TempBlackboard);
	Blackboard = TempBlackboard;
	Blackboard->SetValueAsObject("Player1", Player1);
	Blackboard->SetValueAsObject("Player2", Player2);

	RunBehaviorTree(BT);
}

void AAnimalController::OnUnPossess()
{
	ControlledAnimal = nullptr;
}

void AAnimalController::Tick(float DeltaTime)
{
	if (!ControlledAnimal)
		return;
	// if not sleeping: MoveTo(); targetLocation
}

void AAnimalController::Survive()
{
	ControlledAnimal->Survive();
}

void AAnimalController::Flee()
{
	ControlledAnimal->Flee();
}

void AAnimalController::Sleep()
{
	ControlledAnimal->Sleep();
}
