// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"

void AChicken::Survive()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken and I am survivig"));
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