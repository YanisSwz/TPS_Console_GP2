// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbableComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animal.h"

// Sets default values for this component's properties
UGrabbableComponent::UGrabbableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGrabbableComponent::BeginPlay()
{
	Super::BeginPlay();
}


//// Called every frame
//void UGrabbableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UGrabbableComponent::AttachTo(AActor* parent, FName socketName)
{
	GetOwner()->AttachToComponent(parent->GetComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), socketName);

	FVector socketPos = parent->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(socketName);

	GetOwner()->SetActorLocation(socketPos);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetSimulatePhysics(false);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Grabbed = parent;

	AAnimal* own = Cast<AAnimal, AActor>(GetOwner());

	if (own != nullptr)
	{
		own->SetLastGrabbedBy(Grabbed);
	}
}

void UGrabbableComponent::Launch(FVector dir)
{
	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetSimulatePhysics(true);

	GetOwner()->GetComponentByClass<UCapsuleComponent>()->AddImpulse(dir * LaunchPowerMult, NAME_None, true);

	AAnimal* own = Cast<AAnimal, AActor>(GetOwner());

	if (own != nullptr)
	{
		own->UntouchGround();
	}

	Grabbed = nullptr;
}

void UGrabbableComponent::UnGrab()
{
	if (Grabbed == nullptr)
		return;

	Grabbed->GetComponentByClass<UGrabberComponent>()->UnGrab(this);
}

bool UGrabbableComponent::GetIsGrabbable()
{
	return bIsGrabbable;
}

bool UGrabbableComponent::GetIsTwoSlots()
{
	return bIsTwoSlots;
}

void UGrabbableComponent::SetIsGrabbable(bool bGrabbable)
{
	if (GEngine != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::SanitizeFloat(bGrabbable));
	bIsGrabbable = bGrabbable;
}
