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

		CurrentBlackboard->SetValueAsBool("IsPatrolling", false);

		if (const AActor* TargetActor = Cast<AActor>(CurrentBlackboard->GetValueAsObject("TargetActor"));
			IsValid(TargetActor))
		{
			UE_LOG(LogTemp, Display, TEXT("UNxBTTask_Sleep::ExecuteTask: Sleeping, Lost Player"));
			CurrentBlackboard->SetValueAsObject("TargetActor", nullptr);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
