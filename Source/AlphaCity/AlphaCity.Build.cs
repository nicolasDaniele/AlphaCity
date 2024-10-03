// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlphaCity : ModuleRules
{
	public AlphaCity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameLiftServerSDK" });
		bEnableExceptions = true;

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemSteam" });


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        const string outputBinDir = "$(TargetOutputDir)";
        const string gameLiftFilesDir = "$(ProjectDir)/Plugins/GameLiftPlugin/Libs";
        RuntimeDependencies.Add($"{outputBinDir}/libcrypto-3-x64.dll", $"{gameLiftFilesDir}/libcrypto-3-x64.dll");
        RuntimeDependencies.Add($"{outputBinDir}/libssl-3-x64.dll", $"{gameLiftFilesDir}/libssl-3-x64.dll");
    }
}
