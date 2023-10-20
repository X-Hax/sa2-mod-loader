#pragma once

#include "SA2ModLoader.h"
#include "CodeParser.hpp"
#include <vector>

using TextureLoadEvent = void(__cdecl*)(NJS_TEXMANAGE*, const char*, Uint32);

extern std::vector<ModEvent> modFrameEvents;
extern std::vector<ModEvent> modInputEvents;
extern std::vector<ModEvent> modControlEvents;
extern std::vector<ModEvent> modExitEvents;
extern std::vector<TextureLoadEvent> modCustomTextureLoadEvents;
extern std::vector<ModEvent> modRenderDeviceLost;
extern std::vector<ModEvent> modRenderDeviceReset;
extern std::vector<ModEvent> onRenderSceneEnd;
extern std::vector<ModEvent> onRenderSceneStart;
extern CodeParser codeParser;

void RegisterEvent(std::vector<ModEvent>& eventList, HMODULE module, const char* name);

/**
* Calls all registered events in the specified event list.
* @param eventList The list of events to trigger.
*/
static inline void RaiseEvents(const std::vector<ModEvent>& eventList)
{
	for (auto& i : eventList)
		i();
}

void InitOnFrame();
void __cdecl OnFrame();
void __cdecl OnInput();
void __cdecl OnControl();
void __cdecl OnExit(UINT uExitCode, int a1, int a2);