// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "NxBTService_SetNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class MOUSELAND_TEACUP_API UNxBTService_SetNextPatrolPoint : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UNxBTService_SetNextPatrolPoint();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
