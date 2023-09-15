// Copyright Artem Volovyi. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MultiplayerGameEditorTarget : TargetRules
{
	public MultiplayerGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "MultiplayerGame" } );
	}
}
