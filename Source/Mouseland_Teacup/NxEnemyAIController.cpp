// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyAIController.h"

#include "NxEnemyCat.h"
#include "NxEnemyCharacter.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


ANxEnemyAIController::ANxEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANxEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!IsValid(PossessCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("PossessCharacter is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	EnemyCharacter = Cast<ANxEnemyCharacter>(PossessCharacter);
}

void ANxEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIPerception = Cast<UNxEnemyAIPerception>(FindComponentByClass<UNxEnemyAIPerception>());
	if (IsValid(EnemyAIPerception))
	{
		EnemyAIPerception->Init();
		EnemyAIPerception->SetSightRadius(SightRadius);
		EnemyAIPerception->SetLoseSightRadius(LoseSightRadius);
		EnemyAIPerception->SetPeripheralVisionDegree(SightAngle);
		EnemyAIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ANxEnemyAIController::OnTargetPerceptionUpdated);
		
	}
}

void ANxEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}

void ANxEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDebugShowSightRadius)
	{
		ShowSightRadius();
	}
}

void ANxEnemyAIController::ShowSightRadius() const
{
	if (!FApp::IsGame())
	{
		return;
	}
	if (!IsValid(EnemyCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacter is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	const FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	const FVector EnemyForwardVector = EnemyCharacter->GetActorForwardVector();
	if (!IsValid(EnemyAIPerception))
	{
		UE_LOG(LogTemp, Error, TEXT("AIEnemyPerception is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	const float LocalSightRadius = EnemyAIPerception->GetSightRadius();
	const float LocalPeripheralVisionDegree = EnemyAIPerception->GetPeripheralVisionDegree();
	DrawDebugCone(GetWorld(), EnemyLocation, EnemyForwardVector, LocalSightRadius, LocalPeripheralVisionDegree,
	              0.0f, 36, FColor::Cyan, false, 0.0f, 0, 0.0f);
}

UNxEnemyAIPerception* ANxEnemyAIController::GetAIEnemyPerception() const
{
	if (!IsValid(EnemyAIPerception))
	{
		return nullptr;
	}
	return EnemyAIPerception;
}

void ANxEnemyAIController::SetToWalkSpeed() const
{
	PossessCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ANxEnemyAIController::SetToRunSpeed() const
{
	PossessCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

float ANxEnemyAIController::GetWalkingSpeed() const
{
	return WalkSpeed;
}

float ANxEnemyAIController::GetRunningSpeed() const
{
	return RunSpeed;
}

void ANxEnemyAIController::SetSightParameters(float NewSightRadius, float NewLoseSightRadius, float NewSightAngle)
{
	SightRadius = NewSightRadius;
	LoseSightRadius = NewLoseSightRadius;
	SightAngle = NewSightAngle;
	if (IsValid(EnemyAIPerception))
	{
		EnemyAIPerception->SetSightRadius(NewSightRadius);
		EnemyAIPerception->SetLoseSightRadius(NewLoseSightRadius);
		EnemyAIPerception->SetPeripheralVisionDegree(NewSightAngle);
	}
}

void ANxEnemyAIController::SetLoseSightRadius(float NewLoseSightRadius)
{
	LoseSightRadius = NewLoseSightRadius;
	if (IsValid(EnemyAIPerception))
	{
		EnemyAIPerception->SetLoseSightRadius(NewLoseSightRadius);
	}
}
