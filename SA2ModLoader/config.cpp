#include "stdafx.h"
#include "json.hpp"
#include "IniFile.hpp"
#include "FileSystem.h"

using json = nlohmann::json;
using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;

wstring currentProfilePath; // Used for crash dumps
vector<std::string> ModList;

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

void LoadModLoaderSettings(LoaderSettings* loaderSettings, std::wstring appPath)
{

	std::wstring profilesPath = appPath + L"\\SA2\\Profiles.json";
	// If the profiles path exists, assume SA Mod Manager
	if (Exists(profilesPath))
	{
		// Load profiles JSON file
		std::ifstream ifs(profilesPath);
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
		currentProfilePath = appPath + L"\\SA2\\" + profname_w;
		std::ifstream ifs_p(appPath + L"\\SA2\\" + profname_w);
		json json_config = json::parse(ifs_p);
		int settingsVersion = json_config.value("SettingsVersion", 0);

		// Graphics settings
		json json_graphics = json_config["Graphics"];
		loaderSettings->ScreenNum = json_graphics.value("SelectedScreen", 0);
		loaderSettings->BorderlessWindow = json_graphics.value("BorderlessWindow", false);
		loaderSettings->FullScreen = json_graphics.value("FullScreen", true);
		loaderSettings->HorizontalResolution = json_graphics.value("HorizontalResolution", 640);
		loaderSettings->VerticalResolution = json_graphics.value("VerticalResolution", 480);
		loaderSettings->PauseWhenInactive = json_graphics.value("EnablePauseOnInactive", true);
		loaderSettings->CustomWindowSize = json_graphics.value("EnableCustomWindow", false);
		loaderSettings->WindowWidth = json_graphics.value("CustomWindowWidth", 640);
		loaderSettings->WindowHeight = json_graphics.value("CustomWindowHeight", 480);
		loaderSettings->ResizableWindow = json_graphics.value("EnableResizableWindow", true);
		loaderSettings->MaintainAspectRatio = json_graphics.value("MaintainAspectRatio", false);

		// Sound settings
		json json_sound = json_config["Sound"];
		loaderSettings->EnableBass = json_sound.value("EnableBassMusic", true);

		// Patches settings
		json json_patches = json_config["Patches"];
		loaderSettings->FramerateLimiter = json_sound.value("FramerateLimiter", true);

		// Debug settings
		json json_debug = json_config["DebugSettings"];
		loaderSettings->DebugConsole = json_debug.value("EnableDebugConsole", false);
		loaderSettings->DebugScreen = json_debug.value("EnableDebugScreen", false);
		loaderSettings->DebugFile = json_debug.value("EnableDebugFile", false);
		loaderSettings->DebugCrashLog = json_debug.value("EnableDebugCrashLog", true);

		// Testspawn settings (These don't really belong in testspawn imo - PkR)
		json json_testspawn = json_config["TestSpawn"];
		loaderSettings->TextLanguage = json_testspawn.value("GameTextLanguage", 1);
		loaderSettings->VoiceLanguage = json_testspawn.value("GameVoiceLanguage", 1);


		// Mods
		json json_mods = json_config["EnabledMods"];
		for (unsigned int i = 1; i <= json_mods.size(); i++)
		{
			std::string mod_fname = json_mods.at(i - 1);
			ModList.push_back(mod_fname);
		}
	}
	else
	{

		FILE* f_ini = _wfopen(L"mods\\SA2ModLoader.ini", L"r");
		if (!f_ini)
		{
			MessageBox(nullptr, L"Mod Loader settings could not be read. Please run the Mod Manager, save settings and try again.", L"SA2 Mod Loader", MB_ICONWARNING);
			return;
		}
		unique_ptr<IniFile> ini(new IniFile(f_ini));
		fclose(f_ini);

		const IniGroup* setgrp = ini->getGroup("");

		loaderSettings->DebugConsole = setgrp->getBool("DebugConsole");
		loaderSettings->DebugScreen = setgrp->getBool("DebugScreen");
		loaderSettings->DebugFile = setgrp->getBool("DebugFile");
		loaderSettings->DebugCrashLog = setgrp->getBool("DebugCrashLog", true);
		loaderSettings->PauseWhenInactive = setgrp->getBool("PauseWhenInactive", true);
		loaderSettings->DisableExitPrompt = setgrp->getBool("DisableExitPrompt");
		loaderSettings->ScreenNum = setgrp->getInt("ScreenNum", 1);
		loaderSettings->BorderlessWindow = setgrp->getBool("BorderlessWindow");
		loaderSettings->FullScreen = setgrp->getBool("FullScreen");
		loaderSettings->SkipIntro = setgrp->getBool("SkipIntro");
		loaderSettings->SyncLoad = setgrp->getBool("SyncLoad", true);
		loaderSettings->HorizontalResolution = setgrp->getInt("HorizontalResolution", 640);
		loaderSettings->VerticalResolution = setgrp->getInt("VerticalResolution", 480);
		loaderSettings->VoiceLanguage = setgrp->getInt("VoiceLanguage", 1);
		loaderSettings->TextLanguage = inilangs[setgrp->getInt("TextLanguage", 0)];
		loaderSettings->CustomWindowSize = setgrp->getBool("CustomWindowSize");
		loaderSettings->WindowWidth = setgrp->getInt("WindowWidth", 640);
		loaderSettings->WindowHeight = setgrp->getInt("WindowHeight", 480);
		loaderSettings->ResizableWindow = setgrp->getBool("ResizableWindow");
		loaderSettings->MaintainAspectRatio = setgrp->getBool("MaintainAspectRatio");
		loaderSettings->FramerateLimiter = setgrp->getBool("FramerateLimiter");
		loaderSettings->TestSpawnLevel = setgrp->getInt("TestSpawnLevel");
		loaderSettings->TestSpawnCharacter = setgrp->getInt("TestSpawnCharacter");
		loaderSettings->TestSpawnPositionEnabled = setgrp->getBool("TestSpawnPositionEnabled");
		loaderSettings->TestSpawnX = setgrp->getInt("TestSpawnX");
		loaderSettings->TestSpawnY = setgrp->getInt("TestSpawnY");
		loaderSettings->TestSpawnZ = setgrp->getInt("TestSpawnZ");
		loaderSettings->TestSpawnRotation = setgrp->getInt("TestSpawnRotation");
		loaderSettings->TestSpawnEvent = setgrp->getInt("TestSpawnEvent");
		loaderSettings->TestSpawnSaveID = setgrp->getInt("TestSpawnSaveID");
		loaderSettings->EnableBass = setgrp->getBool("EnableBass", true);

		for (unsigned int i = 1; i <= 999; i++)
		{
			char key[8];
			snprintf(key, sizeof(key), "Mod%u", i);
			if (!setgrp->hasKey(key))
				break;
			ModList.push_back(setgrp->getString(key));
		}
	}
}