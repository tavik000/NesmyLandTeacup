// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCatAIController.h"

#include "NxEnemyCat.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"


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
		
		const AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
		if (Stimulus.WasSuccessfullySensed() && !EnemyCat->GetIsSleeping())
		{
			if (!IsValid(TargetActor))
			{
				UE_LOG(LogTemp, Display, TEXT("NxEnemyCatAIController::OnTargetPerceptionUpdated: Found player"));
				Blackboard->SetValueAsObject("TargetActor", Actor);
				EnemyCat->PlayFoundPlayerSound();
			}
		}
		else
		{
			if (IsValid(TargetActor))
			{
				UE_LOG(LogTemp, Display,
				       TEXT(
					       "NxEnemyCatAIController::OnTargetPerceptionUpdated: Lost player, WasSuccessfullySensed: %d, IsSleeping: %d"
				       ), Stimulus.WasSuccessfullySensed(), EnemyCat->GetIsSleeping());
				Blackboard->SetValueAsObject("TargetActor", nullptr);
			}
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

void ANxEnemyCatAIController::OnWakeUp()
{
	TArray<AActor*> PerceivedActors;
	EnemyAIPerception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	for (AActor* PerceivedActor : PerceivedActors)
	{
		if (const APlayerCharacter* Player = Cast<APlayerCharacter>(PerceivedActor); IsValid(Player))
		{
			UE_LOG(LogTemp, Display, TEXT("NxEnemyCatAIController::OnWakeUp: Found player"));
			Blackboard->SetValueAsObject("TargetActor", PerceivedActor);
			EnemyCat->PlayFoundPlayerSound();
			break;
		}
	}
}
