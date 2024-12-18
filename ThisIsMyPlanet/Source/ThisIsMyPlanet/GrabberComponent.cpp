// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	owner = GetOwner();
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGrabberComponent::Grab(bool bIsRightHand)
{
	FVector start = owner->GetActorLocation();
	FVector forward = owner->GetActorForwardVector() * HalfSize.X;

	TArray<AActor*> Ignore;
	TArray<FHitResult> Hits;

	if (UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), start + forward, start + forward, HalfSize, owner->GetActorRotation(), Actors, false, Ignore, EDrawDebugTrace::None, Hits, true))
	{
		for (size_t i = 0; i < Hits.Num(); i++)
		{
			UGrabbableComponent* grabbable = Hits[i].GetActor()->GetComponentByClass<UGrabbableComponent>();
			if (grabbable != nullptr && grabbable->GetIsGrabbable())
			{
				if (bIsRightHand)
				{
					grabbable->AttachTo(owner, rightHandSocketName);
					rightGrabbedComp = grabbable;
					if (grabbable->GetIsTwoSlots())
						leftGrabbedComp = grabbable;
				}
				else
				{
					grabbable->AttachTo(owner, leftHandSocketName);
					leftGrabbedComp = grabbable;
					if (grabbable->GetIsTwoSlots())
						rightGrabbedComp = grabbable;
				}

				return;
			}
		}
	}
}

void UGrabberComponent::SwitchHand(bool bIsRightHand)
{
	if (leftGrabbedComp == nullptr || !leftGrabbedComp->GetIsTwoSlots())
		return;

	UGrabbableComponent* grabbedComp = leftGrabbedComp;

	Launch(false, 0);

	if (bIsRightHand)
		grabbedComp->AttachTo(owner, rightHandSocketName);
	else
		grabbedComp->AttachTo(owner, leftHandSocketName);

	leftGrabbedComp = grabbedComp;

	rightGrabbedComp = grabbedComp;
}

void UGrabberComponent::Launch(bool bIsRightHand, float BaseLaunchPower)
{
	if (bIsRightHand)
	{
		if (rightGrabbedComp == nullptr)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "nullptr is not launchable!");
			return;
		}

		FVector launch = owner->GetComponentByClass<UCameraComponent>()->GetForwardVector() * BaseLaunchPower + owner->GetVelocity();

		rightGrabbedComp->Launch(launch);

		if (rightGrabbedComp->GetIsTwoSlots())
			leftGrabbedComp = nullptr;

		rightGrabbedComp = nullptr;
	}
	else
	{
		if (leftGrabbedComp == nullptr)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "nullptr is not launchable!");
			return;
		}

		FVector launch = owner->GetComponentByClass<UCameraComponent>()->GetForwardVector() * BaseLaunchPower + owner->GetVelocity();

		leftGrabbedComp->Launch(launch);

		if (leftGrabbedComp->GetIsTwoSlots())
			rightGrabbedComp = nullptr;

		leftGrabbedComp = nullptr;
	}
}

void UGrabberComponent::UnGrab(UGrabbableComponent* Grabbed)
{
	if (Grabbed == leftGrabbedComp)
	{
		Launch(false, 0);
		OnUnGrab.ExecuteIfBound(false);
	}
	else if (Grabbed == rightGrabbedComp)
	{
		Launch(true, 0);
		OnUnGrab.ExecuteIfBound(true);
	}
}

bool UGrabberComponent::GetIsGrabbed(bool bIsRightHand)
{
	if (bIsRightHand)
		return rightGrabbedComp != nullptr;
	else
		return leftGrabbedComp != nullptr;
}
