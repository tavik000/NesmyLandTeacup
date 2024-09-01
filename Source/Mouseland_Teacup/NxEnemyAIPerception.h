// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "NxEnemyAIPerception.generated.h"


UCLASS(ClassGroup = AI, HideCategories = (Activation, Collision), meta = (BlueprintSpawnableComponent), config = Game)
class MOUSELAND_TEACUP_API UNxEnemyAIPerception : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNxEnemyAIPerception();

	void Init();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSightRadius();

	UFUNCTION(BlueprintCallable)
	void SetSightRadius(float InRadius);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPeripheralVisionDegree();

	UFUNCTION(BlueprintCallable)
	void SetPeripheralVisionDegree(float InHalfDegree);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetLoseSightRadius();

	UFUNCTION(BlueprintCallable)
	void SetLoseSightRadius(float InRadius);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

protected:
	FAISenseID SightSenseID;
};
