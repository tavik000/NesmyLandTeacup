// Copyright Epic Games, Inc. All Rights Reserved.

#include "Mouseland_TeacupGameMode.h"
#include "Mouseland_TeacupCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMouseland_TeacupGameMode::AMouseland_TeacupGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
