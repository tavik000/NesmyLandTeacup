// Fill out your copyright notice in the Description page of Project Settings.


#include "NxAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ANxAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessCharacter = Cast<ACharacter>(InPawn);
}

void ANxAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		// const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		// GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void ANxAIControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
