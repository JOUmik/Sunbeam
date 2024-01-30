// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sunbeam : ModuleRules
{
	public Sunbeam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[]
		{
			"Sunbeam"
		});
		PrivateIncludePaths.AddRange(new string[]
		{
			"Sunbeam"
		});
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "SERIALCOM" });
	}
}
