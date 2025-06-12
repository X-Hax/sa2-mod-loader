#include "stdafx.h"
#include "SA2ModLoader.h"
#include "DebugText.h"
#include "TextureReplacement.h"
#include "FileSystem.h"
#include "InterpolationFixes.h"

using namespace std;

extern unordered_map<unsigned char, unordered_map<short, StartPosition>> StartPositions;
extern bool StartPositionsModified;

void RegisterStartPosition(unsigned char character, const StartPosition& position)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		StartPositions[character][position.Level] = position;
		StartPositionsModified = true;
		break;
	}
}

void ClearStartPositionList(unsigned char character)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		StartPositions[character].clear();
		StartPositionsModified = true;
		break;
	}
}

extern unordered_map<unsigned char, unordered_map<short, LevelEndPosition>> _2PIntroPositions;
extern bool _2PIntroPositionsModified;

void Register2PIntroPosition(unsigned char character, const LevelEndPosition& position)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		_2PIntroPositions[character][position.Level] = position;
		_2PIntroPositionsModified = true;
		break;
	}
}

void Clear2PIntroPositionList(unsigned char character)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		_2PIntroPositions[character].clear();
		_2PIntroPositionsModified = true;
		break;
	}
}

extern unordered_map<unsigned char, unordered_map<short, StartPosition>> EndPositions;
extern bool EndPositionsModified;

void RegisterEndPosition(unsigned char character, const StartPosition& position)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		EndPositions[character][position.Level] = position;
		EndPositionsModified = true;
		break;
	}
}

void ClearEndPositionList(unsigned char character)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		EndPositions[character].clear();
		EndPositionsModified = true;
		break;
	}
}

extern bool Mission23EndPositionsModified;
extern unordered_map<unsigned char, unordered_map<short, LevelEndPosition>> Mission23EndPositions;

void RegisterMission23EndPosition(unsigned char character, const LevelEndPosition& position)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		Mission23EndPositions[character][position.Level] = position;
		Mission23EndPositionsModified = true;
		break;
	}
}

void ClearMission23EndPositionList(unsigned char character)
{
	switch (character)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Tails:
	case Characters_Eggman:
	case Characters_Knuckles:
	case Characters_Rouge:
	case Characters_MechTails:
	case Characters_MechEggman:
	case Characters_SuperSonic:
	case Characters_SuperShadow:
		Mission23EndPositions[character].clear();
		Mission23EndPositionsModified = true;
		break;
	}
}

extern const char* mainsavepath;
const char* GetMainSavePath()
{
	return mainsavepath;
}

extern const char* chaosavepath;
const char* GetChaoSavePath()
{
	return chaosavepath;
}

void HookExport(LPCSTR exportName, const void* newdata)
{
	intptr_t hModule = (intptr_t) * *datadllhandle;
	ULONG ulSize = 0;
	PIMAGE_EXPORT_DIRECTORY pExportDesc = (PIMAGE_EXPORT_DIRECTORY)ImageDirectoryEntryToData(
		**datadllhandle, TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &ulSize);

	if (pExportDesc != nullptr)
	{
		intptr_t* funcaddrs = (intptr_t*)(hModule + pExportDesc->AddressOfFunctions);
		intptr_t* nameaddrs = (intptr_t*)(hModule + pExportDesc->AddressOfNames);
		short* ordaddrs = (short*)(hModule + pExportDesc->AddressOfNameOrdinals);

		for (unsigned int i = 0; i < pExportDesc->NumberOfNames; ++i)
		{
			LPCSTR ename = (LPCSTR)(hModule + nameaddrs[i]);

			if (!lstrcmpiA(ename, exportName))
			{
				auto thing = &funcaddrs[ordaddrs[i]];
				DWORD dwOldProtect = 0;
				VirtualProtect(thing, sizeof(intptr_t), PAGE_WRITECOPY, &dwOldProtect);
				*thing = (intptr_t)newdata - hModule;
				VirtualProtect(thing, sizeof(intptr_t), dwOldProtect, &dwOldProtect);
			}
		}
	}
}

const char* __cdecl GetReplaceablePath(const char* path)
{
	return sadx_fileMap.replaceFile(path);
}

