// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTTask_Attack.h"

#include "AIController.h"
#include "NxEnemyCat.h"
#include "NxEnemyCatAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UNxBTTask_Attack::UNxBTTask_Attack()
{
	NodeName = "Attack";
}

EBTNodeResult::Type UNxBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ANxEnemyCat* EnemyCat = Cast<ANxEnemyCat>(OwnerComp.GetAIOwner()->GetPawn());
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


	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(CurrentBlackboard))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UNxBTService_SetNextPatrolPoint::OnBecomeRelevant: CurrentBlackboard is not valid"));
		return EBTNodeResult::Failed;
	}

	const AActor* TargetActor = Cast<AActor>(CurrentBlackboard->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Failed;
	}
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		EnemyCat->GetActorLocation(), TargetActor->GetActorLocation());
	EnemyCat->SetActorRotation(FRotator(EnemyCat->GetActorRotation().Pitch, LookAtRotation.Yaw,
	                                    EnemyCat->GetActorRotation().Roll));

	if (EnemyCat->TryFistAttack())
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
