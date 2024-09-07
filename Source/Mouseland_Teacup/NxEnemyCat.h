// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NxEnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "NxEnemyCat.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ANxEnemyCat : public ANxEnemyCharacter
{
	GENERATED_BODY()

public:
	ANxEnemyCat();

	virtual void Tick(float DeltaTime) override;
	
	void BeginFistAttack();
	void EndFistAttack();

	bool TrySleep();
	bool TryWakeUp();
	

	bool IsAttacking() const;
	
	UFUNCTION(BlueprintCallable)
	bool GetIsSleeping() const;
	
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void TryFistAttack();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	
public:


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* FistAttackMontage;

private:
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USphereComponent* LeftFistCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float FistCollisionRadius = 32.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	bool IsSleeping = false;
	
};
