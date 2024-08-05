// Fill out your copyright notice in the Description page of Project Settings.


#include "Teacup.h"


// Sets default values
ATeacup::ATeacup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);

	
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ATeacup::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* Mouse : MouseFriends)
	{
		Mouse->SetActorHiddenInGame(true);
	}
}

// Called every frame
void ATeacup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeacup::TryAddMouseFriend()
{

	for (AActor* Mouse : MouseFriends)
	{
		if (Mouse->IsHidden())
		{
			Mouse->SetActorHiddenInGame(false);
			return;
		}
	}
}

bool ATeacup::CanAddMouseFriend()
{
	if (MouseFriends.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("MouseFriends is empty"));
		return false;
	}

	for (AActor* Mouse : MouseFriends)
	{
		if (Mouse->IsHidden())
		{
			return true;
		}
	}

	return false;
}

void ATeacup::HideAllMouseFriends()
{
	for (AActor* Mouse : MouseFriends)
	{
		Mouse->SetActorHiddenInGame(true);
	}
}

void ATeacup::StartSpin()
{
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 30.0f, 0.0f);
}

void ATeacup::StopSpin()
{
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}
