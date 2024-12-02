// Fill out your copyright notice in the Description page of Project Settings.


#include "Deer.h"

void ADeer::BeginPlay()
{
	Super::BeginPlay();
}

void ADeer::Tick(float DeltaTime)
{
	eatingTimer -= DeltaTime;
	if (eatingTimer < 0.0f)
	{
		eatingTimer = 0.0f;
	}
	if (FMath::RandRange(0, FMath::FloorToInt(turningInverseFrequency * DeltaTime)) == 1 && !bIsSleeping) bIsTurningLeft = !bIsTurningLeft;
}

void ADeer::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("AREYOUADEERSNIFFSNIFFSNIFFSNIFF"));*/
	

	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		TArray<AActor*> deerList;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeer::StaticClass(), deerList);
		float dist = 1000.0f;
		AActor* nearestDeer = UGameplayStatics::FindNearestActor(GetActorLocation(), deerList, dist);
		deerList.RemoveSingle(nearestDeer);
		nearestDeer = UGameplayStatics::FindNearestActor(GetActorLocation(), deerList, dist);


		FNavLocation targetFNavLocation;
		if (nearestDeer == nullptr)
		{
			NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), grassSearchingRadius, targetFNavLocation);
		}
		else
		{
			NavSystem->GetRandomReachablePointInRadius(nearestDeer->GetActorLocation(), grassSearchingRadius, targetFNavLocation);
		}
		
		TargetLocation = targetFNavLocation.Location;
		bIsLookingForSpot = false;
	}
	if ((GetActorLocation() - TargetLocation).Length() <= 160.0f && !bIsEating)
	{

		bIsEating = true;
		eatingTimer = eatingTime;
	}
	if (bIsEating && eatingTimer <= 0.0f)
	{
		bIsEating = false;
		bIsLookingForSpot = true;
	}
	bHasAttacked = false;
}

void ADeer::Flee()
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

		if (bIsTurningLeft)
		{
			TargetLocation = GetActorLocation() - (playerLocation - GetActorLocation()).RotateAngleAxis(-45.0f, FVector(0.0f, 0.0f, 1.0f));
		}
		else
		{
			TargetLocation = GetActorLocation() - (playerLocation - GetActorLocation()).RotateAngleAxis(45.0f, FVector(0.0f, 0.0f, 1.0f));
		}

	}
}

void ADeer::ApplyEffect(APawn* player)
{
	// TODO: BIG AHH STUN
	Cast<ACharacter>(player)->LaunchCharacter((player->GetActorLocation() - GetActorLocation()) * 5.0f + FVector(0.0f, 0.0f, 300.0f), false, false);
}