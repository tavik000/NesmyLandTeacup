// Fill out your copyright notice in the Description page of Project Settings.


#include "NxEnemyAIPerception.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"


// Sets default values for this component's properties
UNxEnemyAIPerception::UNxEnemyAIPerception()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNxEnemyAIPerception::Init()
{
	SightSenseID = UAISense::GetSenseID<UAISense_Sight>();
}

void UNxEnemyAIPerception::BeginPlay()
{
	Super::BeginPlay();
}

float UNxEnemyAIPerception::GetSightRadius()
{
	const UAISenseConfig_Sight* Config_Sight = Cast<UAISenseConfig_Sight>(GetSenseConfig(SightSenseID));
	if (!IsValid(Config_Sight))
	{
		UE_LOG(LogTemp, Error, TEXT("GetSightRadius: Sight is not the dominant sense! Config Not found!"));
		return 0;
	}
	return Config_Sight->SightRadius;
}

void UNxEnemyAIPerception::SetSightRadius(float InRadius)
{
	if (UAISenseConfig_Sight* Config_Sight = Cast<UAISenseConfig_Sight>(GetSenseConfig(SightSenseID));
		IsValid(Config_Sight))
	{
		Config_Sight->SightRadius = InRadius;
		ConfigureSense(*Config_Sight);
	}
}

float UNxEnemyAIPerception::GetPeripheralVisionDegree()
{
	const UAISenseConfig_Sight* Config_Sight = Cast<UAISenseConfig_Sight>(GetSenseConfig(SightSenseID));
	if (!IsValid(Config_Sight))
	{
		UE_LOG(LogTemp, Error, TEXT("GetPeripheralVisionDegree: Sight is not the dominant sense! Config Not found!"));
		return 0;
	}
	return Config_Sight->PeripheralVisionAngleDegrees;
}

void UNxEnemyAIPerception::SetPeripheralVisionDegree(float InHalfDegree)
{
	if (UAISenseConfig_Sight* Config_Sight = Cast<UAISenseConfig_Sight>(GetSenseConfig(SightSenseID));
		IsValid(Config_Sight))
	{
		Config_Sight->PeripheralVisionAngleDegrees = InHalfDegree;
		ConfigureSense(*Config_Sight);
	}
}

float UNxEnemyAIPerception::GetLoseSightRadius()
{
	const UAISenseConfig_Sight* Config_Sight = Cast<UAISenseConfig_Sight>(GetSenseConfig(SightSenseID));
	if (!IsValid(Config_Sight))
	{
		UE_LOG(LogTemp, Error, TEXT("GetLoseSightRadius: Sight is not the dominant sense! Config Not found!"));
		return 0;
	}
	return Config_Sight->LoseSightRadius;
}

void UNxEnemyAIPerception::SetLoseSightRadius(float InRadius)
{
	if (UAISenseConfig_Sight* Config_Sight = Cast<UAISenseConfig_Sight>(GetSenseConfig(SightSenseID));
		IsValid(Config_Sight))
	{
		Config_Sight->LoseSightRadius = InRadius;
		ConfigureSense(*Config_Sight);
	}
}
