// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"

void ABear::BeginPlay()
{
	Super::BeginPlay();
}

void ABear::Tick(float DeltaTime)
{
	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}

	SleepTimer -= DeltaTime;
	if (SleepTimer <= 0.0f)
	{
		SleepTimer = 0.0f;
		BerryCount = 0;
	}

}

void ABear::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("BEAR"));*/
	

	if (SleepTimer > 0.0f)
	{
		// honk mimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimi
	}
	else
	{
		if (bIsLookingForSpot)
		{
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			if (!NavSystem)
				return;

			FNavLocation targetFNavLocation;
			NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), BerrySearchingRadius, targetFNavLocation);
			TargetLocation = targetFNavLocation.Location;
			bIsLookingForSpot = false;
		}
		if ((GetActorLocation() - TargetLocation).Length() <= 160.0f && !bIsEating)
		{

			bIsEating = true;
			EatingTimer = EatingTime;
		}
		if (bIsEating && EatingTimer <= 0.0f)
		{
			bIsEating = false;
			bIsLookingForSpot = true;
			++BerryCount;
		}
		if (BerryCount >= 5)
		{
			SleepTimer = 15.0f;
		}
	}
	
	
	bHasAttacked = false;
}

void ABear::Flee()
{
	FVector playerLocation;
	FVector player1Location = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector player2Location = UGameplayStatics::GetPlayerPawn(GetWorld(), 1)->GetActorLocation();
	APawn* nearestPlayer;
	if ((player1Location - GetActorLocation()).Length() < (player2Location - GetActorLocation()).Length())
	{
		playerLocation = player1Location;
		nearestPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	else
	{
		playerLocation = player2Location;
		nearestPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);
	}

	if (!bHasAttacked)
	{

		TargetLocation = playerLocation;
		if ((GetActorLocation() - TargetLocation).Length() <= 160.0f)
		{
			ApplyEffect(nearestPlayer);
			bHasAttacked = true;
		}
	}
	else
	{
		TargetLocation = GetActorLocation() - (playerLocation - GetActorLocation());
	}
}

void ABear::ApplyEffect(APawn* player)
{
	// TODO: BIG AHH RAGDOLL
	Cast<ACharacter>(player)->LaunchCharacter((player->GetActorLocation() - GetActorLocation()) * 15.0f + FVector(0.0f, 0.0f, 1000.0f), false, false);
}