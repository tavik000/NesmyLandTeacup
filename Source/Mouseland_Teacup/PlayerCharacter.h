
#pragma once

#include "CoreMinimal.h"
#include "Mouseland_TeacupCharacter.h"
#include "Item.h"
#include "PlayerCharacter.generated.h"

class ATeacupStage;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerUseItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAddItem, UItem*, Item);

UCLASS()
class MOUSELAND_TEACUP_API APlayerCharacter : public AMouseland_TeacupCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Interact();
	void FindAndHighlightInteractableObjectNearPlayer();
	void AddInventoryItem(bool IsTeacup);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	
	TArray<AActor*> CloseActors;
	AActor* FocusActor;
	
	FTimerHandle FindInteractableTimerHandle;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerAddItem OnPlayerAddItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<UItem*, int32> InventoryData;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	UItem* CurrentSlotItem;

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
	
};
