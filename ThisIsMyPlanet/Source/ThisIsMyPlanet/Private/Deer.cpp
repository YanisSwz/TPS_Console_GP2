// Fill out your copyright notice in the Description page of Project Settings.


#include "Deer.h"

void ADeer::BeginPlay()
{
	Super::BeginPlay();
	TurnTimer = TurnTime;
}

void ADeer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}
	TurnTimer -= DeltaTime;
	if (TurnTimer <= 0.0f)
	{
		TurnTimer = TurnTime;
		
		bIsTurningLeft = !bIsTurningLeft;
	}
	else if (TurnTimer <= 0.5f * TurnTime)
	{
		if (bHasAttacked) Jump();
	}
}

void ADeer::Survive()
{
	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		
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
		if ((GetActorLocation() - TargetLocation).Length() <= AttackRange)
		{
			if (ClosestPlayer != nullptr)
			{
				if (AttackAnimation != nullptr)
					PlayAnimMontage(AttackAnimation);
				ApplyEffect(ClosestPlayer);
				bHasAttacked = true;
				TurnTimer = TurnTime;
			}
		}
	}
	else
	{
		FVector projectedClosestPlayerLocation = FVector(ClosestPlayer->GetActorLocation().X, ClosestPlayer->GetActorLocation().Y, 0.0f);
		if (bIsTurningLeft)
		{
			TargetLocation = GetActorLocation() - (projectedClosestPlayerLocation - GetActorLocation()).RotateAngleAxis(-45.0f, FVector::UpVector);
		}
		else
		{
			TargetLocation = GetActorLocation() - (projectedClosestPlayerLocation - GetActorLocation()).RotateAngleAxis(45.0f, FVector::UpVector);
		}
	}
}

void ADeer::ApplyEffect(APlayerCharacter* player)
{
	player->Stun(StunTime);
	Cast<ACharacter>(player)->GetMesh()->AddImpulse((player->GetActorLocation() - GetActorLocation()) * HitForce + HitForceBonus, NAME_None, true);
}