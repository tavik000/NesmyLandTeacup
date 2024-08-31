// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCat.h"


// Sets default values
ANxEnemyCat::ANxEnemyCat()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANxEnemyCat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANxEnemyCat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


