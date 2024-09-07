// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NxEnemyCharacter.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANxEnemyCharacter();
	virtual void Tick(float DeltaTime) override;

	float GetSpeed() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

protected:
	TArray<AActor*> HittingActorList;
	UAnimInstance* AnimInstance;
	
private:
	UCharacterMovementComponent* CharacterMovementComponent;
};
