// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"

bool ABear::GetIsNapping()
{
	return bIsNapping;
}

void ABear::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABush::StaticClass(), bushList);
}

void ABear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}

	NapTimer -= DeltaTime;
	if (NapTimer <= 0.0f && BerryCount >= 2)
	{
		NapTimer = 0.0f;
		bIsNapping = false;
		BerryCount = 0;
	}

	AfterAttackTimer -= DeltaTime;
	if (AfterAttackTimer < 0.0f)
	{
		AfterAttackTimer = 0.0f;
	}
}

void ABear::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("BEAR"));*/
	

	if (NapTimer > 0.0f)
	{
		// honk mimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimimi
		
	}
	else
	{
		if (bIsLookingForSpot)
		{
			AnimalController->SightConfig->SightRadius = SightRadius;
			AnimalController->HearingConfig->HearingRange = HearingRange;
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
		if (BerryCount >= 2)
		{
			if (AnimalController != nullptr) 
			{
				AnimalController->SightConfig->SightRadius = 0.f;
				AnimalController->HearingConfig->HearingRange = HearingRange / 2.f;
			}
			NapTimer = 15.0f;
			bIsNapping = true;
		}
	}
	
	bHasAttacked = false;
}

void ABear::Flee()
{
	if (bIsNapping) 
	{
		if (AnimalController != nullptr)
		{
			AnimalController->SightConfig->SightRadius = SightRadius;
			AnimalController->HearingConfig->HearingRange = HearingRange;
		}
		NapTimer = 0.0f;
		bIsNapping = false;
		BerryCount = 0;
	}
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
				if (AttackAnimation != nullptr)
					PlayAnimMontage(AttackAnimation);
				ApplyEffect(p);
				bHasAttacked = true;
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
			TargetLocation = GetActorLocation() - (playerLocation - GetActorLocation());
			bIsWaitingAfterAttack = false;
		}
		

		
	}
}

void ABear::ApplyEffect(APlayerCharacter* player)
{
	player->Stun(StunTime);
	Cast<ACharacter>(player)->GetMesh()->AddImpulse((player->GetActorLocation() - GetActorLocation()) * HitForce + HitForceBonus, NAME_None, true);
}

void ABear::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AnimalController = Cast<AAnimalController>(NewController);
}
