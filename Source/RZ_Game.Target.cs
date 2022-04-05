using UnrealBuildTool;
using System.Collections.Generic;

public class RZ_GameTarget : TargetRules
{
	public RZ_GameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RZ_Game", "S2D_Game", "S3D_Game" } );
	}
}
