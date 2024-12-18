// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"
#include "ChickenDirector.h"

void AFox::BeginPlay()
{
	Super::BeginPlay();
	InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SoundTimer = MaxSoundTimer + FMath::RandRange(-SoundTimerRandOffset, SoundTimerRandOffset);
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

	AfterAttackTimer -= DeltaTime;
	if (AfterAttackTimer < 0.0f)
	{
		AfterAttackTimer = 0.0f;
	}

	if (!bIsSleeping)
	{
		SoundTimer -= DeltaTime;
		if (SoundTimer < 0.0f)
		{
			SoundTimer = MaxSoundTimer + FMath::RandRange(-SoundTimerRandOffset, SoundTimerRandOffset);
			UGameplayStatics::PlaySoundAtLocation(this, FoxAmbientSound, this->GetActorLocation(), 1.f, FMath::RandRange(0.8f, 1.2f), 0.0f, FoxSoundAttenuation);
		}
	}
}

void AFox::Survive()
{
	if (GetCharacterMovement()->GetMaxSpeed() != InitialSpeed)
		GetCharacterMovement()->MaxWalkSpeed = InitialSpeed;
	
	if (NearestChicken == nullptr)
	{
		NearestChicken = UGameplayStatics::FindNearestActor(GetActorLocation(), GetWorld()->GetGameInstance()->GetSubsystem<UChickenDirector>()->GetChickens(), NearestChickenDistance);
	}
	if (NearestChickenDistance < ChickenSpottingRange && HungerTimer <= 0.0f && NearestChicken != nullptr)
	{
		TargetLocation = NearestChicken->GetActorLocation();
		if (FVector::Dist(TargetLocation, GetActorLocation()) < EatingDistance)
		{
			if (AttackAnimation != nullptr)
				PlayAnimMontage(AttackAnimation);
			FVector SpawnLocation = FVector(FMath::RandRange(-2000.0f, 2000.0f), FMath::RandRange(-2000.0f, 2000.0f), 50000.0f);
			if (FMath::Abs(SpawnLocation.X) < 500.0f && FMath::Abs(SpawnLocation.Y) < 500.0f) SpawnLocation = FVector(1000.0f, 1000.0f, 50000.0f);
			NearestChicken->SetActorLocation(SpawnLocation);
			HungerTimer = HungerTime;

			NearestChicken = nullptr;
			NearestChickenDistance = ChickenSpottingRange + 100.f;
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
			TargetLocation = GetActorLocation() + FVector(MoveStep, 0.0f, 0.0f).RotateAngleAxis(ScoutStepAngle, FVector::UpVector);
			bIsWaitingAfterAttack = false;
		}
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
