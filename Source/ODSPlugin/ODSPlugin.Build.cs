// Copyright 2019 Charlie Winter, Inc. All Rights Reserved.


using UnrealBuildTool;

public class ODSPlugin : ModuleRules
{
	public ODSPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] 
            {

			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] 
            {

			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
            {

            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "CinematicCamera",
                "LevelSequence",
                "MovieScene"
	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
			);
	}
}
