// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCat.h"

#include "NxEnemyCatAIController.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANxEnemyCat::ANxEnemyCat()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeftFistCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFistCollisionComponent"));
	LeftFistCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                              TEXT("DEF-f_pinky_002_L"));
	LeftFistCollisionComponent->SetSphereRadius(FistCollisionRadius);
}

// Called when the game starts or when spawned
void ANxEnemyCat::BeginPlay()
{
	Super::BeginPlay();

	LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFistCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANxEnemyCat::OnOverlapBegin);

	CatAIController = Cast<ANxEnemyCatAIController>(GetController());
}

bool ANxEnemyCat::TryFistAttack() const
{
	if (!IsAttacking())
	{
		AnimInstance->Montage_Play(FistAttackMontage);
		return true;
	}
	return false;
}

void ANxEnemyCat::PlayFoundPlayerSound()
{
	if (!IsValid(FoundPlayerSound))
	{
		UE_LOG(LogTemp, Error, TEXT("ANxEnemyCat::PlayFoundPlayerSound: FoundPlayerSound is not valid"));
		return;
	}
	if (IsValid(FoundPlayerSoundAudioComponent))
	{
		if (FoundPlayerSoundAudioComponent->IsPlaying())
		{
			return;
		}
	}

	FoundPlayerSoundAudioComponent = UGameplayStatics::SpawnSoundAtLocation(
		GetWorld(), FoundPlayerSound, GetActorLocation());
}

void ANxEnemyCat::PlayStartSleepingSound() const
{
	if (!IsValid(StartSleepingSound))
	{
		UE_LOG(LogTemp, Error, TEXT("ANxEnemyCat::PlayStartSleepingSound: StartSleepingSound is not valid"));
		return;
	}

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), StartSleepingSound, GetActorLocation());
}

void ANxEnemyCat::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != this && !HittingActorList.Contains(OtherActor))
	{
		HittingActorList.Add(OtherActor);

		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (IsValid(Player))
		{
			if (!IsValid(CatAIController))
			{
				UE_LOG(LogTemp, Error, TEXT("ANxEnemyCat::OnOverlapBegin: CatAIController is not valid"));
				return;
			}
			UE_LOG(LogTemp, Display, TEXT("ANxEnemyCat::OnOverlapBegin: Hit player"));
			CatAIController->OnHitPlayer();
		}
	}
}

void ANxEnemyCat::BeginFistAttack()
{
	HittingActorList.Empty();
	LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ANxEnemyCat::EndFistAttack()
{
	LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ANxEnemyCat::TrySleep()
{
	if (IsAttacking())
	{
		return false;
	}
	IsSleeping = true;
	PlayStartSleepingSound();
	return true;
}

bool ANxEnemyCat::TryWakeUp()
{
	if (IsSleeping)
	{
		IsSleeping = false;
		CatAIController->OnWakeUp();
		return true;
	}
	return false;
}

bool ANxEnemyCat::IsAttacking() const
{
	return AnimInstance->Montage_IsPlaying(FistAttackMontage);
}

bool ANxEnemyCat::GetIsSleeping() const
{
	return IsSleeping;
}

// Called every frame
void ANxEnemyCat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
