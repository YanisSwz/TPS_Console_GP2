// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"


void AChicken::BeginPlay()
{
	Super::BeginPlay();
	bIsLookingForSpot = true;
	bIsEating = false;
	EatingTimer = 0.0f;
}

void AChicken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}

	if (bIsSleeping && bLaunched)
		bLaunched = false;
}

void AChicken::Survive()
{
	if (bLaunched)
		bLaunched = false;
	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		FNavLocation targetFNavLocation;
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), SeedSearchingRadius, targetFNavLocation);
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
}

void AChicken::Flee()
{
	if (!bLaunched)
	{
		FRotator Rota = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestPlayer->GetActorLocation());
		Rota.Roll = 0.f;
		Rota.Pitch = 0.f;
		Rota.Yaw += 180.f;
		SetActorRotation(Rota);
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, HorizontalImpulse) * FlyForce, true, true);
		bLaunched = true;
	}
	TargetLocation = GetActorLocation();
}

void AChicken::ApplyEffect(APlayerCharacter* player)
{
	player->Flash(FlashDuration);
}