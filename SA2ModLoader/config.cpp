#include "stdafx.h"
#include "json.hpp"
#include "IniFile.hpp"
#include "FileSystem.h"
#include <ShlObj.h>

using json = nlohmann::json;
using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;

std::wstring currentProfilePath; // Used for crash dumps
vector<std::string> ModList;
vector<std::string> GamePatchList;

std::string GetModName(int index)
{
	return ModList.at(index - 1);
}

int GetModCount()
{
	return ModList.size();
}

char inilangs[]{
	Language_English,
	Language_German,
	Language_Spanish,
	Language_French,
	Language_Italian,
	Language_Japanese
};

void DisplaySettingsLoadError(wstring file)
{
	wstring error = L"Mod Loader settings could not be read. Please run the Mod Manager, save settings and try again.\n\nThe following file was missing: " + file;
	MessageBox(nullptr, error.c_str(), L"SA2 Mod Loader", MB_ICONERROR);
	OnExit(0, 0, 0);
	ExitProcess(0);
}

// List of game patches that may be using the old system
std::string LegacyGamePatchList[] =
{
	"FramerateLimiter",
	"DisableExitPrompt",
	"SyncLoad",
	"ExtendVertexBuffer",
	"EnvMapFix",
	"ScreenFadeFix",
	"CECarFix",
	"ParticlesFix",
};

bool IsGamePatchEnabled(const char* patchName)
{
	return (std::find(std::begin(GamePatchList), std::end(GamePatchList), patchName) != std::end(GamePatchList));
}

void ListGamePatches()
{
	PrintDebug("Enabling %d game patches:\n", GamePatchList.size());
	for (std::string s : GamePatchList)
	{
		PrintDebug("Enabled game patch: %s\n", s.c_str());
	}
}

