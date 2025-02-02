#include "stdafx.h"
#include <dbghelp.h> 
#include <windows.h>
#include <direct.h>
#include <Psapi.h>
#include <shlwapi.h>
#include <time.h>
#include "config.h"
#include <filesystem>  // For std::filesystem on C++17 and above
#include <iostream>
#include <iomanip>
#include <ShlObj.h>
#include <shellapi.h>
#include <locale>
#include <sstream>
#include <ctime>

extern bool useTestSpawn;

using namespace std;
extern HelperFunctions helperFunctions;
const string eyeTrackCrash = "The game failed to load the Eyes tracker for the character. If you are making a skin mod, this usually means the hierarchy of your character is wrong.\n\nAlso make sure to NOT rename or delete the bones.";
const string jiggleCrash = "The game failed to load the Jiggle function of the character (quills, tail physics etc.).\nIf you are making a skin mod, you should disable the jiggle for this character using a DLL mod.";

static const unordered_map<intptr_t, string> crashes_addresses_map = {
	{ 0x42C304, "Animation error: the game failed to play one or more animations."  },
	{ 0x42C4F1, "Texture error: the game failed to apply one or more textures."  },
	{ 0x459851, "Animation error: the game failed to load the motion file of a character."  },
	{ 0x46A1CA, "Animation error: the game failed to play a character animation."  },
	{ 0x42EF3A, "Draw Model error: the game failed to draw a model.\nIf you are making a character mod, this might be the jiggle.\nDisable the jiggle or make the head mesh 100% weighted to the head bone."  },
};

struct addressRange
{
	intptr_t start = 0;
	intptr_t end = 0;
	string crashMsg = "";
};

static const addressRange Range_Addresses_list[] =
{
	{ 0x476530, 0x476692, eyeTrackCrash },
	{ 0x447580, 0x447907, jiggleCrash} ,
};

static const string GetRangeAddressesCrash(const intptr_t address)
{
	for (uint8_t i = 0; i < LengthOfArray(Range_Addresses_list); i++)
	{
		if (address >= Range_Addresses_list[i].start && address <= Range_Addresses_list[i].end)
		{
			return Range_Addresses_list[i].crashMsg;
		}
	}

	return "NULL";
}

static const string getErrorMSG(intptr_t address)
{
	if ((crashes_addresses_map.find(address) == crashes_addresses_map.end()))
	{
		return "NULL";
	}

	return crashes_addresses_map.find(address)->second; //return a custom error message if the address is known
}

static const unordered_map<uint8_t, wstring> char_map
{
	{ 0, L"Sonic"},
	{ 1, L"Shadow"},
	{ 2, L"Tails"},
	{ 3, L"Eggman"},
	{ 4, L"Knuckles"},
	{ 5, L"Rouge"},
	{ 6, L"MechTails"},
	{ 7, L"MechEggman"},
	{ 8, L"Amy"},
	{ 9, L"SuperSonic"},
	{ 10, L"SuperShadow"},
	{ 12, L"Metal"},
	{ 13, L"ChaoWalker"},
	{ 14, L"DarkChaoWalker"},
	{ 15, L"Tikal"},
	{ 16, L"Chaos"},
};

//todo all all levels
static wstring GetLevelName()
{
	if (GameState != 0)
	{
		return L"lvl" + std::to_wstring(CurrentLevel);
	}

	return L"";
}

static wstring GetCharName()
{
	if (GameState > 0)
	{
		uint8_t charID = MainCharObj2[0] ? MainCharObj2[0]->CharID2 : CurrentCharacter;
		auto it = char_map.find(charID);

		if (it != char_map.cend())
			return it->second;
		else
			return L"Unknown";
	}

	return L"";
}

static void SetErrorMessage(string& fullMsg, const string address, const string dllName, const intptr_t crashID, const string what)
{
	if (crashID == -1 && address == "")
	{
		fullMsg = "SA2 has crashed, but the crash information couldn't be obtained.\n\n";
		fullMsg += "If you still want to report this crash, please share your mod list and the 'extra_info' text file from your game's CrashDumps folder.\n";
		if (what != "")
			fullMsg += "Advanced Error:\n" + what;

		return;
	}

	string errorCommon = getErrorMSG(crashID); //get error message if the crash address is common
	fullMsg = "SA2 has crashed at " + address + " (" + dllName + ").\n";

	if (errorCommon != "NULL")
	{
		fullMsg += "\n" + errorCommon + "\n"; //add the common error message if it exists
	}
	else
	{
		//if the crash isn't in the list, check if it's a common crash from addresses from a whole function...
		auto charcrash = GetRangeAddressesCrash(crashID);

		if (charcrash != "NULL")
		{
			fullMsg += "\n" + charcrash + "\n";
		}
	}

	if (dllName.find("Data_DLL") != std::string::npos)
		fullMsg += "\nThis is a crash in the Mod Loader, if you are making a mod, there is a chance that you did something very wrong.";

	fullMsg += "\nA crash dump, a mod list and a text file have been added to your game's CrashDumps folder.\n\nIf you want to report this crash, please include the dump (.dmp file), the mod list (.json file) and the info (.txt file) in your report.\n";
}

