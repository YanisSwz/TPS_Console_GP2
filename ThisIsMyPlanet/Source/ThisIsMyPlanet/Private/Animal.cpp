// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "Bullet.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsGrabbed = true;
	GrabbableComp = CreateDefaultSubobject<UGrabbableComponent>(TEXT("GrabbableComponent"));
	bIsSleeping = false;
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AAnimal::OnAnimalHit);
	SleepTimer = SleepDuration;
	PlayerInvincibilityTimer = MaxPlayerInvincibilityTimer;
	Health = MaxHealth;
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
		FVector inertia = GetMesh()->GetBoneLinearVelocity(NAME_None);
		GrabbableComp->UnGrab();
		GrabbableComp->SetIsGrabbable(false);
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCharacterMovement()->GravityScale = 1;
		GetMesh()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("") + BaseMeshOffset);
		GetCapsuleComponent()->ResetSceneVelocity();
		GetMesh()->SetRelativeLocationAndRotation(-BaseMeshOffset, BaseMeshRotation);
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->Velocity = inertia;
		SleepTimer = SleepDuration;
		bIsSleeping = false;
		Health = MaxHealth;
	}
}

void AAnimal::ApplyEffect(APlayerCharacter* player)
{

}

void AAnimal::SetUngrabbed()
{
	bIsGrabbed = false;
	bIsPlayerInvincible = true;
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
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), "How the F did you got grabbed by nothing???");
		return;
	}

	LastGrabbedBy = Cast<APlayerCharacter>(actor);
	bIsGrabbed = true;
}

void AAnimal::ResetMeshPos()
{
	GetMesh()->SetRelativeLocationAndRotation(-BaseMeshOffset, BaseMeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	
}

void AAnimal::OnAnimalHit(AActor* _SelfActor, AActor* _OtherActor, FVector _NormalImpulse, const FHitResult& _Hit)
{
	if (_OtherActor != nullptr)
	{
		if (bIsActive)
		{
			if (!bIsGrabbed && SleepTimer > 0.0001f && GetMesh()->GetBoneLinearVelocity(NAME_None).Length() > MinVelocityToApplyEffect)
			{
				APlayerCharacter* player = Cast<APlayerCharacter, AActor>(_OtherActor);

				if (player != nullptr && (LastGrabbedBy != player || !bIsPlayerInvincible))
				{
					if (GEngine != nullptr)
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), player->GetActorNameOrLabel());
					if (GEngine != nullptr)
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), LastGrabbedBy->GetActorNameOrLabel());
					if (GEngine != nullptr)
						GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::MakeRandomColor(), FString::SanitizeFloat(bIsPlayerInvincible));
					ApplyEffect(player);
					SleepTimer = 0.f;
				}
			}

			ABullet* bullet = Cast<ABullet>(_OtherActor);

			if (bullet != nullptr)
			{
				if (!bIsSleeping)
				{
					--Health;
					if (Health <= 0)
					{
						GetMesh()->SetSimulatePhysics(true);

						GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						GetCharacterMovement()->GravityScale = 0;
						GetCharacterMovement()->Velocity = FVector::Zero();
						GetCapsuleComponent()->ResetSceneVelocity();

						GetCharacterMovement()->StopMovementImmediately();
						GetCharacterMovement()->SetMovementMode(MOVE_None);
						bIsSleeping = true;
						GrabbableComp->SetIsGrabbable(true);
					}
				}
				_OtherActor->Destroy();
			}
		}

	}
}
