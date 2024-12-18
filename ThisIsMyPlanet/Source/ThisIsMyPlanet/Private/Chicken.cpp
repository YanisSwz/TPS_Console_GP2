// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"
#include "ChickenDirector.h"

bool AChicken::GetIsFlying()
{
	return bLaunched;
}

void AChicken::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance()->GetSubsystem<UChickenDirector>()->AddChicken(this);

	bIsLookingForSpot = true;
	bIsEating = false;
	EatingTimer = 0.0f;
	SoundTimer = MaxSoundTimer + FMath::RandRange(-SoundTimerRandOffset, SoundTimerRandOffset);
}

void AChicken::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetGameInstance()->GetSubsystem<UChickenDirector>()->RemoveChicken(this);
}

void AChicken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EatingTimer -= DeltaTime;
	if (EatingTimer < 0.0f)
	{
		EatingTimer = 0.0f;
	}

	if (bIsSleeping && bLaunched)
		bLaunched = false;

	if (!bIsSleeping)
	{
		SoundTimer -= DeltaTime;
		if (SoundTimer < 0.0f)
		{
			SoundTimer = MaxSoundTimer + FMath::RandRange(-SoundTimerRandOffset, SoundTimerRandOffset);
			UGameplayStatics::PlaySoundAtLocation(this, ChickenAmbientSound, this->GetActorLocation(), 1.f, FMath::RandRange(0.8f, 1.2f), 0.0f, ChickenSoundAttenuation);
		}
	}
}

void AChicken::Survive()
{
	if (bLaunched && !GetCharacterMovement()->IsFalling())
	{
		bLaunched = false;
	}
	
	if (bLaunched)
		return;

	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		FNavLocation TargetFNavLocation;
		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), SeedSearchingRadius, TargetFNavLocation);
		TargetLocation = TargetFNavLocation.Location;
		bIsLookingForSpot = false;
	}
	else
	{
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
	}
}

void AChicken::Flee()
{
	TargetLocation = GetActorLocation();
	if (!bLaunched)
	{
		bIsEating = false;
		bIsLookingForSpot = true;
		FRotator Rota = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestPlayer->GetActorLocation());
		Rota.Roll = 0.f;
		Rota.Pitch = 0.f;
		Rota.Yaw += 180.f;
		SetActorRotation(Rota);
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, HorizontalImpulse) * FlyForce, true, true);
		bLaunched = true;
	}
	else if(!GetCharacterMovement()->IsFalling() && bLaunched)
	{
		bLaunched = false;
	}
}

void AChicken::ApplyEffect(APlayerCharacter* player)
{
	player->Flash(FlashDuration);
}