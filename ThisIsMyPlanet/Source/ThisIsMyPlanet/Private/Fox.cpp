// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"

void AFox::BeginPlay()
{
	Super::BeginPlay();
}

void AFox::Tick(float DeltaTime)
{
	
	
}

void AFox::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am survivig"));*/

	float deltaTime = GetWorld()->GetDeltaSeconds();

	if (bIsTurningLeft)
	{
		scoutStepAngle += turningSpeed * deltaTime;
		
	}
	else
	{
		scoutStepAngle -= turningSpeed * deltaTime;
	}
	if (FMath::RandRange(0, FMath::FloorToInt(turningInverseFrequency * deltaTime)) == 1) bIsTurningLeft = !bIsTurningLeft;
	if (FMath::RandRange(0, FMath::FloorToInt(jumpingInverseFrequency * deltaTime)) == 1) Jump();

	hungerTimer -= deltaTime;
	if (hungerTimer <= 0.0f) hungerTimer = 0.0f;

	TArray<AActor*> chickenList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChicken::StaticClass(), chickenList);
	float dist = 1000.0f;
	AActor* nearestChicken = UGameplayStatics::FindNearestActor(GetActorLocation(), chickenList, dist);
	if (nearestChicken == nullptr)
	{
		targetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(scoutStepAngle, FVector::UpVector);
		
	}
	else if ((nearestChicken->GetActorLocation() - GetActorLocation()).Length() < 3000.0f && hungerTimer <= 0.0f)
	{
		targetLocation = nearestChicken->GetActorLocation();
		if ((targetLocation - GetActorLocation()).Length() < 100.0f)
		{
			// TODO: ATTACC
			nearestChicken->Destroy();
			hungerTimer = hungerTime;
		}
	}
	else
	{
		targetLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f).RotateAngleAxis(scoutStepAngle, FVector::UpVector);
	}
	
}

void AFox::Flee()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am fleeig"));*/

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

	float deltaTime = GetWorld()->GetDeltaSeconds();
	if (FMath::RandRange(0, FMath::FloorToInt(jumpingInverseFrequency * deltaTime)) == 1) Jump();
}

void AFox::ApplyEffect(APlayerCharacter* player)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am bonkig"));

	player->Stun(StunTime);
}
