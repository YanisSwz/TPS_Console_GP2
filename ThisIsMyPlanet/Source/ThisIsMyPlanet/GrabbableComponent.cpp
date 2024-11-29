// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UGrabbableComponent::UGrabbableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabbableComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabbableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrabbableComponent::AttachTo(AActor* parent, FName socketName)
{
	GetOwner()->AttachToComponent(parent->GetComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), socketName);

	FVector socketPos = parent->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(socketName);

	GetOwner()->SetActorLocation(socketPos);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetSimulatePhysics(false);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UGrabbableComponent::Launch(FVector dir, float power)
{
	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetSimulatePhysics(true);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, dir.ToString());

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->AddImpulse(dir * power, NAME_None, true);
}
