/**
 * SA2 Mod Loader.
 * Mod metadata structures.
 */

#ifndef SA2MODLOADER_SA2MODINFO_H
#define SA2MODLOADER_SA2MODINFO_H

#include "SA2Structs.h"

static const int ModLoaderVer = 16;

struct PatchInfo
{
	void* address;
	const void* data;
	int datasize;
};

struct PatchList
{
	const PatchInfo* Patches;
	int Count;
};

struct PointerInfo
{
	void* address;
	void* data;
};

struct PointerList
{
	const PointerInfo* Pointers;
	int Count;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }
#define ptrdecl(address,data) { (void*)address, (void*)data }

struct LoaderSettings
{
	bool DebugConsole;
	bool DebugScreen;
	bool DebugFile;
	bool DebugCrashLog;
	bool PauseWhenInactive;
	bool DisableExitPrompt;
	int ScreenNum;
	bool BorderlessWindow;
	bool FullScreen;
	bool SkipIntro;
	bool SyncLoad;
	int HorizontalResolution;
	int VerticalResolution;
	int VoiceLanguage;
	int TextLanguage;
	bool CustomWindowSize;
	int WindowWidth;
	int WindowHeight;
	bool ResizableWindow;
	bool MaintainAspectRatio; // Deprecated, use StretchToWindow
	bool FramerateLimiter;
	int TestSpawnLevel;
	int TestSpawnCharacter;
	int TestSpawnPlayer2;
	bool TestSpawnPositionEnabled;
	int TestSpawnX;
	int TestSpawnY;
	int TestSpawnZ;
	int TestSpawnRotation;
	int TestSpawnEvent;
	int TestSpawnSaveID;
	bool ExtendVertexBuffer;
	bool EnvMapFix;
	bool ScreenFadeFix;
	bool CECarFix;
	bool ParticlesFix;
	bool KeepAspectWhenResizing; // Deprecated, use StretchToWindow
	int ScreenMode; // Window Mode (Windowed, Fullscreen, Borderless Fullscren, or Custom Window);
	bool DisableBorderImage; // Requires version >= 14.
	bool StretchToWindow; // Stretch content to the window instead of respecting aspect ratio. Requires version >= 14.
	// Paths
	std::wstring ExtLibPath; // Location of the 'extlib' folder; requires version >= 15
};

struct ModDependency
{
	const char* ID;
	const char* Folder;
	const char* Name;
	const char* Link;
};

struct ModDepsList
{
	typedef ModDependency value_type;
	typedef int size_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;
	typedef pointer iterator;

	pointer data;
	size_type size;

	// Retrieves an iterator to the start of the list (enables range-based for).
	iterator begin()
	{
		return data;
	}

	// Retrieves an iterator to the end of the list (enables range-based for).
	iterator end()
	{
		return data + size;
	}

	reference operator [](size_type pos)
	{
		return data[pos];
	}
};

struct Mod
{
	const char* Name;
	const char* Author;
	const char* Description;
	const char* Version;
	const char* Folder;
	const char* ID;
	HMODULE DLLHandle;
	bool MainSaveRedirect;
	bool ChaoSaveRedirect;
	const ModDepsList Dependencies;

	template <typename T>
	T GetDllExport(const char* name) const
	{
		if (!DLLHandle)
			return nullptr;
		return reinterpret_cast<T>(GetProcAddress(DLLHandle, name));
	}
};

struct ModList
{
	typedef Mod value_type;
	typedef int size_type;
	typedef const value_type& reference;
	typedef const value_type* pointer;
	typedef pointer iterator;

	// Retrieves an iterator to the start of the list (enables range-based for).
	iterator(*begin)();
	// Retrieves an iterator to the end of the list (enables range-based for).
	iterator(*end)();
	// Retrieves the item at position pos.
	reference(*at)(size_type pos);
	// Retrieves a pointer to the start of the list.
	pointer(*data)();
	// Retrieves the number of items in the list.
	size_type(*size)();
	// Find a mod by its ID.
	iterator(*find)(const char* id);
	// Find a mod by its name.
	iterator(*find_by_name)(const char* name);
	// Find a mod by its folder.
	iterator(*find_by_folder)(const char* folder);
	// Find a mod by its DLL handle.
	iterator(*find_by_dll)(HMODULE handle);

	reference operator [](size_type pos)
	{
		return at(pos);
	}
};

