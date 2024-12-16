// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"

void ABear::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABush::StaticClass(), bushList);
	PointValue = 200;
}

void ABear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}

	SleepTimer -= DeltaTime;
	if (SleepTimer <= 0.0f)
	{
		SleepTimer = 0.0f;
		BerryCount = 0;
	}

}

void ABear::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("BEAR"));*/
	

	if (SleepTimer > 0.0f)
	{
		// honk mimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimi
	}
	else
	{
		if (bIsLookingForSpot)
		{
			
			float dist = 10000.0f;
			AActor* randomBush = bushList[FMath::RandRange(0, bushList.Num() - 1)];
			

			if (randomBush == nullptr)
			{
				TargetLocation = GetActorLocation();
			}
			else
			{
				TargetLocation = randomBush->GetActorLocation();
			}

			bIsLookingForSpot = false;
		}
		if ((GetActorLocation() - TargetLocation).Length() <= 160.0f && !bIsEating)
		{

			bIsEating = true;
			EatingTimer = EatingTime;
		}
		if (bIsEating && EatingTimer <= 0.0f)
		{
			bIsEating = false;
			bIsLookingForSpot = true;
			++BerryCount;
		}
		if (BerryCount >= 5)
		{
			SleepTimer = 15.0f;
		}
	}
	
	
	bHasAttacked = false;
}

void ABear::Flee()
{
	FVector playerLocation;
	FVector player1Location = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector player2Location = UGameplayStatics::GetPlayerPawn(GetWorld(), 1)->GetActorLocation();
	APawn* nearestPlayer;
	if ((player1Location - GetActorLocation()).Length() < (player2Location - GetActorLocation()).Length())
	{
		playerLocation = player1Location;
		nearestPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
	else
	{
		playerLocation = player2Location;
		nearestPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);
	}

	if (!bHasAttacked)
	{

		TargetLocation = playerLocation;
		if ((GetActorLocation() - TargetLocation).Length() <= 160.0f)
		{
			APlayerCharacter* p = Cast<APlayerCharacter>(nearestPlayer);
			if (p != nullptr)
			{
				ApplyEffect(p);
				bHasAttacked = true;
			}
		}
	}
	else
	{
		TargetLocation = GetActorLocation() - (playerLocation - GetActorLocation());
	}
}

void ABear::ApplyEffect(APlayerCharacter* player)
{
	player->Stun(StunTime);
	Cast<ACharacter>(player)->GetMesh()->AddImpulse((player->GetActorLocation() - GetActorLocation()) * HitForce + HitForceBonus, NAME_None, true);
}