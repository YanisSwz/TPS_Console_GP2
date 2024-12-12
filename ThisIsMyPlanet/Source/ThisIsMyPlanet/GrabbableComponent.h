// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabbableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THISISMYPLANET_API UGrabbableComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	float LaunchPowerMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	bool bIsTwoSlots = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	bool bIsGrabbable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FName GrabSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
	FQuat GrabQuaternion;

	AActor* Grabbed = nullptr;

public:	
	// Sets default values for this component's properties
	UGrabbableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AttachTo(AActor* parent, FName socketName);

	void Launch(FVector dir);

	void UnGrab();

	bool GetIsGrabbable();

	bool GetIsTwoSlots();

	void SetIsGrabbable(bool bGrabbable);
};
