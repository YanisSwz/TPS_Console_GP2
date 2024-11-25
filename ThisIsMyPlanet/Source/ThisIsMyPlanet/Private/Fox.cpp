// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"

void AFox::BeginPlay()
{
	Super::BeginPlay();
}

void AFox::Survive()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am survivig"));*/
}

void AFox::Flee()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am fleeig"));
}

void AFox::Sleep()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx and I am honk mimimiming"));
}

void AFox::ApplyEffect()
{

}