//extra info
static void CreateExtraInfoFile(wstring curCrashFolder, wstring timeStr, string fullMsg)
{
	std::wstring chara = GetCharName();
	std::wstring lvl = GetLevelName();

	std::wstring filename = timeStr + L"_extra_info_" + chara + L"_" + lvl + L".txt";
	std::filesystem::path directory = curCrashFolder;
	std::filesystem::path destinationPath = directory / filename;

	// Create and open the file
	std::ofstream outfile(destinationPath);

	if (!outfile)
		return;

	outfile << "Mod Loader API Version: " << ModLoaderVer << std::endl;
	outfile << "Use TestSpawn: " << (useTestSpawn ? "Yes" : "No") << std::endl;
	outfile << "\n" << std::endl;
	outfile << "[Full Message]\n" << fullMsg << "\n" << std::endl;
	// Write information to the file
	outfile << "[Mods]" << std::endl;

	auto mod = helperFunctions.Mods;

	if (mod)
	{
		for (uint16_t i = 0; i < mod->size(); i++)
			outfile << mod->at(i).Name << " - " << mod->at(i).Version << std::endl;
	}

	outfile << "\n" << std::endl;
	outfile << "[Game Values]" << std::endl;
	outfile << "GameMode: " << GameMode << std::endl;
	outfile << "GameState: " << GameState << std::endl;
	outfile << "Current Level: " << CurrentLevel << std::endl;
	outfile << "Last Level: " << LastLevel << std::endl;
	outfile << "Current Character: " << CurrentCharacter << std::endl;
	outfile << "\n" << std::endl;

	if (playertwp[0])
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			auto p = MainCharObj1[i];
			auto pwp = MainCharObj2[i];
			if (p)
			{
				if (i > 0)
					outfile << "\n" << std::endl;
				else
					outfile << "[Players]" << std::endl;


				outfile << "Action: " << static_cast<int>(p->Action) << std::endl;
				outfile << "Next Action: " << static_cast<int>(p->NextAction) << std::endl;
				outfile << "Position: " << p->Position.x << ", " << p->Position.y << ", " << p->Position.z << std::endl;
				outfile << "Rotation: " << p->Rotation.x << ", " << p->Rotation.y << ", " << p->Rotation.z << std::endl;
				outfile << "Is on Ground: " << ((p->Status & 3) ? "Yes" : "No") << std::endl;
				outfile << "Is Holding Obj: " << ((p->Status & Status_HoldObject) ? "Yes" : "No") << std::endl;
				outfile << "Is on Path: " << ((p->Status & Status_OnPath) ? "Yes" : "No") << std::endl;
			}

			if (pwp)
			{
				outfile << "Player ID: " << static_cast<int>(pwp->PlayerNum) << std::endl;
				outfile << "Char ID: " << static_cast<int>(pwp->CharID) << std::endl;
				outfile << "Char ID2: " << static_cast<int>(pwp->CharID2) << std::endl;
				outfile << "Is in Super Form: " << ((pwp->Upgrades & Upgrades_SuperSonic) ? "Yes" : "No") << std::endl;
				outfile << "Anim: " << pwp->AnimInfo.Current << std::endl;
				outfile << "Frame: " << pwp->AnimInfo.nframe << std::endl;
				outfile << "Next Anim: " << pwp->AnimInfo.Next << std::endl;
			}
			else
			{
				if (i == 0)
					outfile << "playerpwp pointer was null.";
			}
		}
	}
	else
	{
		outfile << "playertwp pointer was null.";
	}


	outfile << "\n" << std::endl;


	// Close the file
	outfile.close();
}

void CopyAndRename_SA2LoaderProfile(std::wstring crashDumpFolder, std::wstring timeStr)
{
	std::filesystem::path directory = crashDumpFolder;
	std::filesystem::path sourcePath = currentProfilePath;

	std::wstring chara = GetCharName();
	std::wstring lvl = GetLevelName();
	std::wstring fileName = timeStr + L"_ModList_" + chara + L"_" + lvl + L".json";

	std::filesystem::path destinationPath = directory / (fileName);

	try
	{
		std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);
		PrintDebug("CrashDump: Successfully copied and renamed SA2 Profile.\n");
	}
	catch (const std::exception& e) {
		PrintDebug("CrashDump: Failed to copy and rename SA2 Profile. Error: %s\n", e.what());
	}
}

