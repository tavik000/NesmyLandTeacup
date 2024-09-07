// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "NxBTDecorator_IsWalking.generated.h"

/**
 * 
 */
UCLASS()
class MOUSELAND_TEACUP_API UNxBTDecorator_IsWalking : public UBTDecorator
{
	GENERATED_BODY()
public:
	UNxBTDecorator_IsWalking();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
