// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyCat.h"

#include "PlayerCharacter.h"


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
}

void ANxEnemyCat::TryFistAttack()
{
	AnimInstance->Montage_Play(FistAttackMontage);
}

void ANxEnemyCat::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != this && !HittingActorList.Contains(OtherActor))
	{
		HittingActorList.Add(OtherActor);

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		if (IsValid(PlayerCharacter))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit Player"));
		}
	}
}

void ANxEnemyCat::BeginFistAttack()
{
	HittingActorList.Empty();
	LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogTemp, Warning, TEXT("BeginFistAttack"));
}

void ANxEnemyCat::EndFistAttack()
{
	LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Warning, TEXT("EndFistAttack"));
}

bool ANxEnemyCat::TrySleep()
{
	if (IsAttacking())
	{
		return false;
	}
	IsSleeping = true;
	return true;
}

bool ANxEnemyCat::TryWakeUp()
{
	if (IsSleeping)
	{
		IsSleeping = false;
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
