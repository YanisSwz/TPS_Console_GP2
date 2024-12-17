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
	if (GetOwner()->AttachToComponent(parent->GetComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), socketName))
	{
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::MakeRandomColor(), "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	}

	FVector socketPos = parent->GetComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(socketName);


	AAnimal* own = Cast<AAnimal, AActor>(GetOwner());

	Grabbed = parent;

	if (own != nullptr)
	{
		own->SetLastGrabbedBy(Grabbed);

		GetOwner()->SetActorLocation(socketPos, false, nullptr, ETeleportType::TeleportPhysics);

		own->GetMesh()->SetBodySimulatePhysics(own->GetMesh()->GetSocketBoneName(GrabSocket), false);


		// This is here because when an animal is in ragdoll with one bone not simulated (only when it's not the "root" bone like "BEAR_-Pelvis"), it moves weirdly (like break-dancing), but deactivating the AnimClass fixes it.
		AnimClass = own->GetMesh()->GetAnimClass();
		own->GetMesh()->SetAnimClass(nullptr);
		//

		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::MakeRandomColor(), own->GetMesh()->GetSocketBoneName(GrabSocket).ToString());

		own->GetMesh()->SetRelativeRotation((own->GetMesh()->GetSocketRotation(GrabSocket).Quaternion().Inverse() * own->GetMesh()->GetComponentRotation().Quaternion()), false, nullptr, ETeleportType::TeleportPhysics);

		own->GetMesh()->SetWorldLocation(socketPos - (own->GetMesh()->GetSocketLocation(GrabSocket) - own->GetMesh()->GetComponentLocation()), false, nullptr, ETeleportType::TeleportPhysics);
	}

	bIsGrabbable = false;
}

void UGrabbableComponent::Launch(FVector dir)
{
	GetOwner()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//GetOwner()->GetComponentByClass<UCapsuleComponent>()->SetSimulatePhysics(true);

	AAnimal* own = Cast<AAnimal, AActor>(GetOwner());

	if (own != nullptr)
	{
		own->SetUngrabbed();
		own->GetMesh()->SetSimulatePhysics(true);
		own->GetMesh()->AddImpulse(dir * LaunchPowerMult, own->GetMesh()->GetSocketBoneName(GrabSocket), true);

		// check comment at line 56
		own->GetMesh()->SetAnimClass(AnimClass);
		//
	}


	Grabbed = nullptr;

	bIsGrabbable = true;
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
