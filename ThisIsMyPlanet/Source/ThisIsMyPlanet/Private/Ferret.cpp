// Fill out your copyright notice in the Description page of Project Settings.


#include "Ferret.h"

void AFerret::BeginPlay()
{
	Super::BeginPlay();
	WaitTimer = 0.f;
	AxisVector = FVector(0.f, 0.f, 1.f);
	RotationCount = 0.f;
	bIsLookingForSpot = true;
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
	if (RotationCount < NumberOfRotations)
	{
		if (FVector::Dist(GetActorLocation(), TargetLocation) > RotationDistance)
			TargetLocation = ClosestPlayer->GetActorLocation();
		else
		{
			FVector NewLocation = ClosestPlayer->GetActorLocation();

			AngleAxis += GetWorld()->GetDeltaSeconds() * RotationSpeed;

			if (AngleAxis >= 360.0f)
			{
				++RotationCount;
				AngleAxis = 0.f;
			}

			FVector RotateValue = FVector(RotationDistance, 0.f, 0.f).RotateAngleAxis(AngleAxis, AxisVector);

			NewLocation.X += RotateValue.X;
			NewLocation.Y += RotateValue.Y;
			NewLocation.Z += RotateValue.Z;

			SetActorLocation(NewLocation);
			TargetLocation = NewLocation;
		}
	}
	else
	{
		TargetLocation = Burrow->GetActorLocation();
		if (FVector::Dist(GetActorLocation(), TargetLocation) <= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f)
		{
			Destroy();
		}
	}
}

void AFerret::ApplyEffect(APlayerCharacter* player)
{
}
