#ifndef SA2MODLOADER_H
#define SA2MODLOADER_H

#include <WTypes.h>
#include <cstdint>

#include "ModLoader/MemAccess.h"
#include "SA2ModLoader/SA2Enums.h"
#include "SA2ModLoader/SA2Structs.h"
#include "SA2ModLoader/SA2Variables.h"
#include "SA2ModLoader/SA2Functions.h"

// ModInfo

static const int ModLoaderVer = 3;

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }

struct PointerInfo
{
	void *address;
	void *data;
};

#define ptrdecl(address,data) { (void*)address, (void*)data }

struct HelperFunctions_v3
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void (__cdecl *RegisterStartPosition)(unsigned char character, const StartPosition &position);
	// Clears the list of registered start positions for a character.
	void (__cdecl *ClearStartPositionList)(unsigned char character);
	// Registers a 2P intro position for a character.
	void (__cdecl *Register2PIntroPosition)(unsigned char character, const LevelEndPosition &position);
	// Clears the list of registered 2P intro positions for a character.
	void (__cdecl *Clear2PIntroPositionList)(unsigned char character);
};

typedef HelperFunctions_v3 HelperFunctions;

struct ModInfo
{
	int Version;
	void (__cdecl *Init)(const char *path, const HelperFunctions &helperFunctions);
	const PatchInfo *Patches;
	int PatchCount;
	const PointerInfo *Jumps;
	int JumpCount;
	const PointerInfo *Calls;
	int CallCount;
	const PointerInfo *Pointers;
	int PointerCount;
};
#endif