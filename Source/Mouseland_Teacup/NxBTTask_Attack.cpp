// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTTask_Attack.h"

#include "AIController.h"
#include "NxEnemyCat.h"
#include "NxEnemyCatAIController.h"

UNxBTTask_Attack::UNxBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UNxBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const ANxEnemyCat* EnemyCat = Cast<ANxEnemyCat>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(EnemyCat))
	{
		UE_LOG(LogTemp, Error, TEXT("UNxBTTask_Attack::ExecuteTask: Enemy is not valid"));
		return EBTNodeResult::Failed;
	}
	if (EnemyCat->IsAttacking())
	{
		return EBTNodeResult::Succeeded;
	}

	ANxEnemyCatAIController* CatAIController = Cast<ANxEnemyCatAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(CatAIController))
	{
		UE_LOG(LogTemp, Error, TEXT("UNxBTTask_Attack::ExecuteTask: CatAIController is not valid"));
		return EBTNodeResult::Failed;
	}
	CatAIController->StopMovement();

	if (EnemyCat->TryFistAttack())
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
