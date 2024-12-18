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
	AfterAttackTimer -= DeltaTime;
	if (AfterAttackTimer < 0.0f)
	{
		AfterAttackTimer = 0.0f;
	}
}

void ADeer::Survive()
{
	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeer::StaticClass(), DeerList);
		float Dist;
		AActor* NearestDeer = UGameplayStatics::FindNearestActor(GetActorLocation(), DeerList, Dist);
		DeerList.RemoveSingle(NearestDeer);
		NearestDeer = UGameplayStatics::FindNearestActor(GetActorLocation(), DeerList, Dist);

		FNavLocation TargetFNavLocation;
		if (NearestDeer == nullptr)
		{
			NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), GrassSearchingRadius, TargetFNavLocation);
		}
		else
		{
			NavSystem->GetRandomReachablePointInRadius(NearestDeer->GetActorLocation(), GrassSearchingRadius, TargetFNavLocation);
		}
		
		TargetLocation = TargetFNavLocation.Location;
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

		if (!bIsWaitingAfterAttack)
		{
			AfterAttackTimer = AfterAttackTime;
			bIsWaitingAfterAttack = true;
		}
		if (bIsWaitingAfterAttack && AfterAttackTimer <= 0.0f)
		{
			FVector projectedClosestPlayerLocation = FVector(ClosestPlayer->GetActorLocation().X, ClosestPlayer->GetActorLocation().Y, GetActorLocation().Z);
			if (bIsTurningLeft)
			{
				TargetLocation = GetActorLocation() - (projectedClosestPlayerLocation - GetActorLocation()).RotateAngleAxis(-45.0f, FVector::UpVector);
			}
			else
			{
				TargetLocation = GetActorLocation() - (projectedClosestPlayerLocation - GetActorLocation()).RotateAngleAxis(45.0f, FVector::UpVector);
			}
			bIsWaitingAfterAttack = false;
		}

		
	}
}

void ADeer::ApplyEffect(APlayerCharacter* player)
{
	player->Stun(StunTime);
	Cast<ACharacter>(player)->GetMesh()->AddImpulse((player->GetActorLocation() - GetActorLocation()).GetSafeNormal() * HitForce + HitForceBonus, NAME_None, true);
}