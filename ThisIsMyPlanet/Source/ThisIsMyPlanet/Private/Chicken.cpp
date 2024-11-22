// Fill out your copyright notice in the Description page of Project Settings.


#include "Chicken.h"

void AChicken::Survive()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("I am a chimken"));
}

void AChicken::Flee()
{

}

void AChicken::Sleep()
{

}

void AChicken::ApplyEffect()
{

}