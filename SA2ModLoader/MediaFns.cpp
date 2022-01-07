/**
 * SA2 Mod Loader
 * Media functions.
 */

#include "stdafx.h"
#include "MediaFns.hpp"
#include "FileReplacement.h"
#include "FileSystem.h"

#include "bass_vgmstream.h"

#include <string>
#include <vector>
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

void* ReleaseSoundEffects_r() {
	BASS_Free();
	return sub_437E90();
}

void Init_AudioBassHook() {
	WriteCall((void*)0x435511, ReleaseSoundEffects_r);
	BassInit_r();
	return;
}