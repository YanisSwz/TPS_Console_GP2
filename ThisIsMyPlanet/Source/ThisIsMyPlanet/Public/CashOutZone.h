// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CashOutZone.generated.h"

UCLASS()
class THISISMYPLANET_API ACashOutZone : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DetectionZone", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DetectionBox;

public:	
	// Sets default values for this actor's properties
	ACashOutZone();

	int bluePlayerPoints = 0;
	int redPlayerPoints = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
