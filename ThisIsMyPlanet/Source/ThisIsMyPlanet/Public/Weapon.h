// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bullet.h"
#include "Weapon.generated.h"

UCLASS()
class THISISMYPLANET_API AWeapon : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABullet> bullet;

public:
	// Sets default values for this pawn's properties
	AWeapon();

	bool bCanShoot = true;

	float shootingStrenght = 20;

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
