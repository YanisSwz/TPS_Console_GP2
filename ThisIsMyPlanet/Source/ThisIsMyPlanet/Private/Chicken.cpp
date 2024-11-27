// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"


void AChicken::BeginPlay()
{
	Super::BeginPlay();
	bIsLookingForSpot = true;
}

void AChicken::Survive()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am survivig"));

	if (bIsLookingForSpot)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSystem)
			return;

		NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 5000.0f, targetLocation);
		bIsLookingForSpot = false;
	}
	
}

void AChicken::Flee()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am fleeig"));
}

void AChicken::Sleep()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am honk mimimiming"));
}

void AChicken::ApplyEffect()
{

}