void LoadModLoaderSettings(LoaderSettings* loaderSettings, std::wstring gamePath)
{
	// Get paths for Mod Loader settings and libraries, normally located in 'Sonic Adventure 2\mods\.modloader'

	wstring loaderDataPath = gamePath + L"\\mods\\.modloader\\";
	loaderSettings->ExtLibPath = loaderDataPath + L"extlib\\";
	wstring profilesFolderPath = loaderDataPath + L"profiles\\";
	wstring profilesJsonPath = profilesFolderPath + L"Profiles.json";

	// If Profiles.json isn't found, assume the old paths system
	if (!Exists(profilesJsonPath))
	{
		// Check 'Sonic Adventure 2\SAManager\SA2' (portable mode) first
		profilesJsonPath = gamePath + L"\\SAManager\\SA2\\Profiles.json";
		if (Exists(profilesJsonPath))
		{
			loaderDataPath = gamePath + L"\\SAManager\\";
		}
		// If that doesn't exist either, assume the settings are in 'AppData\Local\SAManager'
		else
		{
			WCHAR appDataLocalBuf[MAX_PATH];
			// Get the LocalAppData folder and check if it has the profiles json
			if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataLocalBuf)))
			{
				wstring appDataLocalPath(appDataLocalBuf);
				profilesJsonPath = appDataLocalPath + L"\\SAManager\\SA2\\Profiles.json";
				if (Exists(profilesJsonPath))
				{
					loaderDataPath = appDataLocalPath + L"\\SAManager\\";
				}
				// If it still can't be found, display an error message
				else
					DisplaySettingsLoadError(gamePath + L"\\mods\\.modloader\\Profiles.json");
			}
			else
			{
				MessageBox(nullptr, L"Unable to retrieve local AppData path.", L"SA2 Mod Loader", MB_ICONERROR);
				OnExit(0, 0, 0);
				ExitProcess(0);
			}
		}
		// If Profiles.json was found, set old paths
		loaderSettings->ExtLibPath = loaderDataPath + L"extlib\\";
		profilesFolderPath = loaderDataPath + L"SA2\\";
	}

	// Load profiles JSON file
	std::ifstream ifs(profilesJsonPath);
	json json_profiles = json::parse(ifs);
	ifs.close();

	// Get current profile index
	int ind_profile = json_profiles.value("ProfileIndex", 0);

	// Get current profile filename
	json proflist = json_profiles["ProfilesList"];
	std::string profname = proflist.at(ind_profile)["Filename"];

	// Convert profile name from UTF8 stored in JSON to wide string
	int count = MultiByteToWideChar(CP_UTF8, 0, profname.c_str(), profname.length(), NULL, 0);
	std::wstring profname_w(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, profname.c_str(), profname.length(), &profname_w[0], count);

	// Load the current profile
	currentProfilePath = profilesFolderPath + profname_w;
	if (!Exists(currentProfilePath))
	{
		DisplaySettingsLoadError(currentProfilePath);
	}


	std::ifstream ifs_p(currentProfilePath);
	json json_config = json::parse(ifs_p);
	int settingsVersion = json_config.value("SettingsVersion", 0);

	// Graphics settings
	json json_graphics = json_config["Graphics"];
	const uint8_t BFOWTF = json_graphics.value("ScreenMode", 1);
	loaderSettings->ScreenMode = BFOWTF;
	loaderSettings->ScreenNum = json_graphics.value("SelectedScreen", 0);
	loaderSettings->HorizontalResolution = json_graphics.value("HorizontalResolution", 640);
	loaderSettings->VerticalResolution = json_graphics.value("VerticalResolution", 480);
	loaderSettings->PauseWhenInactive = json_graphics.value("EnablePauseOnInactive", true);
	loaderSettings->CustomWindowSize = json_graphics.value("EnableCustomWindow", false);
	loaderSettings->WindowWidth = json_graphics.value("CustomWindowWidth", 640);
	loaderSettings->WindowHeight = json_graphics.value("CustomWindowHeight", 480);
	loaderSettings->ResizableWindow = json_graphics.value("EnableResizableWindow", true);
	loaderSettings->StretchToWindow = json_graphics.value("StretchToWindow", false);
	loaderSettings->MaintainAspectRatio = loaderSettings->KeepAspectWhenResizing = !loaderSettings->StretchToWindow;
	loaderSettings->SkipIntro = json_graphics.value("SkipIntro", false);
	loaderSettings->DisableBorderImage = json_graphics.value("DisableBorderImage", false);

	// Game Patches settings (for compatibility)
	if (json_config.contains("Patches"))
	{
		json json_oldpatches = json_config["Patches"];
		loaderSettings->FramerateLimiter = json_oldpatches.value("FramerateLimiter", true);
		loaderSettings->DisableExitPrompt = json_oldpatches.value("DisableExitPrompt", true);
		loaderSettings->SyncLoad = json_oldpatches.value("SyncLoad", true);
		loaderSettings->ExtendVertexBuffer = json_oldpatches.value("ExtendVertexBuffer", true);
		loaderSettings->EnvMapFix = json_oldpatches.value("EnvMapFix", true);
		loaderSettings->ScreenFadeFix = json_oldpatches.value("ScreenFadeFix", true);
		loaderSettings->CECarFix = json_oldpatches.value("CECarFix", true);
		loaderSettings->ParticlesFix = json_oldpatches.value("ParticlesFix", true);
		// Add old game patches to the new system
		for (int p = 0; p < LengthOfArray(LegacyGamePatchList); p++)
		{
			if (json_oldpatches.value(LegacyGamePatchList[p], false)) // Old game patches are stored in the json regardless of whether they are enabled or not
			{
				GamePatchList.push_back(LegacyGamePatchList[p]);
			}
		}
	}

	// Debug settings
	json json_debug = json_config["DebugSettings"];
	loaderSettings->DebugConsole = json_debug.value("EnableDebugConsole", false);
	loaderSettings->DebugScreen = json_debug.value("EnableDebugScreen", false);
	loaderSettings->DebugFile = json_debug.value("EnableDebugFile", false);
	loaderSettings->DebugCrashLog = json_debug.value("EnableDebugCrashLog", true);

	// Testspawn settings 
	json json_testspawn = json_config["TestSpawn"];
	loaderSettings->TextLanguage = json_testspawn.value("GameTextLanguage", 1);
	loaderSettings->VoiceLanguage = json_testspawn.value("GameVoiceLanguage", 1);

	// Game Patches (current system)
	if (json_config.contains("EnabledGamePatches"))
	{
		json json_patches = json_config["EnabledGamePatches"];
		for (unsigned int i = 1; i <= json_patches.size(); i++)
		{
			std::string patch_name = json_patches.at(i - 1);
			// Check if it isn't on the list already (legacy patches can be there)
			if (std::find(std::begin(GamePatchList), std::end(GamePatchList), patch_name) == std::end(GamePatchList));
			GamePatchList.push_back(patch_name);
		}

		loaderSettings->FramerateLimiter = IsGamePatchEnabled("FramerateLimiter");
		loaderSettings->DisableExitPrompt = IsGamePatchEnabled("DisableExitPrompt");
	}

	// Mods
	json json_mods = json_config["EnabledMods"];
	for (unsigned int i = 1; i <= json_mods.size(); i++)
	{
		std::string mod_fname = json_mods.at(i - 1);
		ModList.push_back(mod_fname);
	}
}