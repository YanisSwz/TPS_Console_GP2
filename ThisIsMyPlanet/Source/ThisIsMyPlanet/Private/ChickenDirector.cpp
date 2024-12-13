// Fill out your copyright notice in the Description page of Project Settings.


#include "ChickenDirector.h"

UChickenDirector::UChickenDirector()
{
}

void UChickenDirector::AddChicken(AActor* Actor)
{
	if(Cast<AChicken>(Actor) != nullptr)
		Chickens.Add(Actor);
}

void UChickenDirector::RemoveChicken(AActor* Actor)
{
	Chickens.Remove(Actor);
}

TArray<AActor*> UChickenDirector::GetChickens()
{
	return Chickens;
}

int UChickenDirector::GetChickenCount()
{
	return Chickens.Num();
}
