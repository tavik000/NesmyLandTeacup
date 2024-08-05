#include "PickupBase.h"

#include "PlayerCharacter.h"
#include "Subtitle.h"
#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	MeshOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshOutline"));
	MeshOutline->SetupAttachment(Mesh);
	InteractionHUD = CreateDefaultSubobject<UBillboardComponent>(TEXT("InteractionHUD"));
	InteractionHUD->SetupAttachment(RootComp);
	InteractionHUD->SetRelativeLocation(FVector(0.0f, -48.0f, 31.0f));
	InteractionHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	InteractionHUD->SetHiddenInGame(false);
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComp);
	FColor ArrowColor = FColor::FromHex(TEXT("7F7CFFFF"));
	Arrow->SetArrowColor(ArrowColor);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	SetupOutline_Implementation();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASubtitle::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("ASubtitle is not found!")));
		return;
	}

	Subtitle = Cast<ASubtitle>(FoundActors[0]);
}

void APickupBase::SetupOutline_Implementation()
{
	IInteractableInterface::SetupOutline_Implementation();
	MeshOutline->SetStaticMesh(Mesh->GetStaticMesh());
	MeshOutline->SetRelativeScale3D(FVector(1.02f, 1.02f, 1.02f));

	if (!OutlineMaterial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "OutlineMaterial is null, function: AAPickup::SetupOutline_Implementation()")));
		UE_LOG(LogTemp, Error, TEXT("OutlineMaterial is null, function: AAPickup::SetupOutline_Implementation()"));
	}

	for (int i = 0; i <= Mesh->GetNumMaterials() - 1; i++)
	{
		// UMaterialInterface* OutlineMaterial = Cast<UMaterialInterface>(
		// 	StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/Materials/M_MeshOutline")));
		MeshOutline->SetMaterial(i, OutlineMaterial);
	}
	MeshOutline->SetVisibility(false);
	InteractionHUD->SetVisibility(false);
}

bool APickupBase::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


bool APickupBase::GiveItem()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is null!")));
		return false;
	}
	const bool IsGiveItemSuccessful = PlayerCharacter->AddInventoryItem(ItemType, 1);
	if (IsGiveItemSuccessful)
	{
		if (!GiveItemSound)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("GiveItemSound is null!")));
		}
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GiveItemSound, GetActorLocation());
		Destroy();
	}
	return IsGiveItemSuccessful;
}

void APickupBase::Interact_Implementation(APlayerCharacter* InteractCharacter)
{
	if (CanPickup(InteractCharacter))
	{
		IInteractableInterface::Interact_Implementation(InteractCharacter);
		OnInteract.Broadcast();
		if (ItemType)
		{
			GiveItem();
		}
		else
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GiveItemSound, GetActorLocation());
			Destroy();
		}
	}
	else
	{
		Subtitle->ShowSubtitleFromScript(CannotPickText.ToString(), 8.0f);
	}
}

bool APickupBase::CanPickup(APlayerCharacter* PickingCharacter)
{
	if (PickingCharacter->HasItem(ItemType))
	{
		return false;
	}
	return true;
}

void APickupBase::ToggleOutline_Implementation(bool bValue)
{
	IInteractableInterface::ToggleOutline_Implementation(bValue);
	MeshOutline->SetVisibility(bValue);
	InteractionHUD->SetVisibility(bValue);
}

bool APickupBase::IsEnable_Implementation()
{
	return true;
}

void APickupBase::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();
	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &APickupBase::CheckAndUpdateWidgetVisible_Implementation, 0.1f, true);
}

void APickupBase::CheckAndUpdateWidgetVisible_Implementation()
{
	IInteractableInterface::CheckAndUpdateWidgetVisible_Implementation();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is null!")));
		return;
	}
	if (PlayerCharacter->FocusActor != this || !PlayerCharacter->CloseActors.Contains(this))
	{
		ToggleOutline_Implementation(false);
		PlayerCharacter->FocusActor = nullptr;
		GetWorldTimerManager().ClearTimer(CheckAndUpdateWidgetVisibleTimer);
	}
}
