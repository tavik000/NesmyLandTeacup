#include "TeacupCloset.h"

#include "PlayerCharacter.h"
#include "Teacup.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"


ATeacupCloset::ATeacupCloset()
{
	PrimaryActorTick.bCanEverTick = true;

	IsActivated = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);

	InteractionHUD = CreateDefaultSubobject<UBillboardComponent>(TEXT("InteractionHUD"));
	InteractionHUD->SetupAttachment(RootComp);
	InteractionHUD->SetRelativeLocation(FVector(0.0f, -48.0f, 31.0f));
	InteractionHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	InteractionHUD->SetHiddenInGame(false);
}

void ATeacupCloset::BeginPlay()
{
	Super::BeginPlay();

	InteractionHUD->SetVisibility(false);


	if (Teacups.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "Teacpus is null!, function: Teacup::Interact_Implementation()")));
		return;
	}
	for (AActor* Teacup : Teacups)
	{
		Teacup->SetActorHiddenInGame(true);
	}

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


void ATeacupCloset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeacupCloset::Interact_Implementation(APlayerCharacter* InteractCharacter)
{
	if (Teacups.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "Teacpus is null!, function: Teacup::Interact_Implementation()")));
		return;
	}
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InteractCharacter);
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "PlayerCharacter is null!, function: Teacup::Interact_Implementation()")));
		return;
	}

	if (PlayerCharacter->HasItem(RequireTeacupItemType))
	{
		IInteractableInterface::Interact_Implementation(InteractCharacter);
		PlayerCharacter->RemoveInventoryItem(RequireTeacupItemType, 1);

		// for each teacup, and set the teacup to be visible
		for (AActor* Teacup : Teacups)
		{
			if (Teacup->IsHidden())
			{
				Teacup->SetActorHiddenInGame(false);
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlaceSound, GetActorLocation());
				return;
			}
		}
	}
}

void ATeacupCloset::ToggleOutline_Implementation(bool bValue)
{
	IInteractableInterface::ToggleOutline_Implementation(bValue);
	InteractionHUD->SetVisibility(bValue);
}

bool ATeacupCloset::IsEnable_Implementation()
{
	return true;
}

void ATeacupCloset::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();

	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &ATeacupCloset::CheckAndUpdateWidgetVisible_Implementation, 0.1f, true);
}

void ATeacupCloset::CheckAndUpdateWidgetVisible_Implementation()
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

bool ATeacupCloset::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}

int32 ATeacupCloset::GetTeacupCount()
{
	int32 Result = 0;
	for (const AActor* Teacup : Teacups)
	{
		if (!Teacup->IsHidden())
		{
			Result++;
		}
	}
	return Result;
}
