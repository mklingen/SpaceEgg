// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceEgg : ModuleRules
{
	public SpaceEgg(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "SlateCore" });
	}
}
