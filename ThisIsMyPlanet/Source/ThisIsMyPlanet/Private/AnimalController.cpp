// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalController.h"

AAnimalController::AAnimalController()
{
	PrimaryActorTick.bCanEverTick = true;
	CreatePerceptionSystem();
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

	SetupPerceptionSystem();

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

void AAnimalController::CreatePerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	}
}

void AAnimalController::SetupPerceptionSystem()
{
	SightConfig->SightRadius = ControlledAnimal->SightRadius;
	SightConfig->LoseSightRadius = ControlledAnimal->LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = ControlledAnimal->PeripheralVisionAngleDegrees;
	SightConfig->SetMaxAge(ControlledAnimal->MaxAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = ControlledAnimal->AutoSuccessRangeFromLastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAnimalController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAnimalController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if(auto* const ch = Cast<APlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("bSawPlayer", Stimulus.WasSuccessfullySensed());
	}
}
