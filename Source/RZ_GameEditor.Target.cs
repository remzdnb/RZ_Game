using UnrealBuildTool;
using System.Collections.Generic;

public class RZ_GameEditorTarget : TargetRules
{
	public RZ_GameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RZ_Game", "S2D_Game", "S3D_Game"  } );
	}
}
