// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTTask_WakeUp.h"

#include "AIController.h"
#include "NxEnemyCat.h"

EBTNodeResult::Type UNxBTTask_WakeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!IsValid(OwnerComp.GetAIOwner()))
	{
		return EBTNodeResult::Failed;
	}
	ANxEnemyCat* EnemyCat = Cast<ANxEnemyCat>(OwnerComp.GetAIOwner()->GetPawn());

	if (!IsValid(EnemyCat))
	{
		return EBTNodeResult::Failed;
	}
	if (EnemyCat->TryWakeUp())
	{
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
