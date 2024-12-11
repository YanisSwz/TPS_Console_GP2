// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "../PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "CashOutZone.generated.h"

UCLASS()
class THISISMYPLANET_API ACashOutZone : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DetectionZone", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DetectionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "player", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* player1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "player", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* player2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Points", meta = (AllowPrivateAccess = "true"))
	float bluePlayerPoints = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Points", meta = (AllowPrivateAccess = "true"))
	float redPlayerPoints = 0;

public:	
	// Sets default values for this actor's properties
	ACashOutZone();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeBlueScore();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeRedScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
