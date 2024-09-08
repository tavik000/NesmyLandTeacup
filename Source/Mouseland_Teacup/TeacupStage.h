// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "Subtitle.h"
#include "Teacup.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "TeacupStage.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartSpin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopSpin);

UCLASS()
class MOUSELAND_TEACUP_API ATeacupStage : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeacupStage();
	void BeginPlay();


	void StartSpin();
	void StopSpin();
	
	bool CanStart();

	UFUNCTION(BlueprintCallable)
	void StopPlay();
	
	UFUNCTION(BlueprintCallable)
	int32 GetHappy() const;
	
	int32 GetTeacupCount();
	
protected:
	virtual void Tick(float DeltaTime) override;
	bool TryAddMouseFriend(APlayerCharacter* PlayerCharacter);
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
	URotatingMovementComponent* RotatingMovementComponent;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;

	UPROPERTY(EditAnywhere)
	UTexture2D* StartIcon;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteract OnInteract;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItem *RequireTeacupItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItem *RequireMouseFriendItemType;



public:



	UPROPERTY(EditAnywhere)
	ASubtitle* Subtitle;
	
	UPROPERTY(EditAnywhere)
	FText NoTeacupText;
	
	UPROPERTY(EditAnywhere)
	FText TooHeavyText;
	
	UPROPERTY(EditAnywhere)
	TArray<ATeacup*> Teacups;

	UPROPERTY(EditAnywhere)
	UTexture2D* StartSprite;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* PlaceSprite;
	
	UPROPERTY(EditAnywhere)
	UTexture2D* CleanSprite;
	
	UPROPERTY(EditAnywhere)
	USoundBase* PlaceSound;

	UPROPERTY(BlueprintAssignable)
	FOnStartSpin OnStartSpin;

	UPROPERTY(BlueprintAssignable)
	FOnStopSpin OnStopSpin;
	
private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;

	int32 Happy;
	
};
