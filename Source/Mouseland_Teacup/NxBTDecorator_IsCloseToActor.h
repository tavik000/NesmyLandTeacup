// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "NxBTDecorator_IsCloseToActor.generated.h"

/**
 * 
 */
UCLASS()
class MOUSELAND_TEACUP_API UNxBTDecorator_IsCloseToActor : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
	
public:
	UNxBTDecorator_IsCloseToActor();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 150.f;
	
};
