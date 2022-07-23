using UnrealBuildTool;

public class RZ_Game : ModuleRules
{
	public RZ_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"AIModule",
			"Slate",
			"SlateCore",
			"UMG",
			"RZ_Common",
			"RZ_BaseWidgets",
			"RZ_AttributeSystem",
			"RZ_BuildingSystem",
			"RZ_PowerSystem",
			"RZ_InventorySystem",
			"RZ_PerceptionSystem",
			"RZ_AnimationSystem",
			"RZ_CameraSystem",
			"RZ_UISystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
