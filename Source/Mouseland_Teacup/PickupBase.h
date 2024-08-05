#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "PickupBase.generated.h"

class ASubtitle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UCLASS()
class MOUSELAND_TEACUP_API APickupBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	bool GiveItem();

	virtual void Interact_Implementation(APlayerCharacter* InteractCharacter) override;
	virtual void ToggleOutline_Implementation(bool bValue) override;
	virtual bool IsEnable_Implementation() override;
	virtual void StartCheckAndUpdateWidgetVisibleTimer_Implementation() override;
	virtual void CheckAndUpdateWidgetVisible_Implementation() override;
	virtual void SetupOutline_Implementation() override;
	virtual bool IsInteractiveHUDVisible_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool CanPickup(APlayerCharacter* PickingCharacter);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* MeshOutline;

	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere)
	UItem* ItemType;

	UPROPERTY(EditAnywhere)
	USoundBase* GiveItemSound;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* OutlineMaterial;
	
	UPROPERTY(EditAnywhere)
	ASubtitle* Subtitle;
	
	UPROPERTY(EditAnywhere)
	FText CannotPickText;
	
private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;
};
