// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NxEnemyCharacter.h"
#include "NxEnemyCat.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxEnemyCat : public ANxEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANxEnemyCat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
