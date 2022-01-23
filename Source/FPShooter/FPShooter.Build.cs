// zoonyeok All Rights Reserved.

using UnrealBuildTool;

public class FPShooter : ModuleRules
{
	public FPShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"Niagara",
			"PhysicsCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"FPShooter/Public/Player",
			"FPShooter/Public/Components", 
			"FPShooter/Public/Dev",
			"FPShooter/Public/Weapon",
			"FPShooter/Public/UI",
			"FPShooter/Public/Animation",
			"FPShooter/Public/Pickups",
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
