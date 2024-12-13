// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Chicken.h"
#include "ChickenDirector.generated.h"

/**
 * 
 */

UCLASS()
class THISISMYPLANET_API UChickenDirector : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Chickens;

public:
	UChickenDirector();

	UFUNCTION(BlueprintCallable)
	void AddChicken(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemoveChicken(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetChickens();

	UFUNCTION(BlueprintCallable)
	int GetChickenCount();


};
