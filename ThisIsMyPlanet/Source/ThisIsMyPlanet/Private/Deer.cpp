// Fill out your copyright notice in the Description page of Project Settings.


#include "Deer.h"

void ADeer::BeginPlay()
{
	Super::BeginPlay();
}

void ADeer::Tick(float DeltaTime)
{
	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}
	if (FMath::RandRange(0, FMath::FloorToInt(TurningInverseFrequency * DeltaTime)) == 1 && !bIsSleeping) bIsTurningLeft = !bIsTurningLeft;
}

void ADeer::Survive()
{
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
			NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), GrassSearchingRadius, targetFNavLocation);
		}
		else
		{
			NavSystem->GetRandomReachablePointInRadius(nearestDeer->GetActorLocation(), GrassSearchingRadius, targetFNavLocation);
		}
		
		TargetLocation = targetFNavLocation.Location;
		bIsLookingForSpot = false;
	}
	if (bReachedDestination && !bIsEating)
	{
		bIsEating = true;
		EatingTimer = EatingTime;
	}
	if (bIsEating && EatingTimer <= 0.0f)
	{
		bIsEating = false;
		bIsLookingForSpot = true;
	}
	bHasAttacked = false;
}

void ADeer::Flee()
{
	if (!bHasAttacked)
	{
		TargetLocation = ClosestPlayer->GetActorLocation();
		if ((GetActorLocation() - TargetLocation).Length() <= 160.0f)
		{
			if (ClosestPlayer != nullptr)
			{
				ApplyEffect(ClosestPlayer);
				bHasAttacked = true;
			}
		}
	}
	else
	{
		if (bIsTurningLeft)
		{
			TargetLocation = GetActorLocation() - (ClosestPlayer->GetActorLocation() - GetActorLocation()).RotateAngleAxis(-45.0f, FVector::UpVector);
		}
		else
		{
			TargetLocation = GetActorLocation() - (ClosestPlayer->GetActorLocation() - GetActorLocation()).RotateAngleAxis(45.0f, FVector::UpVector);
		}
	}
}

void ADeer::ApplyEffect(APlayerCharacter* player)
{
	LaunchCharacter((player->GetActorLocation() - GetActorLocation()) * 5.0f + FVector(0.0f, 0.0f, 300.0f), false, false);
	player->Stun(StunTime);
}