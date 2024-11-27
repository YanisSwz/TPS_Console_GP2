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
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 500.0f, targetFNavLocation);
		targetLocation = targetFNavLocation.Location;
		bIsLookingForSpot = false;
	}
	if ((GetActorLocation() - targetLocation).Length() <= 160.0f && !bIsEating)
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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am fleeig"));

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
	targetLocation = GetActorLocation() - (playerLocation - GetActorLocation());
}

void AChicken::Sleep()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am honk mimimiming"));
}

void AChicken::ApplyEffect()
{

}