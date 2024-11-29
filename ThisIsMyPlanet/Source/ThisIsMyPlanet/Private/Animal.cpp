// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAnimal::Survive()
{

}

void AAnimal::Flee()
{

}

void AAnimal::Sleep()
{
	SleepTimer -= GetWorld()->DeltaTimeSeconds;
	if (SleepTimer <= 0.f)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("woke up!"));
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetMesh()->SetSimulatePhysics(false);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		SleepTimer = SleepDuration;
		bIsSleeping = false;
	}
	
}

void AAnimal::ApplyEffect()
{

}
