// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NxAIControllerBase.h"
#include "NxEnemyAIPerception.h"
#include "NxEnemyAIController.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxEnemyAIController : public ANxAIControllerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANxEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void ShowSightRadius() const;

	UNxEnemyAIPerception* GetAIEnemyPerception() const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetToWalkSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetToRunSpeed() const;

	float GetWalkingSpeed() const;
	float GetRunningSpeed() const;

	void SetSightParameters(float NewSightRadius, float NewLoseSightRadius, float NewSightAngle);
	void SetLoseSightRadius(float NewLoseSightRadius);

	

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	

public:
	UPROPERTY(EditAnywhere, Category = "AI")
	bool IsDebugShowSightRadius = false;
	
protected:
	UPROPERTY()
	UNxEnemyAIPerception* EnemyAIPerception = nullptr;

	UPROPERTY(BlueprintReadOnly)
	class ANxEnemyCharacter* EnemyCharacter = nullptr;

	UPROPERTY(EditAnywhere, Category = "AI")
	float WalkSpeed = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float RunSpeed = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float SightRadius = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseSightRadius = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float SightAngle = 0.0f;

};
