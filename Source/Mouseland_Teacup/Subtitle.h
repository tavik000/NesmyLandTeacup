﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subtitle.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ASubtitle : public AActor
{
	GENERATED_BODY()

public:
	ASubtitle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSubtitleFromScript(const FString& SubtitleText, float Duration);
};
