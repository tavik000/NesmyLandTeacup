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
	
	
	UFUNCTION(BlueprintCallable)
	bool TryFistAttack() const;

	void PlayFoundPlayerSound();
	void PlayStartSleepingSound() const;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnFistOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:
	UPROPERTY(EditAnywhere)
	USoundBase* FoundPlayerSound;

	UPROPERTY(EditAnywhere)
	USoundBase* StartSleepingSound;
	

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
	
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class ANxEnemyCatAIController* CatAIController;

	UPROPERTY()
	UAudioComponent* FoundPlayerSoundAudioComponent;
};
