// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Survive.h"

UBTTask_Survive::UBTTask_Survive()
{
    NodeName = "Survive";
}

EBTNodeResult::Type UBTTask_Survive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AAnimalController* Controller = Cast<AAnimalController>(OwnerComp.GetAIOwner());
    if(!Controller)
        return EBTNodeResult::Failed;

    Controller->Survive();

    return EBTNodeResult::Succeeded;
}
