// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Item.h"
#include "TeacupStage.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/InteractableInterface.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(FindInteractableTimerHandle, this,
	                                &APlayerCharacter::FindAndHighlightInteractableObjectNearPlayer, 0.1f, true);

	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	CameraComponent = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));

	WalkSpeed = CharacterMovementComponent->MaxWalkSpeed;

	GetWorldTimerManager().SetTimer(AlignFloorTimerHandle, this, &APlayerCharacter::AlignFloor, 0.1f, true);

	LoadDizzyEffectAsset();
}

bool APlayerCharacter::CanSprint() const
{
	return !IsJumping() && !IsDizzy;
}

bool APlayerCharacter::CanSlow() const
{
	return !IsJumping() && !IsDizzy;
}

bool APlayerCharacter::IsJumping() const
{
	return CharacterMovementComponent->IsFalling();
}

void APlayerCharacter::LoadDizzyEffectAsset()
{
	UE_LOG(LogTemp, Log, TEXT("DizzyEffectAsset requeset load"));
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(DizzyEffectAsset.ToSoftObjectPath(),
	                                                             FStreamableDelegate::CreateUObject(
		                                                             this, &APlayerCharacter::OnDizzyEffectLoaded));
}

void APlayerCharacter::OnDizzyEffectLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("DizzyEffectLoaded"));

	if (IsValid(DizzyEffectAsset.Get()))
	{
		UE_LOG(LogTemp, Log, TEXT("DizzyEffectAsset is valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DizzyEffectAsset is null, Function name: %s"), *FString(__FUNCTION__));
	}
}


// Called every frame

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDizzy)
	{
		if (IsSprinting)
		{
			if (CurrentEnergy < SprintCostEnergyPerSec * DeltaTime)
			{
				OnSprintEnd();
			}
			else
			{
				CurrentEnergy = FMath::Min(MaxEnergy, CurrentEnergy - SprintCostEnergyPerSec * DeltaTime);
			}
		}
		else
		{
			CurrentEnergy = FMath::Min(MaxEnergy, CurrentEnergy + EnergyRegenRate * DeltaTime);
		}
	}

	if (IsSprinting)
	{
		CameraComponent->FieldOfView = FMath::FInterpTo(CameraComponent->FieldOfView, SprintFieldOfView, DeltaTime,
		                                                FieldOfViewInterpSpeed);
	}
	else
	{
		CameraComponent->FieldOfView = FMath::FInterpTo(CameraComponent->FieldOfView, NormalFieldOfView, DeltaTime,
		                                                FieldOfViewInterpSpeed);
	}
}

void APlayerCharacter::AlignFloor() const
{
	const FVector MeshLocation = GetMesh()->GetComponentLocation() + 1.f * FVector::UpVector;
	const FVector MeshDownLocation = MeshLocation - 1000.f * FVector::UpVector;
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	const bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, MeshLocation, MeshDownLocation, ECC_WorldStatic,
	                                                        CollisionQueryParams);
	if (IsHit)
	{
		// DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + 10.f * HitResult.ImpactNormal, FColor::Green, false, 1.0f, 0, 1.0f);
		FVector FloorNormal = HitResult.ImpactNormal;
		FVector RightVector = GetActorRightVector();
		FVector UpVector = GetActorUpVector();
		float SlopePitch;
		float SlopeRoll;
		UKismetMathLibrary::GetSlopeDegreeAngles(RightVector, FloorNormal, UpVector, SlopePitch, SlopeRoll);
		// UE_LOG(LogTemp, Warning, TEXT("RightVector: %s, FloorNormal: %s, UpVector: %s, SlopePitch: %f, SlopeRoll: %f"),
		//              *RightVector.ToString(), *FloorNormal.ToString(), *UpVector.ToString(), SlopePitch, SlopeRoll);
		SlopePitch = -SlopePitch;
		const float MeshYaw = GetMesh()->GetComponentRotation().Yaw;
		const float MeshPitch = GetMesh()->GetComponentRotation().Pitch;
		const FRotator FloorRotation = FRotator(MeshPitch, MeshYaw, SlopePitch);
		GetMesh()->SetWorldRotation(FloorRotation);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::Interact);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::OnSprintStart);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::OnSprintEnd);

		EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::OnSlowStart);

		EnhancedInputComponent->BindAction(SlowAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::OnSlowEnd);
	}
}

void APlayerCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));

	if (!IsValid(FocusActor))
	{
		return;
	}

	if (FocusActor)
	{
		IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(FocusActor);
		if (InteractableInterface)
		{
			if (InteractableInterface->Execute_IsEnable(FocusActor))
			{
				InteractableInterface->Execute_Interact(FocusActor, this);
			}
		}
	}
}

void APlayerCharacter::OnSprintStart()
{
	if (IsSprinting) return;
	if (!CanSprint()) return;
	IsSprinting = true;
	CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::OnSprintEnd()
{
	if (!IsSprinting) return;
	IsSprinting = false;
	if (IsDizzy) return;
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::OnSlowStart()
{
	if (IsSprinting) return;
	if (!CanSlow()) return;
	IsSlow = true;
	CharacterMovementComponent->MaxWalkSpeed = SlowSpeed;
}

void APlayerCharacter::OnSlowEnd()
{
	if (!IsSlow) return;
	IsSlow = false;
	if (IsDizzy)
	{
		return;
	}
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::StartDizzy()
{
	if (IsDizzy)
	{
		return;
	}
	CharacterMovementComponent->MaxWalkSpeed = DizzySpeed;


	IsDizzy = true;

	UNiagaraSystem* DizzyEffectSystem = DizzyEffectAsset.Get();
	if (!IsValid(DizzyEffectSystem))
	{
		UE_LOG(LogTemp, Error, TEXT("DizzyEffectSystem is null, Function name: %s"), *FString(__FUNCTION__));
	}
	else
	{
		// TODO: onload spawn effect delegate
	}
	DizzyEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(DizzyEffectSystem, SceneComponent, NAME_None,
	                                                           DizzyEffectOffset, FRotator::ZeroRotator,
	                                                           EAttachLocation::KeepRelativeOffset, true);

	if (!IsValid(DizzySoundAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("DizzySound is null, Function name: %s"), *FString(__FUNCTION__));
	}
	else
	{
		DizzySound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DizzySoundAsset, GetActorLocation());
	}
	GetWorldTimerManager().SetTimer(DizzyTimerHandle, this, &APlayerCharacter::EndDizzy, DizzyDuration, false);
}

void APlayerCharacter::EndDizzy()
{
	if (IsValid(DizzyEffect))
	{
		DizzyEffect->DestroyComponent();
	}
	IsDizzy = false;
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
	DizzySound->Stop();
}

bool APlayerCharacter::GetIsDizzy() const
{
	return IsDizzy;
}

void APlayerCharacter::FindAndHighlightInteractableObjectNearPlayer()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	bool IsHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(),
	                                                       FindHighlightInteractiveObjectDistance,
	                                                       InteractableObjectTypes, nullptr,
	                                                       IgnoredActors, CloseActors);
	if (IsHit)
	{
		float MinDistance = 1000000;
		AActor* ClosestInteractableObject = nullptr;
		for (AActor* OverlappingActor : CloseActors)
		{
			float Distance = GetDistanceTo(OverlappingActor);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestInteractableObject = OverlappingActor;
			}
		}

		FocusActor = ClosestInteractableObject;
		IInteractableInterface* InteractableInterface = Cast<IInteractableInterface>(ClosestInteractableObject);
		if (InteractableInterface)
		{
			if (!InteractableInterface->Execute_IsInteractiveHUDVisible(ClosestInteractableObject) &&
				InteractableInterface->Execute_IsEnable(ClosestInteractableObject))
			{
				InteractableInterface->Execute_ToggleOutline(ClosestInteractableObject, true);
				InteractableInterface->Execute_StartCheckAndUpdateWidgetVisibleTimer(ClosestInteractableObject);
			}
		}
	}
}

int32 APlayerCharacter::GetInventoryItemCount(UItem* Item) const
{
	if (const int32* FoundItemCount = InventoryData.Find(Item))
	{
		return *FoundItemCount;
	}
	return 0;
}

