// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NxEnemyAIController.h"
#include "NxEnemyCatAIController.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxEnemyCatAIController : public ANxEnemyAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANxEnemyCatAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
