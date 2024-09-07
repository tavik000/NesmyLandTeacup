// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTDecorator_Random.h"

UNxBTDecorator_Random::UNxBTDecorator_Random(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Random?");
	bCreateNodeInstance = true;
	bNotifyTick = false;
}

void UNxBTDecorator_Random::PostLoad()
{
	Super::PostLoad();
}

void UNxBTDecorator_Random::SetOwner(AActor* ActorOwner)
{
	Super::SetOwner(ActorOwner);
}

void UNxBTDecorator_Random::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
	Super::OnInstanceCreated(OwnerComp);
}

bool UNxBTDecorator_Random::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (Percent > 0.0 && FMath::FRandRange(0.0, 100.0) <= Percent)
	{
		return true;
	}

	return false;
}

void UNxBTDecorator_Random::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
