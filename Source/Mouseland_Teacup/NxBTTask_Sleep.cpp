// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTTask_Sleep.h"

#include "AIController.h"
#include "NxEnemyCat.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UNxBTTask_Sleep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if (EnemyCat->TrySleep())
	{
		UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
		if (!IsValid(CurrentBlackboard))
		{
			UE_LOG(LogTemp, Error,
			       TEXT("UNxBTService_SetNextPatrolPoint::OnBecomeRelevant: CurrentBlackboard is not valid"));
			return EBTNodeResult::Failed;
		}
		
		UE_LOG(LogTemp, Display, TEXT("UNxBTTask_Sleep::ExecuteTask: Sleeping, Lost Player"));
		CurrentBlackboard->SetValueAsBool("IsPatrolling", false);
		CurrentBlackboard->SetValueAsObject("TargetActor", nullptr);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
