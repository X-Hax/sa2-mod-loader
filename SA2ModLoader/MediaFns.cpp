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

#pragma region "BASS General"

static HSTREAM voicechan[3];

static Bool bassinit = FALSE;

/**
 * Initialize media playback.
 */
void BassInit_r()
{
	bassinit = !!BASS_Init(-1, 44100, 0, nullptr, nullptr);
}

void __stdcall onVoiceEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
	BASS_ChannelStop(channel);
	BASS_StreamFree(channel);

	for (int i = 0; i < 3; ++i)
	{
		if (voicechan[i] == channel)
		{
			voicechan[i] = NULL;
		}
	}
}

HSTREAM s2mlGetAudioFile(const char* filename, DWORD flags)
{
	HSTREAM stream;

	stream = BASS_VGMSTREAM_StreamCreate(filename, flags);

	if (!stream) stream = BASS_StreamCreateFile(false, filename, 0, 0, flags);

	return stream;
}

void* ReleaseSoundEffects_r()
{
	bassinit = !BASS_Free();

	return sub_437E90();
}

#pragma endregion

#pragma region "Serif Replacement System"

const bool (*sub_4430B0)(signed int a1, signed int a2) = GenerateUsercallWrapper<decltype(sub_4430B0)>(rEAX, 0x4430B0, rEAX, rEDX);
DataPointer(void**, dword_1A55998, 0x1A55998);
DataPointer(int, VoiceCount, 0x1A5599C);

DataPointer(Sint32, VoiceVolume, 0x01A55990);

signed int PlayVoice_r(int idk, int num)
{
	if (!VoicesEnabled)
	{
		return -1;
	}

	for (int i = 0; i < 3; ++i)
	{
		if (++VoiceCount >= 2)
			VoiceCount = 0;

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
				voicechan[VoiceCount] = s2mlGetAudioFile(filename, NULL);

				if (voicechan[VoiceCount])
				{
					BASS_ChannelStop(voicechan[VoiceCount]);

					BASS_ChannelPlay(voicechan[VoiceCount], true);

					BASS_ChannelSetSync(voicechan[VoiceCount], BASS_SYNC_END, 0, onVoiceEnd, nullptr);
					return VoiceCount;
				}
			}
		}

		// if audio doesn't exist in replacement folder, run the original logic
		if (sub_4430B0(VoiceCount, (unsigned __int8)idk))
		{
			int v5 = (int)&dword_1A55998[7 * VoiceCount];
			*(_DWORD*)(v5 + 40) = num;
			*(char*)(v5 + 36) = 1;
			*(char*)(v5 + 37) = idk;
			return VoiceCount;
		}
	}
	return -1;
}

UsercallFuncVoid(StopVoice, (Sint32 num), (num), 0x00443200, rEAX);

Void hk_StopVoice(Sint32 num)
{
	if (bassinit)
	{
		BASS_ChannelStop(voicechan[num]);
		BASS_StreamFree(voicechan[num]);
		voicechan[num] = NULL;
		return;
	}

	StopVoice.Original(num);
}

FunctionHook<void>	StopAllVoices(0x004431B0);

Void hk_StopAllVoices()
{
	for (int i = 0; i < 3; ++i)
	{
		if (voicechan[i])
		{
			BASS_ChannelStop(voicechan[i]);
			BASS_StreamFree(voicechan[i]);
			voicechan[i] = NULL;
		}
	}

	StopAllVoices.Original();
}

FunctionHook<Void>	PauseVoices(0x00443250);

Void hk_PauseVoices()
{
	for (int i = 0; i < 3; ++i)
	{
		if (voicechan[i])
		{
			BASS_ChannelPause(voicechan[i]);
		}
	}

	PauseVoices.Original();
}

FunctionHook<Void>	UnpauseVoices(0x00443290);

Void hk_UnpauseVoices()
{
	for (int i = 0; i < 3; ++i)
	{
		if (voicechan[i])
		{
			BASS_ChannelPlay(voicechan[i], false);
		}
	}

	UnpauseVoices.Original();
}

#pragma endregion

void Init_AudioBassHook(std::wstring extLibPath)
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
	StopVoice.Hook(hk_StopVoice);
	StopAllVoices.Hook(hk_StopAllVoices);
	PauseVoices.Hook(hk_PauseVoices);
	UnpauseVoices.Hook(hk_UnpauseVoices);

	return;
}