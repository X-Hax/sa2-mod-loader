#include "stdafx.h"
#include <dbghelp.h> 
#include <windows.h>
#include <direct.h>
#include <Psapi.h>
#include <map>

using namespace std;

static const unordered_map<intptr_t, string> crashes_addresses_map = {
	{ 0x42C304, "Animation error: the game failed to play one or more animations."  },
	{ 0x42C4F1, "Texture error: the game failed to apply one or more textures."  },
	{ 0x459851, "Animation error: the game failed to load the motion file of a character."  },
	{ 0x46A1CA, "Animation error: the game failed to play a character animation."  },
	{ 0x42EF3A, "Draw Model error: the game failed to draw a model.\nIf you are making a character mod, this might be the jiggle.\nDisable the jiggle or make the head mesh 100% weighted to the head bone."  },
};

static const string getErrorMSG(intptr_t address)
{
	if ((crashes_addresses_map.find(address) == crashes_addresses_map.end()))
	{
		return "NULL";
	}

	return crashes_addresses_map.find(address)->second; //return a custom error message if the address is known
}

void CopyAndRename_ModLoaderIni()
{
	char timeStr[255];
	time_t t = time(NULL);
	tm tM;
	localtime_s(&tM, &t);
	strftime(timeStr, 255, "_%d_%m_%Y_%H_%M_%S", &tM);
	char tmp[256];
	string directory = getcwd(tmp, 256);

	const string quote = "\"";
	string fullLine = "xcopy " + quote + directory + "\\mods\\SA2ModLoader.ini" + quote + " " + quote + directory + "\\CrashDump" + quote;
	int copyState = system(fullLine.c_str());

	if (copyState != -1) {
		std::string rename = "ren " + quote + directory + "\\CrashDump\\SA2ModLoader.ini" + quote + " " + quote + "ModList" + timeStr + ".ini" + quote;
		system(rename.c_str());
		PrintDebug("CrashDump: Successfully copied SA2ModLoader.ini to the CrashDump Folder.\n");
	}
	else
	{
		PrintDebug("CrashDump: Failed to copy SA2ModLoader.ini to the Crash Dump Folder.\n");
	}
}

bool IsPathExist(const std::string& s)
{
	struct stat buffer;
	return (stat(s.c_str(), &buffer) == 0);
}

#pragma comment(lib, "dbghelp.lib") 
#pragma comment(lib, "Psapi.lib")
LONG WINAPI HandleException(struct _EXCEPTION_POINTERS* apExceptionInfo)
{
	char timeStr[255];
	time_t t = time(NULL);
	tm tM;
	localtime_s(&tM, &t);
	strftime(timeStr, 255, "CrashDump_%d_%m_%Y_%H_%M_%S.dmp", &tM);
	std::string s = "CrashDump\\";
	s.append(timeStr);

	const char* crashFolder = "CrashDump";

	if (!IsPathExist(crashFolder))
	{
		_mkdir(crashFolder);
	}

	//generate crash dump
	HANDLE hFile = CreateFileA(
		s.c_str(),
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
		MINIDUMP_EXCEPTION_INFORMATION info =
		{
		 GetCurrentThreadId(),
		 apExceptionInfo,
		 TRUE
		};

		MiniDumpWriteDump(
			hProcess,
			GetCurrentProcessId(),
			hFile,
			MiniDumpWithIndirectlyReferencedMemory,
			&info,
			NULL,
			NULL
		);

		CloseHandle(hFile);

		PrintDebug("Done.\n");

		PrintDebug("Get modules name...\n");
		map<intptr_t, string> dllMap;

		HMODULE hMods[1024];
		DWORD cbNeeded;

		//browse process to get all the modules
		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				TCHAR szModName[MAX_PATH];

				// Get the full path to the module's file.
				if (GetModuleBaseName(hProcess, hMods[i], szModName,
					sizeof(szModName) / sizeof(TCHAR)))
				{
					MODULEINFO moduleInfo;

					if (GetModuleInformation(hProcess, hMods[i], &moduleInfo, sizeof(moduleInfo))) {

						intptr_t address = (intptr_t)moduleInfo.lpBaseOfDll;

						if (address > 0) { //store module address and name in the map
							wstring getModName(&szModName[0]);
							string modName(getModName.begin(), getModName.end());
							dllMap.insert({ address, modName });
						}
					}
				}
			}
		}

		//sort map by address
		for (auto const& entry : dllMap)
		{
			std::cout << entry.second << " -> (" << entry.first << " )" << '\n';
		}

		//get crash address
		intptr_t crashID = (intptr_t)info.ExceptionPointers->ExceptionRecord->ExceptionAddress;
		char hex[MAX_PATH];
		sprintf_s(hex, "%x", crashID);
		string address = hex;

		string dllName;

		//Browse the map to and do a comparison with the crash address to get the fault module name.
		for (auto itr = dllMap.begin(); itr != dllMap.end(); itr++)
		{
			//the last module whose address is less than the crash address is the correct one.
			if (itr->first < crashID)
			{
				dllName = itr->second;
			}
		}

		string errorCommon = getErrorMSG(crashID); //get error message if the crash address is common
		string fullMsg = "SA2 has crashed at " + address + " (" + dllName + ").\n";

		if (errorCommon != "NULL") {
			fullMsg += errorCommon + "\n"; //add the common error message if it exists
		}

		fullMsg += "A minidump has been created in your SA2 folder.\n";
		CopyAndRename_ModLoaderIni(); //copy ModLoaderIni file to the Crash Dump folder so we know what mod and cheat were used
		std::string text = "Crash Address: " + address + "\n";
		PrintDebug("\nFault module name: %s \n", dllName.c_str());
		PrintDebug(text.c_str());

		PrintDebug("Crash Dump Done.\n");
		MessageBoxA(0, fullMsg.c_str(), "SA2 ERROR", MB_ICONERROR);

	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void CheckCrashMod()
{
	HMODULE Mod = GetModuleHandle(L"CrashMod");

	if (Mod)
	{
		MessageBoxA(0, "SA2 Crash Mod has been detected, this mod is now deprecated.\nPlease uninstall Crash Mod.", "Potential Conflict", MB_ICONWARNING);
	}
}

void initCrashDump()
{
	SetUnhandledExceptionFilter(HandleException);
}