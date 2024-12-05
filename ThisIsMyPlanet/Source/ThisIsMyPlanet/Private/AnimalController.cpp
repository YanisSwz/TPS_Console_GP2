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
	Super::Tick(DeltaTime);
	if (!ControlledAnimal)
		return;
	
	if (!ControlledAnimal->bIsSleeping)
	{
		EPathFollowingRequestResult::Type result = MoveToLocation(ControlledAnimal->TargetLocation, 5.f);
		if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
			ControlledAnimal->SetReachedDestination(true);
		else
			ControlledAnimal->SetReachedDestination(false);

		if (GetBlackboardComponent()->GetValueAsBool("bIsSleeping"))
			GetBlackboardComponent()->SetValueAsBool("bIsSleeping", false);
	}
	else
	{
		if (!GetBlackboardComponent()->GetValueAsBool("bIsSleeping"))
			GetBlackboardComponent()->SetValueAsBool("bIsSleeping", true);
	}
	
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

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAnimalController::OnTargetDetected);
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if(HearingConfig)
	{
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}

void AAnimalController::SetupPerceptionSystem()
{
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	SightConfig->SightRadius = ControlledAnimal->SightRadius;
	SightConfig->LoseSightRadius = ControlledAnimal->LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = ControlledAnimal->PeripheralVisionAngleDegrees;
	SightConfig->SetMaxAge(ControlledAnimal->MaxAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = ControlledAnimal->AutoSuccessRangeFromLastSeenLocation;

	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	HearingConfig->HearingRange = ControlledAnimal->HearingRange;
	HearingConfig->SetMaxAge(ControlledAnimal->HearingMaxAge);

	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}

void AAnimalController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if(auto* const ch = Cast<APlayerCharacter>(Actor))
	{
		if (ControlledAnimal->ClosestPlayer == nullptr)
		{
			ControlledAnimal->ClosestPlayer = ch;
		}
		else if(FVector::Dist(ControlledAnimal->GetActorLocation(), ch->GetActorLocation()) < FVector::Dist(ControlledAnimal->GetActorLocation(), ControlledAnimal->ClosestPlayer->GetActorLocation()))
		{
			ControlledAnimal->ClosestPlayer = ch;
		}

		if (Stimulus.Type == SightConfig->GetSenseID())
		{
			GetBlackboardComponent()->SetValueAsBool("bSawPlayer", Stimulus.WasSuccessfullySensed());
		}
		else if (Stimulus.Type == HearingConfig->GetSenseID())
		{
			GetBlackboardComponent()->SetValueAsBool("bHeardPlayer", Stimulus.WasSuccessfullySensed());
		}
	}
}
