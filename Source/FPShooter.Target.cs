// zoonyeok All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FPShooterTarget : TargetRules
{
	public FPShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "FPShooter" } );
	}
}