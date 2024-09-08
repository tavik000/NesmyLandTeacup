// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Mouseland_Teacup : ModuleRules
{
	public Mouseland_Teacup(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara" });
	}
}
