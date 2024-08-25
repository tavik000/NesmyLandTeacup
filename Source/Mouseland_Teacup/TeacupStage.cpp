#include "TeacupStage.h"

#include "PlayerCharacter.h"
#include "Teacup.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"


ATeacupStage::ATeacupStage()
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
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 0.0f, 00.0f);
}

void ATeacupStage::BeginPlay()
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
		Teacup->SetActorEnableCollision(false);
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

void ATeacupStage::StartSpin()
{
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 30.0f, 0.0f);
	OnStartSpin.Broadcast();
}

void ATeacupStage::StopSpin()
{
	RotatingMovementComponent->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	InteractionHUD->SetSprite(CleanSprite);
	OnStopSpin.Broadcast();
}

void ATeacupStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATeacupStage::TryAddMouseFriend(APlayerCharacter* PlayerCharacter)
{
	for (ATeacup* Teacup : Teacups)
	{
		if (!Teacup->IsHidden() && Teacup->CanAddMouseFriend())
		{
			Teacup->TryAddMouseFriend();
			return true;
		}
	}
	return false;
}

void ATeacupStage::Interact_Implementation(APlayerCharacter* InteractCharacter)
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

	if (!IsActivated)
	{
		if (PlayerCharacter->HasItem(RequireTeacupItemType))
		{
			IInteractableInterface::Interact_Implementation(InteractCharacter);
			PlayerCharacter->RemoveInventoryItem(RequireTeacupItemType, 1);

			// for each teacup, and set the teacup to be visible
			for (AActor* Teacup : Teacups)
			{
				if (Teacup->IsHidden())
				{
					if (!PlaceSound)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
						                                 FString::Printf(TEXT("PlaceSound is null!")));
					}
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlaceSound, GetActorLocation());
					Teacup->SetActorHiddenInGame(false);
					Teacup->SetActorEnableCollision(true);

					if (CanStart() && !PlayerCharacter->HasItem(RequireTeacupItemType) && !PlayerCharacter->HasItem(
						RequireMouseFriendItemType))
					{
						InteractionHUD->SetSprite(StartSprite);
					}
					return;
				}
			}
		}

		if (PlayerCharacter->HasItem(RequireMouseFriendItemType))
		{
			IInteractableInterface::Interact_Implementation(InteractCharacter);

			// for each teacup, and set the teacup to be visible
			if (TryAddMouseFriend(PlayerCharacter))
			{
				PlayerCharacter->RemoveInventoryItem(RequireMouseFriendItemType, 1);

				if (!PlaceSound)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
					                                 FString::Printf(TEXT("PlaceSound is null!")));
				}
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlaceSound, GetActorLocation());

				if (CanStart() && !PlayerCharacter->HasItem(RequireTeacupItemType) && !PlayerCharacter->HasItem(
					RequireMouseFriendItemType))
				{
					InteractionHUD->SetSprite(StartSprite);
				}
				return;
			}
			if (!IsValid(Subtitle))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
				                                 FString::Printf(TEXT(
					                                 "Subtitle is null!, function: Teacup::Interact_Implementation()")));
				return;
			}


			Subtitle->ShowSubtitleFromScript(NoTeacupText.ToString(), 8.0f);
			return;
		}

		if (CanStart())
		{
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
			//                                  FString::Printf(TEXT("Stage Start")));
			TryAddMouseFriend(PlayerCharacter);

			IsActivated = true;
			// Start Cutscene
			StartSpin();
			for (ATeacup* Teacup : Teacups)
			{
				Teacup->StartSpin();
			}

			OnInteract.Broadcast();
			// TODO after cutscene, set the mouse to be visible
		}
	}
	else
	{
		if (InteractCharacter->HasItem(RequireTeacupItemType))
		{
			Subtitle->ShowSubtitleFromScript(TooHeavyText.ToString(), 8.0f);
			return;
		}

		for (ATeacup* Teacup : Teacups)
		{
			if (!Teacup->IsHidden())
			{
				Teacup->SetActorHiddenInGame(true);
				Teacup->SetActorEnableCollision(false);
				InteractCharacter->AddInventoryItem(RequireTeacupItemType, 1);
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlaceSound, GetActorLocation());
				return;
			}
		}
	}
}

void ATeacupStage::StopPlay()
{
	for (ATeacup* Teacup : Teacups)
	{
		Teacup->StopSpin();
		Teacup->HideAllMouseFriends();
	}
	StopSpin();
}

void ATeacupStage::ToggleOutline_Implementation(bool bValue)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsActivated)
	{
		if (CanStart() && !PlayerCharacter->HasItem(RequireTeacupItemType) && !PlayerCharacter->HasItem(
			RequireMouseFriendItemType))
		{
			InteractionHUD->SetSprite(StartSprite);
		}
		else
		{
			InteractionHUD->SetSprite(PlaceSprite);
		}
	}
	else
	{
		InteractionHUD->SetSprite(CleanSprite);
	}

	IInteractableInterface::ToggleOutline_Implementation(bValue);
	InteractionHUD->SetVisibility(bValue);
}

bool ATeacupStage::IsEnable_Implementation()
{
	return true;
}

void ATeacupStage::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();

	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &ATeacupStage::CheckAndUpdateWidgetVisible_Implementation, 0.1f, true);
}

void ATeacupStage::CheckAndUpdateWidgetVisible_Implementation()
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

bool ATeacupStage::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}

int32 ATeacupStage::GetTeacupCount()
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

int32 ATeacupStage::GetHappy()
{
	int32 Result = 0;
	for (const ATeacup* Teacup : Teacups)
	{
		if (!Teacup->IsHidden())
		{
			for (AActor*
			     MouseFriend : Teacup->MouseFriends)
			{
				if (!MouseFriend->IsHidden())
				{
					Result++;
				}
			}
		}
	}
	return Result;
}

bool ATeacupStage::CanStart()
{
	return GetTeacupCount() >= 1;
}
