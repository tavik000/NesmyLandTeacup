// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "NxBTDecorator_Random.generated.h"

/**
 * 
 */
UCLASS()
class MOUSELAND_TEACUP_API UNxBTDecorator_Random : public UBTDecorator
{
	GENERATED_BODY()
	
	
public:
	explicit UNxBTDecorator_Random(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostLoad() override;
	virtual void SetOwner(AActor* ActorOwner) override;
	virtual void OnInstanceCreated(UBehaviorTreeComponent& OwnerComp) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
	
	// Trueを返す確率（0-100）
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Percent", ClampMin = "0.0", ClampMax = "100.0"), Category = "AI")
	float Percent;

};
