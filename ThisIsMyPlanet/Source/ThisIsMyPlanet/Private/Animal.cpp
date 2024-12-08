// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "Bullet.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bHasTouchedGround = true;
	GrabbableComp = CreateDefaultSubobject<UGrabbableComponent>(TEXT("GrabbableComponent"));
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AAnimal::OnAnimalHit);
	SleepTimer = SleepDuration;
	PlayerInvincibilityTimer = MaxPlayerInvincibilityTimer;
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayerInvincible)
	{
		PlayerInvincibilityTimer -= GetWorld()->DeltaTimeSeconds;
		if (PlayerInvincibilityTimer <= 0.f)
		{
			bIsPlayerInvincible = false;
			PlayerInvincibilityTimer = MaxPlayerInvincibilityTimer;
		}
	}
}

void AAnimal::Survive()
{

}

void AAnimal::Flee()
{

}

void AAnimal::Sleep()
{
	SleepTimer -= GetWorld()->DeltaTimeSeconds;
	if (SleepTimer <= 0.f)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("woke up!"));
		GrabbableComp->UnGrab();
		GrabbableComp->SetIsGrabbable(false);
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetMesh()->SetSimulatePhysics(false);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		SleepTimer = SleepDuration;
		bIsSleeping = false;
	}
}

void AAnimal::ApplyEffect(APlayerCharacter* player)
{

}

void AAnimal::UntouchGround()
{
	bHasTouchedGround = false;
}

void AAnimal::SetReachedDestination(bool bResult)
{
	bReachedDestination = bResult;
}

void AAnimal::SetLastGrabbedBy(AActor* actor)
{
	if (actor == nullptr)
	{
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), "How the F did you got ungrabbed by nothing???");
		return;
	}

	LastGrabbedBy = Cast<APlayerCharacter>(actor);
	bIsPlayerInvincible = true;
}

void AAnimal::OnAnimalHit(AActor* _SelfActor, AActor* _OtherActor, FVector _NormalImpulse, const FHitResult& _Hit)
{
	if (_OtherActor != nullptr)
	{
		if (bIsActive)//
		{
			if (!bHasTouchedGround)
			{
				APlayerCharacter* player = Cast<APlayerCharacter, AActor>(_OtherActor);

				if (player != nullptr && LastGrabbedBy != player)
				{
					ApplyEffect(player);
				}

				if (_OtherActor->Tags.Contains("Ground"))
				{
					bHasTouchedGround = true;
				}
			}

			ABullet* bullet = Cast<ABullet>(_OtherActor);

			if (bullet != nullptr)
			{

				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, "Hit");

				//GetCapsuleComponent()->SetSimulatePhysics(true);
				//GetMesh()->SetSimulatePhysics(true);

				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				GetCharacterMovement()->GravityScale = 0;
				GetCharacterMovement()->Velocity = FVector::Zero();
				GetCapsuleComponent()->ResetSceneVelocity();

				GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("pelvis", 1.f);

				GetCharacterMovement()->SetMovementMode(MOVE_None);
				bIsSleeping = true;
				GrabbableComp->SetIsGrabbable(true);
				_OtherActor->Destroy();
			}
		}

	}
}
