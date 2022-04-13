#include "stdafx.h"
#include <dbghelp.h> 
#include <windows.h>
#include <direct.h>

//this uses decimal, convert your hex address to decimal if you want to add more crashes.
static const std::unordered_map<intptr_t, std::string> crashes_addresses_map = {
	{ 4375300, "Animation error: the game failed to play one or more animations."  },	
	{ 4375793, "Texture error: the game failed to apply one or more textures."  },	
	{ 4560977, "Animation error: the game failed to load the motion file of a character."  },
	{ 4628938, "Animation error: the game failed to play a character animation."  },	
	{ 4386618, "Draw Model error: the game failed to draw a model.\nIf you are making a character mod, this might be the jiggle.\nDisable the jiggle or make the head mesh 100% weighted to the head bone."  },
};

static const std::string getErrorMSG(intptr_t address)
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
	std::string directory = getcwd(tmp, 256);

	const std::string quote = "\"";
	std::string fullLine = "xcopy " + quote + directory + "\\mods\\SA2ModLoader.ini" + quote + " " + quote + directory + "\\CrashDump" + quote;
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

		intptr_t crashID = (intptr_t)info.ExceptionPointers->ExceptionRecord->ExceptionAddress;
		char hex[30];
		sprintf_s(hex, "%x", crashID);

		std::string address = hex;
		std::string errorCommon = getErrorMSG(crashID); //get error message if the crash address is common
		std::string fullMsg = "SA2 has crashed at " + address + ".\n";

		if (errorCommon != "NULL") {
			fullMsg += errorCommon + "\n"; //add the common error message if it exists
		}

		fullMsg += "A minidump has been created in your SA2 folder.\n";

		PrintDebug("Done.\n");
		CopyAndRename_ModLoaderIni(); //copy ModLoaderIni file to the Crash Dump folder so we know what mod and cheat were used
		std::string text = "Crash Address: " + address + "\n";
		PrintDebug(text.c_str());
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