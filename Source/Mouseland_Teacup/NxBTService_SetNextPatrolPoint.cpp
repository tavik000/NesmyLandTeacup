// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTService_SetNextPatrolPoint.h"

#include "NxPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UNxBTService_SetNextPatrolPoint::UNxBTService_SetNextPatrolPoint()
{
	NodeName = "Set Next Patrol Point";
	bNotifyBecomeRelevant = true;
	bNotifyTick = false;
}

void UNxBTService_SetNextPatrolPoint::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* CurrentBlackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(CurrentBlackboard))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UNxBTService_SetNextPatrolPoint::OnBecomeRelevant: CurrentBlackboard is not valid"));
		return;
	}

	const ANxPatrolPoint* PatrolPointActor = Cast<ANxPatrolPoint>(
		CurrentBlackboard->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(PatrolPointActor))
	{
		TArray<AActor*> AllPatrolPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANxPatrolPoint::StaticClass(), AllPatrolPoints);
		if (AllPatrolPoints.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("UNxBTService_SetNextPatrolPoint::OnBecomeRelevant: PatrolPoints is empty"));
			return;
		}
		AActor* RandomPatrolPoint = AllPatrolPoints[FMath::RandRange(0, AllPatrolPoints.Num() - 1)];
		CurrentBlackboard->SetValueAsObject(GetSelectedBlackboardKey(), RandomPatrolPoint);
	}
	else
	{
		ANxPatrolPoint* NextPatrolPoint = PatrolPointActor->NextPatrolPoint;
		if (!IsValid(NextPatrolPoint))
		{
			UE_LOG(LogTemp, Error,
			       TEXT("UNxBTService_SetNextPatrolPoint::OnBecomeRelevant: NextPatrolPoint is not valid"));
			return;
		}
		CurrentBlackboard->SetValueAsObject(GetSelectedBlackboardKey(), NextPatrolPoint);
	}
	CurrentBlackboard->SetValueAsBool("IsPatrolling", true);
}
