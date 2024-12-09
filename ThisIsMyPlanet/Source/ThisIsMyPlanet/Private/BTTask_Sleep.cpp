// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Sleep.h"

UBTTask_Sleep::UBTTask_Sleep()
{
	NodeName = "Sleep";
}

EBTNodeResult::Type UBTTask_Sleep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AAnimalController* Controller = Cast<AAnimalController>(OwnerComp.GetAIOwner());
    if (!Controller)
        return EBTNodeResult::Failed;

    Controller->Sleep();

    return EBTNodeResult::Succeeded; return EBTNodeResult::Type();
}
