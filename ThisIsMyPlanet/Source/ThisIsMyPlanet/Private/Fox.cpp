// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"

void AFox::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChicken::StaticClass(), chickenList);
	PointValue = 250;
}

void AFox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTurningLeft)
	{
		ScoutStepAngle += TurningSpeed * DeltaTime;

	}
	else
	{
		ScoutStepAngle -= TurningSpeed * DeltaTime;
	}
	if (FMath::RandRange(0, FMath::FloorToInt(TurningInverseFrequency * DeltaTime)) == 1) bIsTurningLeft = !bIsTurningLeft;
	if (FMath::RandRange(0, FMath::FloorToInt(JumpingInverseFrequency * DeltaTime)) == 1 && !bIsSleeping) Jump();

	HungerTimer -= DeltaTime;
	if (HungerTimer <= 0.0f) HungerTimer = 0.0f;
	
}

void AFox::Survive()
{
	float dist = 1000.0f;
	AActor* nearestChicken = UGameplayStatics::FindNearestActor(GetActorLocation(), chickenList, dist);
	if (nearestChicken == nullptr)
	{
		TargetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(ScoutStepAngle, FVector::UpVector);
		
	}
	else if ((nearestChicken->GetActorLocation() - GetActorLocation()).Length() < 3000.0f && HungerTimer <= 0.0f)
	{
		TargetLocation = nearestChicken->GetActorLocation();
		if ((TargetLocation - GetActorLocation()).Length() < 100.0f)
		{
			// TODO: ATTACC
			nearestChicken->Destroy();
			HungerTimer = HungerTime;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChicken::StaticClass(), chickenList);
		}
	}
	else
	{
		TargetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(ScoutStepAngle, FVector::UpVector);
	}
	
}

void AFox::Flee()
{
	TargetLocation = GetActorLocation() - (ClosestPlayer->GetActorLocation() - GetActorLocation());
}

void AFox::ApplyEffect(APlayerCharacter* player)
{
	player->Stun(StunTime);
}
