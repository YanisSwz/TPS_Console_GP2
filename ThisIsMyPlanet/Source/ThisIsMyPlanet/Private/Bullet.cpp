// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ABullet::ABullet()
{
	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Init");

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);

	//cree le collision profil
	//CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;


	/*FVector inpulse = FVector(100.0f, 0.f, 0.f);

	ProjectileMovement->AddForce(inpulse);*/

	InitialLifeSpan = 5.0f;

}

void ABullet::FireInDirection(FVector ShootDirection)
{
	//ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	CollisionComp->AddImpulse(ShootDirection, NAME_None, true);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Hit");

	//endormir les animeaux
	Destroy();
}

