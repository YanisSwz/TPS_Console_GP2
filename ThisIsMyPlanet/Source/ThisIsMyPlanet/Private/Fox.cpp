// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"

void AFox::BeginPlay()
{
	Super::BeginPlay();
}

void AFox::Tick(float DeltaTime)
{
	if (bIsTurningLeft)
	{
		ScoutStepAngle += TurningSpeed * DeltaTime;

	}
	else
	{
		ScoutStepAngle -= TurningSpeed * DeltaTime;
	}
	if (FMath::RandRange(0, FMath::FloorToInt(TurningInverseFrequency * DeltaTime)) == 1) bIsTurningLeft = !bIsTurningLeft;
	if (FMath::RandRange(0, FMath::FloorToInt(JumpingInverseFrequency * DeltaTime)) == 1 && !bIsSleeping) Jump();

	HungerTimer -= DeltaTime;
	if (HungerTimer <= 0.0f) HungerTimer = 0.0f;
	
}

void AFox::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am survivig"));*/

	

	TArray<AActor*> chickenList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChicken::StaticClass(), chickenList);
	float dist = 1000.0f;
	AActor* nearestChicken = UGameplayStatics::FindNearestActor(GetActorLocation(), chickenList, dist);
	if (nearestChicken == nullptr)
	{
		TargetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(ScoutStepAngle, FVector::UpVector);
		
	}
	else if ((nearestChicken->GetActorLocation() - GetActorLocation()).Length() < 3000.0f && HungerTimer <= 0.0f)
	{
		TargetLocation = nearestChicken->GetActorLocation();
		if ((TargetLocation - GetActorLocation()).Length() < 100.0f)
		{
			// TODO: ATTACC
			nearestChicken->Destroy();
			HungerTimer = HungerTime;
		}
	}
	else
	{
		TargetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(ScoutStepAngle, FVector::UpVector);
	}
	
}

void AFox::Flee()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am fleeig"));*/

	FVector playerLocation;
	FVector player1Location = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector player2Location = UGameplayStatics::GetPlayerPawn(GetWorld(), 1)->GetActorLocation();
	if ((player1Location - GetActorLocation()).Length() < (player2Location - GetActorLocation()).Length())
	{
		playerLocation = player1Location;
	}
	else
	{
		playerLocation = player2Location;
	}
	TargetLocation = GetActorLocation() - (playerLocation - GetActorLocation());
}

void AFox::ApplyEffect(APawn* player)
{

}
