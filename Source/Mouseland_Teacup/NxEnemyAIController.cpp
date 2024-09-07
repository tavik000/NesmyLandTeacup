﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyAIController.h"

#include "NxEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


ANxEnemyAIController::ANxEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	AIEnemyPerception = CreateDefaultSubobject<UNxEnemyAIPerception>(TEXT("EnemyAIPerception"));
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
	if (IsValid(AIEnemyPerception))
	{
		AIEnemyPerception->Init();
	}
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
	if (!IsValid(EnemyCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacter is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	const FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	const FVector EnemyForwardVector = EnemyCharacter->GetActorForwardVector();
	if (!IsValid(AIEnemyPerception))
	{
		UE_LOG(LogTemp, Error, TEXT("AIEnemyPerception is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	const float LocalSightRadius = AIEnemyPerception->GetSightRadius();
	const float LocalPeripheralVisionDegree = AIEnemyPerception->GetPeripheralVisionDegree();
	DrawDebugCone(GetWorld(), EnemyLocation, EnemyForwardVector, LocalSightRadius, LocalPeripheralVisionDegree,
	              0.0f, 36, FColor::Cyan, false, 0.0f, 0, 0.0f);
}

UNxEnemyAIPerception* ANxEnemyAIController::GetAIEnemyPerception() const
{
	if (!IsValid(AIEnemyPerception))
	{
		return nullptr;
	}
	return AIEnemyPerception;
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
	if (IsValid(AIEnemyPerception))
	{
		AIEnemyPerception->SetSightRadius(NewSightRadius);
		AIEnemyPerception->SetLoseSightRadius(NewLoseSightRadius);
		AIEnemyPerception->SetPeripheralVisionDegree(NewSightAngle);
	}
}

void ANxEnemyAIController::SetLoseSightRadius(float NewLoseSightRadius)
{
	LoseSightRadius = NewLoseSightRadius;
	if (IsValid(AIEnemyPerception))
	{
		AIEnemyPerception->SetLoseSightRadius(NewLoseSightRadius);
	}
}
