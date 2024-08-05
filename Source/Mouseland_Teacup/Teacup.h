// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Teacup.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ATeacup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeacup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void TryAddMouseFriend();

	bool CanAddMouseFriend();
	void HideAllMouseFriends();

	void StartSpin();
	void StopSpin();

	UPROPERTY(editAnywhere)
	TArray<AActor*> MouseFriends;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComponent;
};
