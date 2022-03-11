#include "stdafx.h"
#include "Events.h"
#include "CodeParser.hpp"

std::vector<ModEvent> modFrameEvents;
std::vector<ModEvent> modInputEvents;
std::vector<ModEvent> modControlEvents;
CodeParser codeParser;

/**
* Registers an event to the specified event list.
* @param eventList The event list to add to.
* @param module The module for the mod DLL.
* @param name The name of the exported function from the module (i.e OnFrame)
*/
void RegisterEvent(std::vector<ModEvent>& eventList, HMODULE module, const char* name)
{
	const ModEvent modEvent = (const ModEvent)GetProcAddress(module, name);

	if (modEvent != nullptr)
		eventList.push_back(modEvent);
}

void __cdecl OnFrame()
{
	codeParser.processCodeList();
	RaiseEvents(modFrameEvents);

	//fix alt f4 not working
	if (GetKeyState(VK_MENU) & 0x8000 && GetKeyState(VK_F4) & 0x8000)
		SendMessage(GetForegroundWindow(), WM_CLOSE, NULL, NULL);

}

#pragma region OnFrame Initialization

void* caseDefault_ptr = (void*)0x004340CC;
void* case08_ptr = (void*)0x0043405D;
void* case09_ptr = (void*)0x0043407E;
void* case10_ptr = (void*)0x004340B6;

void __declspec(naked) OnFrame_MidJump()
{
	__asm
	{
		push eax
		call OnFrame
		pop eax

		pop edi
		pop esi
		pop ebp
		pop ebx
		pop ecx
		retn
	}
}

void* OnFrame_Hook_ptr = (void*)0x004340E7;
void __declspec(naked) OnFrame_Hook()
{
	__asm
	{
		push eax
		call OnFrame
		pop eax
		retn
	}
}

void InitOnFrame()
{
	WriteJump(case08_ptr, OnFrame_MidJump);
	WriteJump(case09_ptr, OnFrame_MidJump);
	WriteJump(case10_ptr, OnFrame_MidJump);

	// OnFrame caseDefault
	// Occurs if the current game mode isn't 8, 9 or 10, and byte_174AFF9 == 1
	WriteJump(caseDefault_ptr, OnFrame_MidJump);

	// OnFrame OnFrame_Hook
	// Occurs at the end of the function (effectively the "else" to the statement above)
	WriteJump(OnFrame_Hook_ptr, OnFrame_Hook);
}

#pragma endregion

void __cdecl OnInput()
{
	RaiseEvents(modInputEvents);
}

void __cdecl OnControl()
{
	RaiseEvents(modControlEvents);
}
