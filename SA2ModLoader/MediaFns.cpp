/**
 * SA2 Mod Loader
 * Media functions.
 */

#include "stdafx.h"
#include "MediaFns.hpp"
#include "FileSystem.h"

#include "bass_vgmstream.h"

#include <string>
#include <vector>
#include "UsercallFunctionHandler.h"
using std::string;
using std::vector;

static  bool bassinit = false;
static DWORD basschan = 0;
static DWORD voicechan = 0;
/**
 * Initialize media playback.
 */

void BassInit_r()
{
	bassinit = !!BASS_Init(-1, 44100, 0, nullptr, nullptr);
}

static void __stdcall onVoiceEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);
}

const bool (*sub_4430B0)(signed int a1, signed int a2) = GenerateUsercallWrapper<decltype(sub_4430B0)>(rEAX, 0x4430B0, rEAX, rEDX);
DataPointer(void**, dword_1A55998, 0x1A55998);
DataPointer(int, dword_1A5599C, 0x1A5599C);
signed int PlayVoice_r(int idk, int num)
{
	int v3; // edi
	signed int v4; // esi
	int v5; // eax

	if (!VoicesEnabled)
	{
		return -1;
	}

	if (bassinit)
	{
		char path[MAX_PATH];
		if (!VoiceLanguage)
			sprintf_s(path, "resource\\gd_pc\\event_adx\\%04d.ahx", num);
		else
			sprintf_s(path, "resource\\gd_pc\\event_adx_e\\%04d.ahx", num);

		const char* filename = sadx_fileMap.replaceFile(path);
		if (FileExists(filename))
		{
			voicechan = BASS_VGMSTREAM_StreamCreate(filename, 0);

			if (voicechan == 0)
			{
				voicechan = BASS_StreamCreateFile(false, filename, 0, 0, 0);
			}

			if (voicechan != 0)
			{
				BASS_ChannelPlay(voicechan, false);
				BASS_ChannelSetSync(voicechan, BASS_SYNC_END, 0, onVoiceEnd, nullptr);
				return 1;
			}
		}
	}

	v4 = dword_1A5599C;
	v3 = 0;
	while (1)
	{
		if (++v4 >= 2)
		{
			v4 = 0;
		}
		if (sub_4430B0(v4, (unsigned __int8)idk))
		{
			break;
		}
		if (++v3 >= 3)
		{
			dword_1A5599C = v4;
			return -1;
		}
	}
	v5 = (int)&dword_1A55998[7 * v4];
	*(_DWORD*)(v5 + 40) = num;
	*(char*)(v5 + 36) = 1;
	*(char*)(v5 + 37) = idk;
	dword_1A5599C = v4;
	return v4;
}

void* ReleaseSoundEffects_r() {
	BASS_Free();
	return sub_437E90();
}


extern std::wstring extLibPath;
void Init_AudioBassHook()
{
	std::wstring bassFolder = extLibPath + L"BASS\\";

	// If the file doesn't exist, assume it's in the game folder like with the old Manager
	if (!FileExists(bassFolder + L"bass_vgmstream.dll"))
		bassFolder = L"";

	bool bassDLL = false;


	std::wstring fullPath = bassFolder + L"bass_vgmstream.dll";

	bassDLL = LoadLibraryEx(fullPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

	if (bassDLL)
	{
		PrintDebug("Loaded Bass DLLs dependencies\n");
	}
	else
	{
		PrintDebug("Failed to load bass DLL dependencies\n");
		MessageBox(MainWindowHandle, L"Error loading BASS.\n\n"
			L"Make sure the Mod Loader is installed properly.",
			L"BASS Load Error", MB_OK | MB_ICONERROR);
		return;
	}

	WriteCall((void*)0x435511, ReleaseSoundEffects_r);
	BassInit_r();
	GenerateUsercallHook(PlayVoice_r, rEAX, (intptr_t)PlayVoicePtr, rEDX, stack4);
	return;
}