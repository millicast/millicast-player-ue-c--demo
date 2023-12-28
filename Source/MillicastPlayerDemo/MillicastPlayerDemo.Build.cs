// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MillicastPlayerDemo : ModuleRules
{
	public MillicastPlayerDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MillicastPlayer", "MillicastWebRTC" });

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
