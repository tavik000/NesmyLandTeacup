// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NxEnemyAIController.h"
#include "NxEnemyCat.h"
#include "NxEnemyCatAIController.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxEnemyCatAIController : public ANxEnemyAIController
{
	GENERATED_BODY()

public:
	ANxEnemyCatAIController();

	virtual void Tick(float DeltaTime) override;
	
	void OnHitPlayer() const;
	virtual void OnPossess(APawn* InPawn) override;

	void OnWakeUp();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) override;

public:
protected:
	
	UPROPERTY(BlueprintReadOnly)
	ANxEnemyCat* EnemyCat = nullptr;
private:

};