#undef ReplaceFile // WinAPI function
struct HelperFunctions
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void(__cdecl* RegisterStartPosition)(unsigned char character, const StartPosition& position);
	// Clears the list of registered start positions for a character.
	void(__cdecl* ClearStartPositionList)(unsigned char character);
	// Registers a 2P intro position for a character.
	void(__cdecl* Register2PIntroPosition)(unsigned char character, const LevelEndPosition& position);
	// Clears the list of registered 2P intro positions for a character.
	void(__cdecl* Clear2PIntroPositionList)(unsigned char character);
	// Returns the path where main game save files are stored.
	// Requires version >= 4.
	const char* (__cdecl* GetMainSavePath)();
	// Returns the path where Chao save files are stored.
	// Requires version >= 4.
	const char* (__cdecl* GetChaoSavePath)();
	// Registers an end position for a character.
	// Requires version >= 5.
	void(__cdecl* RegisterEndPosition)(unsigned char character, const StartPosition& position);
	// Clears the list of registered end positions for a character.
	// Requires version >= 5.
	void(__cdecl* ClearEndPositionList)(unsigned char character);
	// Registers an end position for missions 2 and 3 for a character.
	// Requires version >= 5.
	void(__cdecl* RegisterMission23EndPosition)(unsigned char character, const LevelEndPosition& position);
	// Clears the list of registered end positions for missions 2 and 3 for a character.
	// Requires version >= 5.
	void(__cdecl* ClearMission23EndPositionList)(unsigned char character);
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
	// Sets the size of the debug font, defaults to 12.
	// Requires version >= 8
	void(__cdecl* SetDebugFontSize)(float size);
	// Sets the argb color of the debug font, defaults to 0xFFBFBFBF.
	// Requires version >= 8
	void(__cdecl* SetDebugFontColor)(int color);
	// Displays a string on screen at a specific location (using NJM_LOCATION)
	// Example: DisplayDebugString(NJM_LOCATION(x, y), "string");
	// Requires version >= 8
	void(__cdecl* DisplayDebugString)(int loc, const char* str);
	// Displays a formatted string on screen at a specific location (using NJM_LOCATION)
	// Requires version >= 8
	void(__cdecl* DisplayDebugStringFormatted)(int loc, const char* Format, ...);
	// Displays a number on screen at a specific location (using NJM_LOCATION)
	// If the number of digits is superior, it will add leading zeroes.
	// Example: DisplayDebugNumber(NJM_LOCATION(x, y), 123, 5); will display 00123.
	// Requires version >= 8
	void(__cdecl* DisplayDebugNumber)(int loc, int value, int numdigits);

	// The settings that the mod loader was initialized with.
	// Requires version >= 9.
	const LoaderSettings* LoaderSettings;

	// API for listing information on loaded mods.
	// Requires version >= 9.
	const ModList* Mods;

	/**
	* @brief Registers an ID for a new voice.
	* Requires version >= 10.
	*
	* @param fileJP: The path to the audio file to play for Japanese.
	* @param fileEN: The path to the audio file to play for English.
	* @return The ID number for your voice, or 0 if the list is full.
	*
	*/
	uint16_t(__cdecl* RegisterVoice)(const char* fileJP, const char* fileEN);

	// Replaces an individual texture from a GVM file with an image file.
	// Requires version >= 11.
	void(__cdecl* ReplaceTexture)(const char* gvm_name, const char* tex_name, const char* file_path, uint32_t gbix, uint32_t width, uint32_t height);

	// Removes any file replacements for the specified file.
	// Requires version >= 12.
	void(__cdecl* UnreplaceFile)(const char* file);
	/**
	* @brief Push Interpolation fix for animations.
	*
	* Use this at the beginning of a display function and please disable it at the end after so it doesn't run for all animations in the game.
	* Requires version >= 13.
	*
	*/
	void(__cdecl* PushInterpolationFix)();

	// Disable interpolation fix for animations, use it at the end of a display function.
	// Requires version >= 13.
	void(__cdecl* PopInterpolationFix)();

	// Retrieves the index of the mod that replaced the file specified in the path.
	// Returns 0 if the file wasn't replaced by a mod.
	// Requires version >= 16.
	int(__cdecl* GetFileModIndex)(const char* path);

	// Replaces the source file with the destination file, with the specified mod index being used to determine the order of replacement.
	// Requires version >= 16.
	void(__cdecl* ReplaceFileAtIndex)(const char* src, const char* dst, int modIndex);
};

typedef void(__cdecl* ModInitFunc)(const char* path, const HelperFunctions& helperFunctions);

typedef void(__cdecl* ModEvent)();

struct ModInfo
{
	int Version;
	void(__cdecl* Init)(const char* path, const HelperFunctions& helperFunctions);
	const PatchInfo* Patches;
	int PatchCount;
	const PointerInfo* Jumps;
	int JumpCount;
	const PointerInfo* Calls;
	int CallCount;
	const PointerInfo* Pointers;
	int PointerCount;
};

#endif // SA2MODLOADER_SA2MODINFO_H