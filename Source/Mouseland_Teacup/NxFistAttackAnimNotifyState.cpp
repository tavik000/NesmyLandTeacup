// Fill out your copyright notice in the Description page of Project Settings.


#include "NxFistAttackAnimNotifyState.h"

#include "NxEnemyCat.h"

void UNxFistAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ANxEnemyCat* EnemyCat = Cast<ANxEnemyCat>(MeshComp->GetOwner());
	if (!IsValid(EnemyCat))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCat is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	EnemyCat->BeginFistAttack();
}

void UNxFistAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	ANxEnemyCat* EnemyCat = Cast<ANxEnemyCat>(MeshComp->GetOwner());
	if (!IsValid(EnemyCat))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCat is null, Function name: %s"), *FString(__FUNCTION__));
		return;
	}
	EnemyCat->EndFistAttack();
}
