// Fill out your copyright notice in the Description page of Project Settings.


#include "NxBTDecorator_IsWalking.h"

#include "NxEnemyAIController.h"
#include "NxEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UNxBTDecorator_IsWalking::UNxBTDecorator_IsWalking()
{
	NodeName = "Is Walking?";
	bCreateNodeInstance = true;
	bNotifyTick = false;
}

bool UNxBTDecorator_IsWalking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ANxEnemyAIController* EnemyAIController = Cast<ANxEnemyAIController>(OwnerComp.GetAIOwner());
	if (!IsValid(EnemyAIController))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UNxBTDecorator_IsWalking::CalculateRawConditionValue: EnemyAIController is not valid"));
		return true;
	}

	const ANxEnemyCharacter* Enemy = Cast<ANxEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(Enemy))
	{
		UE_LOG(LogTemp, Error, TEXT("UNxBTDecorator_IsWalking::CalculateRawConditionValue: Enemy is not valid"));
		return true;
	}
	UCharacterMovementComponent* EnemyMovementComponent = Enemy->GetCharacterMovement();
	if (!IsValid(EnemyMovementComponent))
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UNxBTDecorator_IsWalking::CalculateRawConditionValue: EnemyMovementComponent is not valid"));
		return true;
	}
	return EnemyMovementComponent->GetMaxSpeed() <= EnemyAIController->GetWalkingSpeed();
}
