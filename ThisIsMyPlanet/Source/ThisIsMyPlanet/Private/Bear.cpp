// Fill out your copyright notice in the Description page of Project Settings.


#include "Bear.h"

bool ABear::GetIsNapping()
{
	return bIsNapping;
}

void ABear::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABush::StaticClass(), BushList);
	PointValue = 200;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
			AActor* randomBush = BushList[FMath::RandRange(0, BushList.Num() - 1)];
			

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
		if ((GetActorLocation() - TargetLocation).Length() <= EatingRange && !bIsEating)
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
		if (BerryCount >= MaxBerries)
		{
			if (AnimalController != nullptr) 
			{
				AnimalController->SightConfig->SightRadius = 0.f;
				AnimalController->HearingConfig->HearingRange = HearingRange * 0.5f;
			}
			NapTimer = NapTime;
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
	
	if (!bHasAttacked)
	{
		GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
		TargetLocation = ClosestPlayer->GetActorLocation();
		if ((GetActorLocation() - TargetLocation).Length() <= AttackRange)
		{
			if (AttackAnimation != nullptr)
				PlayAnimMontage(AttackAnimation);
			ApplyEffect(ClosestPlayer);
			bHasAttacked = true;
		}
	}
	else
	{
		if (!bIsWaitingAfterAttack)
		{
			AfterAttackTimer = AfterAttackTime;
			bIsWaitingAfterAttack = true;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
		if (bIsWaitingAfterAttack && AfterAttackTimer <= 0.0f)
		{
			TargetLocation = GetActorLocation() - (ClosestPlayer->GetActorLocation() - GetActorLocation());
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
