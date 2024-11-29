// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"

void ABear::BeginPlay()
{
	Super::BeginPlay();
}

void ABear::Tick(float DeltaTime)
{


}

void ABear::Survive()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("BEAR"));

	float deltaTime = GetWorld()->GetDeltaSeconds();
	eatingTimer -= deltaTime;
	if (eatingTimer < 0.0f)
	{
		eatingTimer = 0.0f;
	}

	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		FNavLocation targetFNavLocation;
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), berrySearchingRadius, targetFNavLocation);
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

void ABear::Flee()
{
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
	targetLocation = playerLocation;
	if ((GetActorLocation() - targetLocation).Length() <= 160.0f)
	{
		ApplyEffect();
	}
}

void ABear::ApplyEffect()
{
	// TODO: BIG AHH KNOCKBACK
}