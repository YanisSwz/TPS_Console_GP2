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
	//Super::Tick(DeltaTime);
	if (!ControlledAnimal)
		return;
	
	

	FVector vector = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	
	EPathFollowingRequestResult::Type result = MoveToLocation(ControlledAnimal->targetLocation.Location, 50.0f);
	//if (GEngine && result == EPathFollowingRequestResult::Type::Failed)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "nay");
	//ControlledAnimal->AddMovementInput((vector - UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()).GetSafeNormal(), 1000.0f);
	//ControlledAnimal->SetActorLocation(vector);
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
