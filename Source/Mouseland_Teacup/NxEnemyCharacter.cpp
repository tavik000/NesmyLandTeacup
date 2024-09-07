// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ANxEnemyCharacter::ANxEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANxEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetCharacterMovement());
}

// Called every frame
void ANxEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ANxEnemyCharacter::GetSpeed() const
{
	return CharacterMovementComponent->Velocity.Size();
}

