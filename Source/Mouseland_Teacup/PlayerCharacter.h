#pragma once

#include "CoreMinimal.h"
#include "Mouseland_TeacupCharacter.h"
#include "Item.h"
#include "NiagaraSystem.h"
#include "PlayerCharacter.generated.h"

class ATeacupStage;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerUseItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAddItem, UItem*, Item);

UCLASS()
class MOUSELAND_TEACUP_API APlayerCharacter : public AMouseland_TeacupCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Interact();
	void OnSprintStart();
	void OnSprintEnd();
	void OnSlowStart();
	void OnSlowEnd();

	void StartDizzy();
	void EndDizzy();
	bool GetIsDizzy() const;

	void FindAndHighlightInteractableObjectNearPlayer();


	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetInventoryItemCount(UItem* Item) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddInventoryItem(UItem* NewItem, int32 ItemCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveInventoryItem(UItem* RemovedItem, int32 RemoveCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseSlotItem();

	UFUNCTION(BlueprintCallable)
	void GameOver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CanSprint() const;
	bool CanSlow() const;
	bool IsJumping() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlowAction;

	TArray<AActor*> CloseActors;
	AActor* FocusActor;

	FTimerHandle FindInteractableTimerHandle;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerAddItem OnPlayerAddItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<UItem*, int32> InventoryData;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	UItem* CurrentSlotItem;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseItem OnPlayerUseItem;

	UPROPERTY(EditDefaultsOnly)
	float FindHighlightInteractiveObjectDistance = 150.0f;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractableObjectTypes;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* GameOverWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> YouWinWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* YouWinWidget;

	UPROPERTY(EditAnywhere)
	ATeacupStage* TeacupStage;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MaxEnergy = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentEnergy;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float EnergyRegenRate = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float SprintCostEnergyPerSec = 35.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SlowSpeed = 100.0f;

	UPROPERTY()
	UNiagaraComponent* DizzyEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> DizzyEffectAsset;

	UPROPERTY()
	UAudioComponent* DizzySound;

	UPROPERTY(EditAnywhere)
	USoundBase* DizzySoundAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItem *CheeseItemType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* RestoreEnergyEffect;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsSlow = false;

	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta=(AllowPrivateAccess = "true"))
	float DizzyDuration = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DizzySpeed = 30.0f;

	bool IsDizzy = false;
	FTimerHandle DizzyTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FVector DizzyEffectOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float SprintFieldOfView = 120.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float NormalFieldOfView = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FieldOfViewInterpSpeed = 1.f;

	UCharacterMovementComponent* CharacterMovementComponent;
	UCameraComponent* CameraComponent;
};
