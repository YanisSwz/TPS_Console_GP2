// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrabbableComponent.h"
#include "Components/ActorComponent.h"
#include "GrabberComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THISISMYPLANET_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

	AActor* owner;

	UGrabbableComponent* leftGrabbedComp = nullptr;

	UGrabbableComponent* rightGrabbedComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FName leftHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FName rightHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FVector HalfSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> Actors;


public:	
	// Sets default values for this component's properties
	UGrabberComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool Grab(bool bIsRightHand);

	void Launch(bool bIsRightHand, float launchPower);
};
