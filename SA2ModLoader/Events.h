#pragma once

#include "SA2ModLoader.h"
#include "CodeParser.hpp"
#include <vector>

extern std::vector<ModEvent> modFrameEvents;
extern std::vector<ModEvent> modInputEvents;
extern std::vector<ModEvent> modControlEvents;
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
