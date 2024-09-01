// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCatAIController.h"


// Sets default values
ANxEnemyCatAIController::ANxEnemyCatAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANxEnemyCatAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANxEnemyCatAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

