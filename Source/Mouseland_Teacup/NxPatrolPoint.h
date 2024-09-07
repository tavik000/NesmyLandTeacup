// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NxPatrolPoint.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxPatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	ANxPatrolPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditInstanceOnly, Category = "AI")
	ANxPatrolPoint* NextPatrolPoint;
};
