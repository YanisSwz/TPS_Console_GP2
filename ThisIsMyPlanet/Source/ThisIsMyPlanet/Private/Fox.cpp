// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"
#include "ChickenDirector.h"

void AFox::BeginPlay()
{
	Super::BeginPlay();
	PointValue = 2000;
	InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
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

	if(bIsSleeping && GetCharacterMovement()->GetMaxSpeed() != InitialSpeed)
		GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
}

void AFox::Survive()
{
	if (GetCharacterMovement()->GetMaxSpeed() != InitialSpeed)
		GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;

	float dist = 1000.0f;
	AActor* nearestChicken = UGameplayStatics::FindNearestActor(GetActorLocation(), GetWorld()->GetGameInstance()->GetSubsystem<UChickenDirector>()->GetChickens(), dist);
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
		}
	}
	else
	{
		TargetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(ScoutStepAngle, FVector::UpVector);
	}
	
}

void AFox::Flee()
{
	if (GetCharacterMovement()->GetMaxSpeed() != FleeSpeed)
		GetCharacterMovement()->MaxWalkSpeed = FleeSpeed;
	TargetLocation = GetActorLocation() - (ClosestPlayer->GetActorLocation() - GetActorLocation());
}

void AFox::ApplyEffect(APlayerCharacter* player)
{
	player->Stun(StunTime);
}
