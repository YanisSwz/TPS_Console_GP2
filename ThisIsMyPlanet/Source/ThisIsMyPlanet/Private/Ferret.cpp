// Fill out your copyright notice in the Description page of Project Settings.


#include "Ferret.h"

void AFerret::BeginPlay()
{
	Super::BeginPlay();
	WaitTimer = WaitDuration;
}

void AFerret::Tick(float DeltaTime)
{
	WaitTimer -= GetWorld()->DeltaRealTimeSeconds;
}

void AFerret::Survive()
{
	if (bIsLookingForSpot) 
	{
		if (WaitTimer <= 0.f)
		{
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			if (!NavSystem)
				return;

			FNavLocation targetFNavLocation;
			if (Burrow != nullptr)
				NavSystem->GetRandomReachablePointInRadius(Burrow->GetActorLocation(), PatrolRadius, targetFNavLocation);
			else
				NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), PatrolRadius, targetFNavLocation);
			TargetLocation = targetFNavLocation.Location;
			bIsLookingForSpot = false;
		}
	}
	else
	{
		if (FVector::Dist(GetActorLocation(), TargetLocation) <= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f)
		{
			WaitTimer = WaitDuration;
			bIsLookingForSpot = true;
		}
	}
}

void AFerret::Flee()
{
	TargetLocation = Burrow->GetActorLocation();
	if (FVector::Dist(GetActorLocation(), TargetLocation) <= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f)
	{
		Destroy();
	}
}

void AFerret::ApplyEffect(APlayerCharacter* player)
{
}
