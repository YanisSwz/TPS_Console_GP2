// Fill out your copyright notice in the Description page of Project Settings.


#include "CashOutZone.h"
#include "animal.h"

// Sets default values
ACashOutZone::ACashOutZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
	//DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ACashOutZone::OnOverlap);
}

// Called when the game starts or when spawned
void ACashOutZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACashOutZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACashOutZone::OnOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AAnimal* animal = Cast<AAnimal>(OtherActor);
	if (animal != nullptr)
	{
		if (animal->bIsSleeping) 
		{
			//mettre les points
			
			/*if (animal->lastGrabbed == player1) 
			{
				bluePlayerPoints += animal->points;
			}
			else if (animal->lastGrabbed == player2)
			{
				redPlayerPoints += animal->points;
			}*/

			//desactiver l'animal

			animal->bIsActive = false;
			//voir si je peux pa juste supp le script
		}
	}
}

