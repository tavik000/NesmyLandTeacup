// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCatAIController.h"

#include "NxEnemyCat.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


ANxEnemyCatAIController::ANxEnemyCatAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ANxEnemyCatAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ANxEnemyCatAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANxEnemyCatAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	Super::OnTargetPerceptionUpdated(Actor, Stimulus);

	if (!Stimulus.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Stimulus is not valid, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(Actor); IsValid(Player))
	{
		if (!IsValid(EnemyCat))
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyCat is null, Function name: %s"), *FString(__FUNCTION__));
			return;
		}

		if (Stimulus.WasSuccessfullySensed() && !EnemyCat->GetIsSleeping())
		{
			UE_LOG(LogTemp, Display, TEXT("NxEnemyCatAIController::OnTargetPerceptionUpdated: Found player"));
			Blackboard->SetValueAsObject("TargetActor", Actor);
			EnemyCat->PlayFoundPlayerSound();
		}
		else
		{
			UE_LOG(LogTemp, Display,
			       TEXT(
				       "NxEnemyCatAIController::OnTargetPerceptionUpdated: Lost player, WasSuccessfullySensed: %d, IsSleeping: %d"
			       ), Stimulus.WasSuccessfullySensed(), EnemyCat->GetIsSleeping());
			Blackboard->SetValueAsObject("TargetActor", nullptr);
		}
	}
}

void ANxEnemyCatAIController::OnHitPlayer() const
{
	Blackboard->SetValueAsBool("IsHitPlayer", true);
	UE_LOG(LogTemp, Display, TEXT("NxEnemyCatAIController::OnHitPlayer: Lost player"));
	Blackboard->SetValueAsObject("TargetActor", nullptr);
}

void ANxEnemyCatAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(PossessCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("PossessCharacter is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	EnemyCat = Cast<ANxEnemyCat>(PossessCharacter);
}
