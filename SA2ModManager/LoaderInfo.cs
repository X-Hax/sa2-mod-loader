using ModManagerCommon;
using System.Collections.Generic;
using System.ComponentModel;

namespace SA2ModManager
{
	class SA2LoaderInfo : LoaderInfo
	{
		public bool DebugConsole { get; set; }
		public bool DebugScreen { get; set; }
		public bool DebugFile { get; set; }
		[DefaultValue(true)]
		public bool DebugCrashLog { get; set; }
		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }
		[DefaultValue(true)]
		public bool PauseWhenInactive { get; set; }
		[DefaultValue(false)]
		public bool DisableExitPrompt { get; set; }
		[DefaultValue(1)]
		public int ScreenNum { get; set; } = 1;
		[DefaultValue(false)]
		public bool BorderlessWindow { get; set; }
		[DefaultValue(false)]
		public bool FullScreen { get; set; }
		public bool SkipIntro { get; set; }
		[DefaultValue(640)]
		public int HorizontalResolution { get; set; } = 640;
		[DefaultValue(480)]
		public int VerticalResolution { get; set; } = 480;
		[DefaultValue(1)]
		public int VoiceLanguage { get; set; } = 1;
		[DefaultValue(0)]
		public int TextLanguage { get; set; } = 0;
		public bool CustomWindowSize { get; set; }
		[DefaultValue(640)]
		public int WindowWidth { get; set; } = 640;
		[DefaultValue(480)]
		public int WindowHeight { get; set; } = 480;
		public bool ResizableWindow { get; set; }
		public bool MaintainAspectRatio { get; set; }
		[DefaultValue(-1)]
		public int TestSpawnLevel { get; set; } = -1;
		[DefaultValue(-1)]
		public int TestSpawnCharacter { get; set; } = -1;
		[DefaultValue(0)]
		public int TestSpawnPlayer2{ get; set; } = -1;
		[DefaultValue(0)]
		public int TestSpawnX { get; set; } = 0;
		[DefaultValue(0)]
		public int TestSpawnY { get; set; } = 0;
		[DefaultValue(0)]
		public int TestSpawnZ { get; set; } = 0;
		[DefaultValue(false)]
		public bool TestSpawnPositionEnabled { get; set; } = false;

		[DefaultValue(-1)]
		public int TestSpawnEvent { get; set; } = -1;
		[DefaultValue(-1)]
		public int TestSpawnSaveID { get; set; } = -1;

		public SA2LoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}
	}
}