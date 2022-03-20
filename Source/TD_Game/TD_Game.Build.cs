using UnrealBuildTool;

public class TD_Game : ModuleRules
{
	public TD_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"InputCore",
			"UMG",
			"RZ_Game",
			"RZM_CharacterActor",
			"RZM_CharacterAnim",
			"RZM_ItemActor",
			"RZM_ItemManager",
			"RZM_ItemUI",
			"RZ_CameraPlugin",
			"RZ_UIElementsPlugin",
			"RZ_UIManagerPlugin"

		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
