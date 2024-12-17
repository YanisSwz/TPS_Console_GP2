// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"

// Sets default values
UWeapon::UWeapon()
{
}

void UWeapon::Fire(FVector LaunchDirection)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector CameraLocation;
		FRotator CameraRotation;
		GetOwner()->GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		//World->SpawnActor<ABullet>(ABullet::StaticClass(),  MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		ABullet* b = World->SpawnActor<ABullet>(bullet, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
		if(b)
			b->FireInDirection(LaunchDirection * 4000.f);
	}
	else
	{
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "World is nullptr");
	}
}

 //Called when the game starts or when spawned
//void UWeapon::BeginPlay()
//{
//	Super::BeginPlay();
//	UE_LOG(LogTemp, Error, TEXT("BBBBB"));
//	if (GEngine != nullptr)
//		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "bbbbbbbbbbb");
//	
//}

// Called every frame
//void UWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	UE_LOG(LogTemp, Error, TEXT("AAAA"));
//	if (GEngine != nullptr)
//		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "aaaaaaaaaaaaaaaaaaaaaaa");
//
//	//changer de place (dans le player)
//	if (!bCanShoot)
//	{
//
//		if (GEngine != nullptr)
//			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::SanitizeFloat(DeltaTime) );
//
//		reloadTimer -= DeltaTime;
//		if (reloadTimer <= 0.f)
//			bCanShoot = true;
//	}
//}
