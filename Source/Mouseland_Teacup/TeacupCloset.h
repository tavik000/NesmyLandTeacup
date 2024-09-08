// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "Subtitle.h"
#include "Teacup.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "TeacupCloset.generated.h"

UCLASS()
class MOUSELAND_TEACUP_API ATeacupCloset : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeacupCloset();
	void BeginPlay();



	
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(APlayerCharacter* InteractCharacter) override;
	virtual void ToggleOutline_Implementation(bool bValue) override;
	virtual bool IsEnable_Implementation() override;
	virtual void StartCheckAndUpdateWidgetVisibleTimer_Implementation() override;
	virtual void CheckAndUpdateWidgetVisible_Implementation() override;
	virtual bool IsInteractiveHUDVisible_Implementation() override;


	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsActivated;

	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;

	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteract OnInteract;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItem *RequireTeacupItemType;

	int32 GetTeacupCount();

public:


	UPROPERTY(EditAnywhere)
	ASubtitle* Subtitle;
	
	UPROPERTY(EditAnywhere)
	FText NoTeacupText;
	
	
	UPROPERTY(EditAnywhere)
	TArray<ATeacup*> Teacups;

	UPROPERTY(EditAnywhere)
	USoundBase* PlaceSound;
	
private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;

	ATeacupStage* TeacupStage;
	
};