bool APlayerCharacter::HasItem(UItem* Item)
{
	if (!IsValid(Item))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("HasItem: Item is null!")));
		return false;
	}
	return InventoryData.Contains(Item) && InventoryData[Item] > 0;
}

bool APlayerCharacter::AddInventoryItem(UItem* NewItem, int32 ItemCount)
{
	bool IsAddSuccessful;
	if (!NewItem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("AddInventoryItem: Failed trying to add null item!")));
		return false;
	}
	if (ItemCount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("AddInventoryItem: ItemCount must be greater than 0!")));
		return false;
	}

	if (NewItem == CheeseItemType)
	{
		CurrentEnergy = MaxEnergy;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		                                         RestoreEnergyEffect,
		                                         GetActorLocation(), FRotator::ZeroRotator,
		                                         0.1f * FVector(1.0f, 1.0f, 1.0f));

		return true;
	}
	int32 MaxCount = NewItem->MaxCount;
	if (MaxCount <= 0)
	{
		MaxCount = MAX_int32;
	}
	if (InventoryData.Contains(NewItem))
	{
		int32 OldItemCount = InventoryData[NewItem];
		InventoryData[NewItem] = FMath::Clamp(OldItemCount + ItemCount, 1, MaxCount);
		IsAddSuccessful = true;
	}
	else
	{
		InventoryData.Add(NewItem, ItemCount);
		IsAddSuccessful = true;
		OnPlayerAddItem.Broadcast(NewItem);
	}
	// Temp set slotItem
	return IsAddSuccessful;
}

bool APlayerCharacter::RemoveInventoryItem(UItem* RemovedItem, int32 RemoveCount)
{
	bool IsRemoveSuccessful;
	if (!RemovedItem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("RemoveInventoryItem: Failed trying to remove null item!")));
		return false;
	}

	int32 ItemCount = GetInventoryItemCount(RemovedItem);
	if (ItemCount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "RemoveInventoryItem: Failed trying to remove item with 0 count!")));
		return false;
	}

	// If RemoveCount <= 0, delete all
	if (RemoveCount <= 0)
	{
		ItemCount = 0;
	}
	else
	{
		ItemCount = FMath::Clamp(ItemCount - RemoveCount, 0, RemovedItem->MaxCount);
	}

	if (ItemCount > 0)
	{
		InventoryData[RemovedItem] = ItemCount;
		IsRemoveSuccessful = true;
	}
	else
	{
		InventoryData.Remove(RemovedItem);
		IsRemoveSuccessful = true;
	}
	return IsRemoveSuccessful;
}

bool APlayerCharacter::UseItem(UItem* Item)
{
	if (GetInventoryItemCount(Item) <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No Item in Inventory")));
		return false;
	}

	bool IsRemoveItemSuccessful = RemoveInventoryItem(Item, 1);
	if (IsRemoveItemSuccessful)
	{
		// Vinegar Only
		// Heal(MaxHp * 0.5f);
		OnPlayerUseItem.Broadcast();
	}
	return IsRemoveItemSuccessful;
}

bool APlayerCharacter::UseSlotItem()
{
	return UseItem(CurrentSlotItem);
}

void APlayerCharacter::GameOver()
{
	// Find Actor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeacupStage::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("ATeacupStage is not found!")));
		return;
	}
	TeacupStage = Cast<ATeacupStage>(FoundActors[0]);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);

	if (TeacupStage->GetTeacupCount() >= 1)
	{
		if (!GameOverWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("MainMenuWidgetClass is null, Function name: %s"), *FString(__FUNCTION__));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			                                 FString::Printf(
				                                 TEXT("MainMenuWidgetClass is null, Function name: %s"),
				                                 *FString(__FUNCTION__)));
			return;
		}
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		GameOverWidget->AddToViewport();
	}
	else
	{
		if (!YouWinWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("MainMenuWidgetClass is null, Function name: %s"), *FString(__FUNCTION__));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			                                 FString::Printf(
				                                 TEXT("MainMenuWidgetClass is null, Function name: %s"),
				                                 *FString(__FUNCTION__)));
			return;
		}
		YouWinWidget = CreateWidget<UUserWidget>(GetWorld(), YouWinWidgetClass);
		YouWinWidget->AddToViewport();
	}
}
