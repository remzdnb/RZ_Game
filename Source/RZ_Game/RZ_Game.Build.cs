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
			"RZM_Shared",
			"RZM_WeaponSystem",
			"RZM_InventorySystem",
			"RZM_AnimationSystem",
			"RZM_Camera",
			"RZM_BaseWidgets",
			"RZM_UIManager",
			"RZM_World"

		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
