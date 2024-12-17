// Fill out your copyright notice in the Description page of Project Settings.


#include "Ferret.h"

bool AFerret::GetIsTurning()
{
	return bIsTurning;
}

void AFerret::BeginPlay()
{
	Super::BeginPlay();
	WaitTimer = 0.f;
	AxisVector = FVector(0.f, 0.f, 1.f);
	RotationCount = 0.f;
	bIsLookingForSpot = true;
	bAppliedSnare = false;
	bIsTurning = false;
	BuryTimer = 0.f;
}

void AFerret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaitTimer -= GetWorld()->DeltaRealTimeSeconds;

	if(bIsBuried)
	{
		BuryTimer -= GetWorld()->DeltaRealTimeSeconds;
		if (BuryTimer <= 0.f)
			Emerge();
	}
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

			FNavLocation TargetFNavLocation;
			if (Burrow != nullptr)
				NavSystem->GetRandomReachablePointInRadius(Burrow->GetActorLocation(), PatrolRadius, TargetFNavLocation);
			else
				NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), PatrolRadius, TargetFNavLocation);
			TargetLocation = TargetFNavLocation.Location;
			bIsLookingForSpot = false;
		}
	}
	else
	{
		if (bReachedDestination)
		{
			WaitTimer = WaitDuration;
			bIsLookingForSpot = true;
		}
	}
}

void AFerret::Flee()
{
	if (FVector::Dist(GetActorLocation(), Burrow->GetActorLocation()) >= MaxAggroRange)
		RotationCount = NumberOfRotations;

	if (RotationCount < NumberOfRotations)
	{
		if (FVector::Dist(GetActorLocation(), ClosestPlayer->GetActorLocation()) > RotationDistance && !bIsTurning)
		{
			TargetLocation = ClosestPlayer->GetActorLocation();
		}
		else
		{
			if (!bIsTurning)
				bIsTurning = true;
			if (!bAppliedSnare) 
			{
				ApplyEffect(ClosestPlayer);
				bAppliedSnare = true;
			}

			FVector NewLocation = ClosestPlayer->GetActorLocation();

			AngleAxis += GetWorld()->GetDeltaSeconds() * RotationSpeed;

			if (AngleAxis >= 360.0f)
			{
				++RotationCount;
				AngleAxis = 0.f;
			}

			FVector RotateValue = FVector(RotationDistance, 0.f, 0.f).RotateAngleAxis(AngleAxis, AxisVector);

			FRotator Rotation = FRotator(0.f, AngleAxis + 90.f, 0.f);
			SetActorRotation(Rotation);

			NewLocation.X += RotateValue.X;
			NewLocation.Y += RotateValue.Y;
			NewLocation.Z += RotateValue.Z;

			SetActorLocation(NewLocation);
			TargetLocation = NewLocation;
		}
	}
	else
	{
		bIsTurning = false;
		if (Burrow != nullptr)
		{
			if (TargetLocation != Burrow->GetActorLocation())
			{
				TargetLocation = Burrow->GetActorLocation();
			}
			if (bReachedDestination && !bIsBuried)
			{
				Bury();
			}
		}
	}
}

void AFerret::ApplyEffect(APlayerCharacter* player)
{
	player->Snare(SnareDuration, SlowAmount);
}

void AFerret::Bury()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	BuryTimer = BuryDuration;
	bIsBuried = true;
}

void AFerret::Emerge()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bIsBuried = false;
	RotationCount = 0;
	WaitTimer = 0.f;
	bAppliedSnare = false;
}
