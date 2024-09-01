// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NxAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class MOUSELAND_TEACUP_API ANxAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;
	
	UPROPERTY(BlueprintReadOnly)
	ACharacter* PossessCharacter = nullptr;
};
