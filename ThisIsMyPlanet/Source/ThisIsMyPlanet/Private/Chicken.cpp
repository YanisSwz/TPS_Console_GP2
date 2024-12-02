// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"


void AChicken::BeginPlay()
{
	Super::BeginPlay();
	bIsLookingForSpot = true;
	bIsEating = false;
	eatingTimer = 0.0f;
}

void AChicken::Tick(float DeltaTime)
{
	eatingTimer -= DeltaTime;
	if (eatingTimer < 0.0f)
	{
		eatingTimer = 0.0f;
	}
}

void AChicken::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am survivig"));*/
	

	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		FNavLocation targetFNavLocation;
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), seedSearchingRadius, targetFNavLocation);
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
}

void AChicken::Flee()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am fleeig"));*/
	TargetLocation = GetActorLocation() - (ClosestPlayer->GetActorLocation() - GetActorLocation());
}

void AChicken::ApplyEffect(APlayerCharacter* player)
{

}