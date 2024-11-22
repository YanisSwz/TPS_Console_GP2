// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberComponent.h"
#include "GrabbableComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	owner = GetOwner();
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGrabberComponent::Grab(bool isRightHand)
{
	FVector start = owner->GetActorLocation();
	FVector forward = owner->GetActorForwardVector();

	TArray<AActor*> Ignore;
	TArray<FHitResult> Hits;

	if (UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), start, start, HalfSize, owner->GetActorRotation(), Actors, false, Ignore, EDrawDebugTrace::ForDuration, Hits, true))
	{
		for (size_t i = 0; i < Hits.Num(); i++)
		{
			UGrabbableComponent* grabbable = Cast<UGrabbableComponent, AActor>(Hits[i].GetActor());
			if (grabbable != nullptr)
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, "GG");

				if (isRightHand)
					grabbable->AttachTo(owner, rightHandSocketName);
				else
					grabbable->AttachTo(owner, leftHandSocketName);

				return true;
			}
		}
	}

	return false;
}