static std::wstring getCurrentDate() 
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm* ptm = std::localtime(&now_c);

	// Format date with '_' separators
	std::wostringstream ss;
	ss << std::setfill(L'0') << std::setw(2) << ptm->tm_mday << L"_"
		<< std::setfill(L'0') << std::setw(2) << (ptm->tm_mon + 1) << L"_"
		<< (ptm->tm_year + 1900);

	return ss.str();
}

static std::wstring getCurrentTime() 
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm* ptm = std::localtime(&now_c);

	// Format time with '_' separators
	std::wostringstream ss;
	ss << std::setfill(L'0') << std::setw(2) << ptm->tm_hour << L"_"
		<< std::setfill(L'0') << std::setw(2) << ptm->tm_min << L"_"
		<< std::setfill(L'0') << std::setw(2) << ptm->tm_sec;

	return ss.str();
}

#pragma comment(lib, "dbghelp.lib") 
#pragma comment(lib, "Psapi.lib")
LONG WINAPI HandleException(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	std::wstring dateStr = getCurrentDate();
	std::wstring timeStr = getCurrentTime();

	wstring curCrashDumpFolder = L"CrashDump\\" + dateStr;

	std::filesystem::create_directories(curCrashDumpFolder);

	std::wstring chara = GetCharName();
	std::wstring lvl = GetLevelName();
	std::wstring fileName = timeStr + L"_" + L"Dump_" + chara + L"_" + lvl + L".dmp";

	wstring crashDumpFile = curCrashDumpFolder + L"\\" + fileName;

	MINIDUMP_EXCEPTION_INFORMATION info = { NULL, NULL, NULL };
	string exception = "";
	try
	{
		//generate crash dump
		HANDLE hFile = CreateFileW(
			crashDumpFile.c_str(),
			GENERIC_WRITE | GENERIC_READ,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL
		);

		HANDLE hProcess = GetCurrentProcess();
		PrintDebug("SA2 HAS CRASHED!\n");

		if (hFile != NULL)
		{
			PrintDebug("Generating Crash Dump file...\n");
			info =
			{
			 GetCurrentThreadId(),
			 apExceptionInfo,
			 TRUE
			};

			MiniDumpWriteDump(hProcess, GetCurrentProcessId(),
				hFile, MiniDumpWithIndirectlyReferencedMemory,
				&info, NULL, NULL
			);

			CloseHandle(hFile);

			PrintDebug("Done.\n");
		}
	}
	catch (const std::exception& e)
	{
		PrintDebug("CrashDump: Failed to generate a Crash Dump. Error: %s\n", e.what());
		exception = e.what();
	}

	intptr_t crashID = -1;
	string address = "";
	string dllName = "";

	if (info.ThreadId != NULL) 		//get crash address
		crashID = (intptr_t)info.ExceptionPointers->ExceptionRecord->ExceptionAddress;

	if (crashID != -1)
	{
		char hex[MAX_PATH];
		sprintf_s(hex, "%x", crashID);
		address = hex;

		PrintDebug("Get fault module name...\n");

		HMODULE handle;

		if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)crashID, &handle))
		{
			char path[MAX_PATH];
			if (GetModuleFileNameA(handle, path, MAX_PATH))
			{
				auto filename = PathFindFileNameA(path);
				dllName = filename;
			}
		}
	}

	string fullMsg = "";
	SetErrorMessage(fullMsg, address, dllName, crashID, exception);
	CopyAndRename_SA2LoaderProfile(curCrashDumpFolder, timeStr); //copy JSON Profile file to the Crash Dump folder so we know what mods and cheats were used
	CreateExtraInfoFile(curCrashDumpFolder, timeStr, fullMsg); //add extra game info like variable states
	string text = "Crash Address: " + address + "\n";
	PrintDebug("\nFault module name: %s \n", dllName.c_str());
	PrintDebug(text.c_str());

	PrintDebug("Crash Dump Done.\n");
	MessageBoxA(MainWindowHandle, fullMsg.c_str(), "SA2 Error", MB_ICONERROR);
	ShellExecute(NULL, L"open", curCrashDumpFolder.c_str(), NULL, NULL, SW_SHOW);

	return EXCEPTION_EXECUTE_HANDLER;
}


void initCrashDump()
{
	SetUnhandledExceptionFilter(HandleException);
}