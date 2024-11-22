// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Flee.h"

UBTTask_Flee::UBTTask_Flee()
{
	NodeName = "Flee";
}

EBTNodeResult::Type UBTTask_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    AAnimalController* Controller = Cast<AAnimalController>(OwnerComp.GetAIOwner());
    Controller->Flee();

    return EBTNodeResult::Succeeded;
}
