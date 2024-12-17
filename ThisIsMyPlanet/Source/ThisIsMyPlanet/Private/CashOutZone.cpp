// Fill out your copyright notice in the Description page of Project Settings.


#include "CashOutZone.h"
#include "animal.h"


// Sets default values
ACashOutZone::ACashOutZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ACashOutZone::OnOverlap);

	player1 = nullptr;
	player2 = nullptr;
}

// Called when the game starts or when spawned
void ACashOutZone::BeginPlay()
{
	Super::BeginPlay();
	
	player1 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	player2 = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1));

	ChangeBlueScore();
	ChangeRedScore();
}

// Called every frame
//void ACashOutZone::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ACashOutZone::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAnimal* animal = Cast<AAnimal>(OtherActor);
	if (animal != nullptr)
	{
		if (animal->bIsSleeping && animal->bIsActive) 
		{
			//add points
			if (animal->LastGrabbedBy == player1)
			{
				bluePlayerPoints += animal->PointValue;
				ChangeBlueScore();

				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "blue scored");
			}
			else if (animal->LastGrabbedBy == player2)
			{
				redPlayerPoints += animal->PointValue;
				ChangeRedScore();

				if (GEngine != nullptr)
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "red scored");
			}


			if (GEngine != nullptr) 
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::SanitizeFloat(redPlayerPoints));
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::SanitizeFloat(bluePlayerPoints));
			}

			//disable animal
			animal->bIsActive = false;
			animal->GetCapsuleComponent()->SetSimulatePhysics(true);
			animal->GetMesh()->SetSimulatePhysics(true);
			animal->GetCharacterMovement()->SetMovementMode(MOVE_None);
			animal->bIsSleeping = true;

			//make animal fly
			FVector inpulse = FVector(0.0f, 0.f, 10000000.f);
			animal->GetMesh()->AddImpulse(inpulse);

			//destroy the animal
				animal->Sell();
		}
	}
}

