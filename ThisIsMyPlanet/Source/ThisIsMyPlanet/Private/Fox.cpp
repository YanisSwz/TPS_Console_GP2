// Fill out your copyright notice in the Description page of Project Settings.


#include "Fox.h"

void AFox::Survive()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("I am a fomx"));
}

void AFox::Flee()
{

}

void AFox::Sleep()
{

}

void AFox::ApplyEffect()
{

}
