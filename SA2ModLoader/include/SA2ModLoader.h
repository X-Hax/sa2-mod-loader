#ifndef SA2MODLOADER_H
#define SA2MODLOADER_H

#if !defined(_M_IX86) && !defined(__i386__)
#error Mods must be built targeting 32-bit x86, change your settings.
#endif

#include <WTypes.h>
#include <cstdint>

#include "MemAccess.h"
#include "SA2Enums.h"
#include "SA2Structs.h"
#include "SA2Variables.h"
#include "SA2Functions.h"

// ModInfo

static const int ModLoaderVer = 7;

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

struct PatchList
{
	const PatchInfo *Patches;
	int Count;
};

struct PointerInfo
{
	void *address;
	void *data;
};

struct PointerList
{
	const PointerInfo *Pointers;
	int Count;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }
#define ptrdecl(address,data) { (void*)address, (void*)data }

#undef ReplaceFile // WinAPI function
struct HelperFunctions
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void(__cdecl *RegisterStartPosition)(unsigned char character, const StartPosition &position);
	// Clears the list of registered start positions for a character.
	void(__cdecl *ClearStartPositionList)(unsigned char character);
	// Registers a 2P intro position for a character.
	void(__cdecl *Register2PIntroPosition)(unsigned char character, const LevelEndPosition &position);
	// Clears the list of registered 2P intro positions for a character.
	void(__cdecl *Clear2PIntroPositionList)(unsigned char character);
	// Returns the path where main game save files are stored.
	// Requires version >= 4.
	const char *(__cdecl *GetMainSavePath)();
	// Returns the path where Chao save files are stored.
	// Requires version >= 4.
	const char *(__cdecl *GetChaoSavePath)();
	// Registers an end position for a character.
	// Requires version >= 5.
	void(__cdecl *RegisterEndPosition)(unsigned char character, const StartPosition &position);
	// Clears the list of registered end positions for a character.
	// Requires version >= 5.
	void(__cdecl *ClearEndPositionList)(unsigned char character);
	// Registers an end position for missions 2 and 3 for a character.
	// Requires version >= 5.
	void(__cdecl *RegisterMission23EndPosition)(unsigned char character, const LevelEndPosition &position);
	// Clears the list of registered end positions for missions 2 and 3 for a character.
	// Requires version >= 5.
	void(__cdecl *ClearMission23EndPositionList)(unsigned char character);
	// Replaces data exported from the Data DLL with your own data.
	// Requires version >= 6.
	void(__cdecl* HookExport)(LPCSTR exportName, const void* newdata);
	/**
	* @brief Gets the real path to a replaceable file.
	*
	* If your mod contains files in its SYSTEM folder that it loads manually,
	* you can use this function to retrieve the full path to the file. This
	* allows other mods to replace this file without any extra work from you.
	* Requires version >= 7.
	*
	* @param path The file path (e.g "resource\\gd_PC\\my_cool_file.bin")
	* @return The replaced path to the file.
	*/
	const char* (__cdecl* GetReplaceablePath)(const char* path);
	// Replaces the source file with the destination file.
	// Requires version >= 7.
	void(__cdecl* ReplaceFile)(const char* src, const char* dst);
	// Sets the window title.
	// Requires version >= 7.
	void(__cdecl* SetWindowTitle)(const wchar_t* title);
};

typedef void(__cdecl *ModInitFunc)(const char *path, const HelperFunctions &helperFunctions);

typedef void(__cdecl *ModEvent)();

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

static inline void njPushMatrix(NJS_MATRIX_PTR m)
{
	if (_nj_current_matrix_ptr_ + 12 <= *(float**)0x2670588)
	{
		if (!m)
			m = _nj_current_matrix_ptr_;
		float *v5 = _nj_current_matrix_ptr_ + 12;
		memcpy(v5, m, 0x30u);
		_nj_current_matrix_ptr_ = v5;
	}
}

static inline void njPopMatrix(int n)
{
	for (int i = 0; i < n; i++)
		if (_nj_current_matrix_ptr_ - 12 >= *(float**)0x267053C)
			_nj_current_matrix_ptr_ -= 12;
		else
			break;
}
#endif