void _ReplaceFile(const char* src, const char* dst)
{
	string orig = sadx_fileMap.normalizePath(src);
	string mod = sadx_fileMap.normalizePath(dst);
	sadx_fileMap.addReplaceFile(orig, mod);
	auto i = orig.find("\\prs\\");
	if (i != string::npos)
	{
		orig.erase(i, 4);
		ReplaceFileExtension(orig, ".prs");
		sadx_fileMap.addReplaceFile(orig, mod);
	}
}

void ReplaceFileAtIndex(const char* src, const char* dst, int modIndex)
{
	string orig = sadx_fileMap.normalizePath(src);
	string mod = sadx_fileMap.normalizePath(dst);
	sadx_fileMap.addReplaceFile(orig, mod, modIndex);
	auto i = orig.find("\\prs\\");
	if (i != string::npos)
	{
		orig.erase(i, 4);
		ReplaceFileExtension(orig, ".prs");
		sadx_fileMap.addReplaceFile(orig, mod, modIndex);
	}
}

void SetWindowTitle(const wchar_t* title)
{
	if (MainWindowHandle)
		SetWindowTextW(MainWindowHandle, title);
}

extern std::vector<Mod> modlist;

namespace ModListImpl
{
	ModList::iterator begin()
	{
		return &*modlist.cbegin();
	}

	ModList::iterator end()
	{
		return &*modlist.cend();
	}

	ModList::reference at(ModList::size_type pos)
	{
		return modlist[pos];
	}

	ModList::pointer data()
	{
		return modlist.data();
	}

	ModList::size_type size()
	{
		return modlist.size();
	}

	ModList::iterator find(const char* id)
	{
		for (auto& iter : modlist)
			if (!strcmp(iter.ID, id))
				return &iter;
		return nullptr;
	}

	ModList::iterator find_by_name(const char* name)
	{
		for (auto& iter : modlist)
			if (!strcmp(iter.Name, name))
				return &iter;
		return nullptr;
	}

	ModList::iterator find_by_folder(const char* folder)
	{
		for (auto& iter : modlist)
			if (!strcmp(iter.Folder, folder))
				return &iter;
		return nullptr;
	}

	ModList::iterator find_by_dll(HMODULE handle)
	{
		if (handle == nullptr)
			return nullptr;
		for (auto& iter : modlist)
			if (iter.DLLHandle == handle)
				return &iter;
		return nullptr;
	}
}

ModList modList = {
	ModListImpl::begin,
	ModListImpl::end,
	ModListImpl::at,
	ModListImpl::data,
	ModListImpl::size,
	ModListImpl::find,
	ModListImpl::find_by_name,
	ModListImpl::find_by_folder,
	ModListImpl::find_by_dll
};

uint16_t voicenum = 2727;
uint16_t RegisterVoice(const char* fileJP, const char* fileEN)
{
	if (voicenum == UINT16_MAX)
		return 0;
	char buf[MAX_PATH];
	sprintf_s(buf,  "resource\\gd_pc\\event_adx\\%04d.ahx", voicenum);
	_ReplaceFile(buf, fileJP);
	sprintf_s(buf,  "resource\\gd_pc\\event_adx_e\\%04d.ahx", voicenum);
	_ReplaceFile(buf, fileEN);
	return voicenum++;
}

void UnreplaceFile(const char* file)
{
	sadx_fileMap.unreplaceFile(file);
}

void PushInterpolationFix()
{
	interpolation::push();
}

void PopInterpolationFix()
{
	interpolation::pop();
}

extern LoaderSettings loaderSettings;

HelperFunctions helperFunctions = {
	ModLoaderVer,
	RegisterStartPosition,
	ClearStartPositionList,
	Register2PIntroPosition,
	Clear2PIntroPositionList,
	GetMainSavePath,
	GetChaoSavePath,
	RegisterEndPosition,
	ClearEndPositionList,
	RegisterMission23EndPosition,
	ClearMission23EndPositionList,
	HookExport,
	GetReplaceablePath,
	_ReplaceFile,
	SetWindowTitle,
	debug_text::SetFontSize,
	debug_text::SetFontColor,
	debug_text::DisplayString,
	debug_text::DisplayStringFormatted,
	debug_text::DisplayNumber,
	&loaderSettings,
	&modList,
	&RegisterVoice,
	&ReplaceTexture,
	&UnreplaceFile,
	&PushInterpolationFix,
	&PopInterpolationFix,
	&ReplaceFileAtIndex
};
