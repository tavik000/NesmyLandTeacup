// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTDecorator_IsCloseToActor.h"

#include "AIController.h"
#include "NxEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UNxBTDecorator_IsCloseToActor::UNxBTDecorator_IsCloseToActor()
{
	NodeName = "Is Close To Target Actor?";
	bCreateNodeInstance = true;
	bNotifyTick = false;
}

bool UNxBTDecorator_IsCloseToActor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	const ANxEnemyCharacter* Enemy = Cast<ANxEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Enemy))
	{
		UE_LOG(LogTemp, Error, TEXT("UNxBTDecorator_IsWalking::CalculateRawConditionValue: Enemy is not valid"));
		return false;
	}


	const UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(CurrentBlackboard))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UNxBTService_SetNextPatrolPoint::OnBecomeRelevant: CurrentBlackboard is not valid"));
		return false;
	}

	const AActor* TargetActor = Cast<AActor>(CurrentBlackboard->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(TargetActor))
	{
		return false;
	}

	return Enemy->GetDistanceTo(TargetActor) <= AcceptanceRadius;
	
}
