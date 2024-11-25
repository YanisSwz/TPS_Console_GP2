// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AnimalController.h"
#include "BTTask_Flee.generated.h"

/**
 * 
 */
UCLASS()
class THISISMYPLANET_API UBTTask_Flee : public UBTTaskNode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector Key;

public:
	UBTTask_Flee();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
