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
			"AIModule",
			"GameplayTasks",
			"GameplayTags",
			"Slate",
			"SlateCore",
			"UMG",
			"RZM_Camera",
			"RZM_BaseWidgets",
			"RZM_UIManager",
			"RZM_CharacterActor",
			"RZM_CharacterAnim",
			"RZM_ItemActor",
			"RZM_ItemManager",
			"RZM_ItemUI",
			"RZM_WorldCore",
			"RZM_WorldGrid",

		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
