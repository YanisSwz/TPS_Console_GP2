// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bullet.h"
#include "Weapon.generated.h"

UCLASS()
class THISISMYPLANET_API UWeapon : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> bullet;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TArray<class ABullet*> bullets;*/

	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

public:
	// Sets default values for this pawn's properties
	UWeapon();

	bool bCanShoot = true;
	float reloadTimer = 0;

	float shootingStrenght = 20;

	void Fire(FVector LaunchDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
