// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"
#include "NavigationSystem.h"

void AChicken::BeginPlay()
{
	Super::BeginPlay();
}

void AChicken::Survive()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am survivig"));

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
		return;

	FNavLocation PatrolPoint;
	NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), 500.0f, PatrolPoint);

	
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