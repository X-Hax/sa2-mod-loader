// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <DbgHelp.h>
#include <Shlwapi.h>
#include "IniFile.hpp"
#include "SA2ModLoader.h"
#include "ModelInfo.h"
#include "CodeParser.hpp"
#include "Events.h"
using namespace std;

// TODO: Split file replacement into separate file(s)

HMODULE myhandle;
HMODULE datadllhandle;
FARPROC __stdcall MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (hModule == myhandle)
		return GetProcAddress(datadllhandle, lpProcName);
	else
		return GetProcAddress(hModule, lpProcName);
}

inline int backslashes(int c)
{
	return (c == '/') ? '\\' : c;
}

IniGroup settings;
unordered_map<string, char *> filemap;
const string resourcedir = "resource\\gd_pc\\";
string sa2dir;
const char *_ReplaceFile(const char *lpFileName)
{
	string path = lpFileName;
	transform(path.begin(), path.end(), path.begin(), backslashes);
	if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
		path = path.substr(2, path.length() - 2);
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	if (path.length() > sa2dir.length() && path.compare(0, sa2dir.length(), sa2dir) == 0)
		path = path.substr(sa2dir.length(), path.length() - sa2dir.length());
	unordered_map<string, char *>::iterator fileIter = filemap.find(path);
	if (fileIter != filemap.end())
		lpFileName = fileIter->second;
	return lpFileName;
}

unordered_map<ModelIndex *, list<ModelInfo>> modelfiles;

void markobjswapped(NJS_OBJECT *obj)
{
	while (obj)
	{
		IsByteswapped(&obj->evalflags);
		IsByteswapped(&obj->model);
		IsByteswapped(&obj->pos[0]);
		IsByteswapped(&obj->pos[1]);
		IsByteswapped(&obj->pos[2]);
		IsByteswapped(&obj->ang[0]);
		IsByteswapped(&obj->ang[1]);
		IsByteswapped(&obj->ang[2]);
		IsByteswapped(&obj->scl[0]);
		IsByteswapped(&obj->scl[1]);
		IsByteswapped(&obj->scl[2]);
		IsByteswapped(&obj->child);
		IsByteswapped(&obj->sibling);
		if (obj->chunkmodel)
		{
			IsByteswapped(&obj->chunkmodel->vlist);
			IsByteswapped(&obj->chunkmodel->plist);
			IsByteswapped(&obj->chunkmodel->center.x);
			IsByteswapped(&obj->chunkmodel->center.y);
			IsByteswapped(&obj->chunkmodel->center.z);
			IsByteswapped(&obj->chunkmodel->r);
		}
		if (obj->child)
			markobjswapped(obj->child);
		obj = obj->sibling;
	}
}

VoidFunc(sub_4297F0, 0x4297F0);
FunctionPointer(void, sub_48FA80, (NJS_OBJECT *, void *), 0x48FA80);
StdcallFunctionPointer(void, sub_419FC0, (void *), 0x419FC0);
FunctionPointer(void, sub_7A5974, (void *), 0x7A5974);
DataPointer(int, dword_1A55800, 0x1A55800);
DataPointer(int, dword_1AF191C, 0x1AF191C);
DataPointer(void *, dword_1AF1918, 0x1AF1918);
ModelIndex *__cdecl LoadMDLFile_ri(const char *filename)
{
	ModelIndex *result;
	char dir[MAX_PATH];
	PathCombineA(dir, resourcedir.c_str(), filename);
	PathRemoveExtensionA(dir);
	char *fn = PathFindFileNameA(dir);
	char combinedpath[MAX_PATH];
	PathCombineA(combinedpath, dir, fn);
	PathAddExtensionA(combinedpath, ".ini");
	const char *repfn = _ReplaceFile(combinedpath);
	if (PathFileExistsA(repfn))
	{
		FILE *f_mod_ini = fopen(repfn, "r");
		unique_ptr<IniFile> ini(new IniFile(f_mod_ini));
		fclose(f_mod_ini);
		const IniGroup *indexes = ini->getGroup("");
		strncpy_s(dir, repfn, MAX_PATH);
		PathRemoveFileSpecA(dir);
		WIN32_FIND_DATAA data;
		HANDLE hfind = FindFirstFileA((string(dir) + "\\*.sa2mdl").c_str(), &data);
		if (hfind == INVALID_HANDLE_VALUE)
			goto defaultmodelload;
		list<ModelInfo> files;
		vector<ModelIndex> modelindexes;
		sub_4297F0();
		do
		{
			PathCombineA(combinedpath, dir, data.cFileName);
			ModelInfo modelfile(combinedpath);
			files.push_back(modelfile);
			markobjswapped(modelfile.getmodel());
			for (auto i = indexes->cbegin(); i != indexes->cend(); i++)
			{
				void *found = modelfile.getdata(i->second);
				if (found != nullptr)
				{
					int ind = stoi(i->first);
					ModelIndex index = { ind, (NJS_OBJECT *)found };
					if (ind >= 0 && ind < 532 && !CharacterModels[ind].Model)
						CharacterModels[ind] = index;
					modelindexes.push_back(index);
				}
			}
		}
		while (FindNextFileA(hfind, &data) != 0);
		FindClose(hfind);
		ModelIndex endmarker = { -1, (NJS_OBJECT *)-1 };
		modelindexes.push_back(endmarker);
		result = new ModelIndex[modelindexes.size()];
		memcpy(result, modelindexes.data(), sizeof(ModelIndex) * modelindexes.size());
		modelfiles[result] = files;
		goto end;
	}

defaultmodelload:
	int v3; // edx@3
	int v5; // edi@5
	unsigned int v6; // eax@5
	unsigned int v7; // edi@5
	int v8; // edi@6
	ModelIndex *v9; // eax@7
	signed int v10; // edx@8
	NJS_OBJECT *v11; // ecx@11
	NJS_OBJECT *v12; // ecx@15
	void *v13; // eax@21
	int v14; // ebx@21
	int v15; // edi@21

	result = (ModelIndex *)LoadPRSFile(filename);
	if (result)
	{
		sub_4297F0();
		v3 = 0;
		if (result->Index != -1)
		{
			ModelIndex *v4 = result;
			do
			{
				v5 = v4->Index;
				v6 = (unsigned int)v4->Model;
				v4->Index = (((v4->Index << 16) | v4->Index & 0xFF00) << 8) | ((((unsigned int)v4->Index >> 16) | v5 & 0xFF0000) >> 8);
				v7 = v6;
				++v3;
				v4->Model = (NJS_OBJECT *)((((v6 << 16) | (unsigned __int16)(v6 & 0xFF00)) << 8) | (((v6 >> 16) | v7 & 0xFF0000) >> 8));
				v4 = &result[v3];
			}
			while (result[v3].Index != -1);
		}
		v8 = 0;
		if (result->Index != -1)
		{
			v9 = result;
			do
			{
				v10 = v9->Index;
				if (v9->Index >= 0 && v10 < 532 && !CharacterModels[v10].Model)
				{
					v11 = v9->Model;
					if (v11 && (unsigned int)v11 <= (unsigned int)result)
					{
						v11 = (NJS_OBJECT *)((DWORD)result + (DWORD)v11);
						v9->Model = v11;
					}
					CharacterModels[v10].Model = v11;
				}
				v12 = v9->Model;
				if (v12)
				{
					if ((unsigned int)v12 <= (unsigned int)result)
					{
						v12 = (NJS_OBJECT *)((DWORD)result + (DWORD)v12);
						v9->Model = v12;
					}
					sub_48FA80(v12, result);
				}
				++v8;
				v9 = &result[v8];
			}
			while (result[v8].Index != -1);
		}
end:
		--dword_1A55800;
		if (dword_1AF191C)
		{
			v13 = dword_1AF1918;
			v14 = *((DWORD *)dword_1AF1918 + 1);
			v15 = *((DWORD *)dword_1AF1918 + 1);
			if (!*(BYTE *)(v14 + 21))
			{
				do
				{
					sub_419FC0(*(void **)(v15 + 8));
					v15 = *(DWORD *)v15;
					sub_7A5974((void *)v14);
					v14 = v15;
				}
				while (!*(BYTE *)(v15 + 21));
				v13 = dword_1AF1918;
			}
			*((DWORD *)v13 + 1) = (DWORD)v13;
			dword_1AF191C = 0;
			*(DWORD *)dword_1AF1918 = (DWORD)dword_1AF1918;
			*((DWORD *)dword_1AF1918 + 2) = (DWORD)dword_1AF1918;
		}
	}
	return result;
}

__declspec(naked) void LoadMDLFile_r()
{
	__asm
	{
		push eax
		call LoadMDLFile_ri
		add esp, 4
		ret
	}
}

DataPointer(MemManFuncs *, MemoryManager, 0x1D19CAC);
void __cdecl ReleaseMDLFile_ri(ModelIndex *a1)
{
	if (a1->Index != -1)
	{
		ModelIndex *v1 = a1;
		do
		{
			if (v1->Index >= 0 && v1->Index < 532 && CharacterModels[v1->Index].Model == v1->Model)
				CharacterModels[v1->Index].Model = 0;
			++v1;
		}
		while (v1->Index != -1);
	}
	if (modelfiles.find(a1) != modelfiles.cend())
	{
		modelfiles.erase(a1);
		delete[] a1;
	}
	else
	{
		*((DWORD *)a1 - 1) = 0x89ABCDEFu;
		MemoryManager->Deallocate((char *)a1 - 4, "..\\..\\src\\file_ctl.c", 1091);
	}
}

__declspec(naked) void ReleaseMDLFile_r()
{
	__asm
	{
		push esi
		call ReleaseMDLFile_ri
		add esp, 4
		ret
	}
}

HANDLE __stdcall MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(_ReplaceFile(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

void HookTheAPI()
{
	ULONG ulSize = 0;
	PROC pNewFunction = NULL;
	PROC pActualFunction = NULL;

	PSTR pszModName = NULL;

	HMODULE hModule = GetModuleHandle(NULL);
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = NULL;

	pNewFunction = (PROC)MyGetProcAddress ;
	PROC pNewCreateFile = (PROC)MyCreateFileA;
	pActualFunction = GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "GetProcAddress");
	PROC pActualCreateFile = GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "CreateFileA");

	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR) ImageDirectoryEntryToData(
		hModule, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);

	if(NULL != pImportDesc)
	{
		for (; pImportDesc->Name; pImportDesc++)
		{
			// get the module name
			pszModName = (PSTR) ((PBYTE) hModule + pImportDesc->Name);

			if(NULL != pszModName)
			{
				// check if the module is kernel32.dll
				if (lstrcmpiA(pszModName, "Kernel32.dll") == 0)
				{
					// get the module
					PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA) ((PBYTE) hModule + pImportDesc->FirstThunk);

					for (; pThunk->u1.Function; pThunk++) 
					{
						PROC* ppfn = (PROC*) &pThunk->u1.Function;
						if(*ppfn == pActualFunction)
						{
							DWORD dwOldProtect = 0;
							VirtualProtect(ppfn, sizeof(pNewFunction), PAGE_WRITECOPY,&dwOldProtect);
							WriteProcessMemory(GetCurrentProcess(), ppfn, &pNewFunction, sizeof(pNewFunction), NULL);
							VirtualProtect(ppfn, sizeof(pNewFunction), dwOldProtect,&dwOldProtect);
						} // Function that we are looking for
						else if (*ppfn == pActualCreateFile)
						{
							DWORD dwOldProtect = 0;
							VirtualProtect(ppfn, sizeof(pNewCreateFile), PAGE_WRITECOPY,&dwOldProtect);
							WriteProcessMemory(GetCurrentProcess(), ppfn, &pNewCreateFile, sizeof(pNewCreateFile), NULL);
							VirtualProtect(ppfn, sizeof(pNewCreateFile), dwOldProtect,&dwOldProtect);
						}
					}
				} // Compare module name
			} // Valid module name
		}
	}
}

char *ShiftJISToUTF8(char *shiftjis)
{
	int cchWcs = MultiByteToWideChar(932, 0, shiftjis, -1, NULL, 0);
	if (cchWcs <= 0) return nullptr;
	wchar_t *wcs = new wchar_t[cchWcs];
	MultiByteToWideChar(932, 0, shiftjis, -1, wcs, cchWcs);
	int cbMbs = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (cbMbs <= 0) { delete[] wcs; return nullptr; }
	char *utf8 = new char[cbMbs];
	WideCharToMultiByte(CP_UTF8, 0, wcs, -1, utf8, cbMbs, NULL, NULL);
	delete[] wcs;
	return utf8;
}

bool dbgConsole, dbgFile;
ofstream dbgstr;
int __cdecl SA2DebugOutput(const char *Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	int length = vsnprintf(NULL, 0, Format, ap) + 1;
	va_end(ap);
	char *buf = new char[length];
	va_start(ap, Format);
	length = vsnprintf(buf, length, Format, ap);
	va_end(ap);
	if (dbgConsole)
		cout << buf << "\n";
	if (dbgFile && dbgstr.good())
	{
		char *utf8 = ShiftJISToUTF8(buf);
		dbgstr << utf8 << "\n";
		delete[] utf8;
	}
	delete[] buf;
	return length;
}

string NormalizePath(string path)
{
	string pathlower = path;
	if (pathlower.length() > 2 && (pathlower[0] == '.' && pathlower[1] == '\\'))
		pathlower = pathlower.substr(2, pathlower.length() - 2);
	transform(pathlower.begin(), pathlower.end(), pathlower.begin(), ::tolower);
	return pathlower;
}

void ScanFolder(string path, int length)
{
	_WIN32_FIND_DATAA data;
	HANDLE hfind = FindFirstFileA((path + "\\*").c_str(), &data);
	if (hfind == INVALID_HANDLE_VALUE)
		return;
	do
	{
		if (data.cFileName[0] == '.')
			continue;
		else if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			ScanFolder(path + "\\" + data.cFileName, length);
		else
		{
			string filebase = path + "\\" + data.cFileName;
			transform(filebase.begin(), filebase.end(), filebase.begin(), ::tolower);
			string modfile = filebase;
			filebase = filebase.substr(length);
			string origfile = resourcedir + filebase;
			char *buf = new char[modfile.length() + 1];
			if (filemap.find(origfile) != filemap.end())
				delete[] filemap[origfile];
			filemap[origfile] = buf;
			modfile.copy(buf, modfile.length());
			buf[modfile.length()] = 0;
			PrintDebug("Replaced file: \"%s\" = \"%s\"", origfile.c_str(), buf);
		}
	}
	while (FindNextFileA(hfind, &data) != 0);
	FindClose(hfind);
}

unordered_map<unsigned char, unordered_map<short, StartPosition>> StartPositions;
bool StartPositionsModified;

struct startposdata { unsigned char character; const StartPosition *positions; };

const startposdata startposaddrs[] = {
	{ Characters_Sonic, (StartPosition *)0x1746F20 },
	{ Characters_Shadow, (StartPosition *)0x17472C0 },
	{ Characters_Tails, (StartPosition *)0x1747BE8 },
	{ Characters_Eggman, (StartPosition *)0x1747BE8 },
	{ Characters_Knuckles, (StartPosition *)0x17475D8 },
	{ Characters_Rouge, (StartPosition *)0x17478F0 },
	{ Characters_MechTails, (StartPosition *)0x1748008 },
	{ Characters_MechEggman, (StartPosition *)0x1747C40 },
	{ Characters_SuperSonic, (StartPosition *)0x1748378 },
	{ Characters_SuperShadow, (StartPosition *)0x1748400 }
};

void InitializeStartPositionLists()
{
	for (unsigned int i = 0; i < LengthOfArray(startposaddrs); i++)
	{
		const StartPosition * origlist = startposaddrs[i].positions;
		StartPositions[startposaddrs[i].character] = unordered_map<short, StartPosition>();
		unordered_map<short, StartPosition> &newlist = StartPositions[startposaddrs[i].character];
		while (origlist->Level != LevelIDs_Invalid)
			newlist[origlist->Level] = *origlist++;
	}
}

void RegisterStartPosition(unsigned char character, const StartPosition &position)
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

int __cdecl LoadStartPosition_ri(int playerNum, NJS_VECTOR *position, Rotation *rotation)
{
	ObjectMaster *v1 = MainCharacter[playerNum];
	if (position)
	{
		position->z = 0.0;
		position->y = 0.0;
		position->x = 0.0;
	}
	if (rotation)
	{
		rotation->z = 0;
		rotation->y = 0;
		rotation->x = 0;
	}
	if (v1)
	{
		CharObj2 *v4 = MainCharObj2[playerNum];
		StartPosition *v5;
		if (v4)
		{
			auto iter = StartPositions.find(v4->CharID);
			if (iter == StartPositions.cend())
				return 1;
			auto iter2 = iter->second.find(CurrentLevel);
			if (iter2 == iter->second.cend())
				return 1;
			v5 = &iter2->second;
		}
		else
			return 1;
		int v6;
		if ( TwoPlayerMode
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow1
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
			|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge )
			v6 = (playerNum != 0) + 1;
		else
			v6 = 0;
		if ( rotation )
			rotation->y = *(&v5->Rotation1P + v6);
		if ( position )
		{
			NJS_VECTOR *v8 = &(&v5->Position1P)[v6];
			position->x = v8->x;
			position->y = v8->y;
			position->z = v8->z;
		}
		return 1;
	}
	return 0;
}

__declspec(naked) void LoadStartPosition_r(Rotation *rotation)
{
	__asm
	{
		mov eax, [rotation]
		push eax
		push edi
		push ecx
		call LoadStartPosition_ri
		add esp, 12
		retn
	}
}

unordered_map<unsigned char, unordered_map<short, LevelEndPosition>> _2PIntroPositions;
bool _2PIntroPositionsModified;

struct endposdata { unsigned char character; const LevelEndPosition *positions; };

const endposdata _2pintroposaddrs[] = {
	{ Characters_Sonic, (LevelEndPosition *)0x1746F20 },
	{ Characters_Shadow, (LevelEndPosition *)0x17472C0 },
	{ Characters_Tails, (LevelEndPosition *)0x1747BE8 },
	{ Characters_Eggman, (LevelEndPosition *)0x1747BE8 },
	{ Characters_Knuckles, (LevelEndPosition *)0x17475D8 },
	{ Characters_Rouge, (LevelEndPosition *)0x17478F0 },
	{ Characters_MechTails, (LevelEndPosition *)0x1748008 },
	{ Characters_MechEggman, (LevelEndPosition *)0x1747C40 },
	{ Characters_SuperSonic, (LevelEndPosition *)0x1748378 },
	{ Characters_SuperShadow, (LevelEndPosition *)0x1748400 }
};

void Initialize2PIntroPositionLists()
{
	for (unsigned int i = 0; i < LengthOfArray(_2pintroposaddrs); i++)
	{
		const LevelEndPosition * origlist = _2pintroposaddrs[i].positions;
		_2PIntroPositions[startposaddrs[i].character] = unordered_map<short, LevelEndPosition>();
		if (origlist == nullptr)
			return;
		unordered_map<short, LevelEndPosition> &newlist = _2PIntroPositions[startposaddrs[i].character];
		while (origlist->Level != LevelIDs_Invalid)
			newlist[origlist->Level] = *origlist++;
	}
}

void Register2PIntroPosition(unsigned char character, const LevelEndPosition &position)
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

DataArray(char, byte_1DE4664, 0x1DE4664, 2);
DataPointer(void *, off_1DE95E0, 0x1DE95E0);
FunctionPointer(void, sub_46DC70, (int a1, NJS_VECTOR *a2, char a3), 0x46DC70);

void __cdecl Load2PIntroPos_ri(int playerNum)
{
	ObjectMaster *v1 = MainCharacter[playerNum];
	CharObj1 *v4;
	NJS_VECTOR *v8;
	if (v1)
	{
		v4 = v1->Data1;
		CharObj2 *v3 = MainCharObj2[playerNum];
		if (v3)
		{
			auto iter = _2PIntroPositions.find(v3->CharID);
			if (iter != _2PIntroPositions.cend())
			{
				auto iter2 = iter->second.find(CurrentLevel);
				if (iter2 != iter->second.cend())
				{
					LevelEndPosition *v5 = &iter2->second;
					int v6 = playerNum != 0;
					v4->Rotation.y = *(&v5->Mission2YRotation + v6);
					NJS_VECTOR *v12 = &(&v5->Mission2Position)[v6];
					v4->Position = *v12;
					v8 = &v4->Position;
					*((int *)*(&off_1DE95E0 + playerNum) + 7) = v4->Rotation.y;
					*(float*)&v3->field_1A0[5] = v4->Position.y - 10.0f;
					goto LABEL_16;
				}
			}
		}
	}
	v4->Position.z = 0.0;
	v8 = &v4->Position;
	v4->Position.y = 0.0;
	v4->Rotation.y = 0;
	v4->Position.x = 0.0;
LABEL_16:
	sub_46DC70(playerNum, v8, 0);
	*((char *)&v4->field_2C->CollisionArray->field_2) |= 0x70u;
	*(int *)&MainCharObj2[playerNum]->field_70[6] = 0;
	byte_1DE4664[playerNum & 1] = *(char*)0x1DE4660;
	CharObj2 *v9 = MainCharObj2[playerNum];
	float *v10 = (float *)*(&off_1DE95E0 + playerNum);
	if (v9)
	{
		v9->Speed.z = 0.0;
		v9->Speed.y = 0.0;
		v9->Speed.x = 0.0;
	}
	if (v10)
	{
		v10[2] = 0.0;
		v10[1] = 0.0;
		v10[0] = 0.0;
	}
}

__declspec(naked) void Load2PIntroPos_r()
{
	__asm
	{
		push eax
		call Load2PIntroPos_ri
		add esp, 4
		retn
	}
}

static const char *mainsavepath = "resource/gd_PC/SAVEDATA";
static const char *GetMainSavePath()
{
	return mainsavepath;
}

static const char *chaosavepath = "resource/gd_PC/SAVEDATA";
static const char *GetChaoSavePath()
{
	return chaosavepath;
}

const HelperFunctions helperFunctions = {
	ModLoaderVer,
	RegisterStartPosition,
	ClearStartPositionList,
	Register2PIntroPosition,
	Clear2PIntroPositionList,
	GetMainSavePath,
	GetChaoSavePath
};

void __cdecl InitMods(void)
{
	datadllhandle = LoadLibrary(L".\\resource\\gd_PC\\DLL\\Win32\\Data_DLL_orig.dll");
	if (!datadllhandle)
	{
		MessageBox(NULL, L"Data_DLL_orig.dll could not be loaded!\n\nSA2 will now proceed to abruptly exit.", L"SA2 Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	HookTheAPI();
	FILE *f_ini = _wfopen(L"mods\\SA2ModLoader.ini", L"r");
	if (!f_ini)
	{
		MessageBox(NULL, L"mods\\SA2ModLoader.ini could not be read!", L"SA2 Mod Loader", MB_ICONWARNING);
		return;
	}
	unique_ptr<IniFile> ini(new IniFile(f_ini));
	fclose(f_ini);
	char pathbuf[MAX_PATH];
	GetModuleFileNameA(NULL, pathbuf, MAX_PATH);
	string exefilename = pathbuf;
	exefilename = exefilename.substr(exefilename.find_last_of("/\\") + 1);
	transform(exefilename.begin(), exefilename.end(), exefilename.begin(), ::tolower);
	const IniGroup *settings = ini->getGroup("");
	if (settings->getBool("DebugConsole"))
	{
		AllocConsole();
		SetConsoleTitle(L"SA2 Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		dbgConsole = true;
	}
	if (settings->getBool("DebugFile"))
	{
		dbgstr = ofstream("mods\\SA2ModLoader.log", ios_base::ate | ios_base::app);
		dbgFile = dbgstr.is_open();
	}
	if (dbgConsole || dbgFile)
	{
		WriteJump(PrintDebug, SA2DebugOutput);
		PrintDebug("SA2 Mod Loader version %d, built %s", ModLoaderVer, __TIMESTAMP__);
	}

	// Unprotect the .rdata section.
	// TODO: Get .rdata address and length dynamically.
	DWORD oldprot;
	VirtualProtect((void *)0x87342C, 0xA3BD4, PAGE_WRITECOPY, &oldprot);

	WriteJump((void *)LoadMDLFilePtr, LoadMDLFile_r);
	WriteJump((void *)ReleaseMDLFilePtr, ReleaseMDLFile_r);

	// Map of files to replace and/or swap.
	unordered_map<string, string> filereplaces = unordered_map<string, string>();

	InitializeStartPositionLists();
	Initialize2PIntroPositionLists();

	vector<std::pair<ModInitFunc, string>> initfuncs;
	vector<std::pair<string, string>> errors;

	string _mainsavepath, _chaosavepath;

	// It's mod loading time!
	PrintDebug("Loading mods...");
	char key[8];
	for (int i = 1; i < 999; i++)
	{
		sprintf_s(key, "Mod%d", i);
		if (!settings->hasKey(key))
			break;
		const string mod_dir = "mods\\" + settings->getString(key);
		const string mod_inifile = mod_dir + "\\mod.ini";
		FILE *f_mod_ini = fopen(mod_inifile.c_str(), "r");
		if (!f_mod_ini)
		{
			PrintDebug("Could not open file mod.ini in \"mods\\%s\".\n", mod_dir.c_str());
			errors.push_back(std::pair<string, string>(mod_dir, "mod.ini missing"));
			continue;
		}
		unique_ptr<IniFile> ini_mod(new IniFile(f_mod_ini));
		fclose(f_mod_ini);

		const IniGroup *modinfo = ini_mod->getGroup("");
		const string mod_name = modinfo->getString("Name");
		PrintDebug("%d. %s\n", i, mod_name.c_str());

		if (ini_mod->hasGroup("IgnoreFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("IgnoreFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
				iter != data->end(); ++iter)
			{
				filemap[iter->first] = "nullfile";
				PrintDebug("Ignored file: %s\n", iter->first.c_str());
			}
		}

		if (ini_mod->hasGroup("ReplaceFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("ReplaceFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
				iter != data->end(); ++iter)
			{
				filereplaces[NormalizePath(iter->first)] =
					NormalizePath(iter->second);
			}
		}

		if (ini_mod->hasGroup("SwapFiles"))
		{
			const IniGroup *group = ini_mod->getGroup("SwapFiles");
			auto data = group->data();
			for (unordered_map<string, string>::const_iterator iter = data->begin();
				iter != data->end(); ++iter)
			{
				filereplaces[NormalizePath(iter->first)] =
					NormalizePath(iter->second);
				filereplaces[NormalizePath(iter->second)] =
					NormalizePath(iter->first);
			}
		}

		// Check for gd_pc replacements
		string sysfol = mod_dir + "\\gd_pc";
		transform(sysfol.begin(), sysfol.end(), sysfol.begin(), ::tolower);
		if (GetFileAttributesA(sysfol.c_str()) & FILE_ATTRIBUTE_DIRECTORY)
			ScanFolder(sysfol, sysfol.length() + 1);

		// Check if a custom EXE is required.
		if (modinfo->hasKeyNonEmpty("EXEFile"))
		{
			string modexe = modinfo->getString("EXEFile");
			transform(modexe.begin(), modexe.end(), modexe.begin(), ::tolower);

			// Are we using the correct EXE?
			if (modexe.compare(exefilename) != 0)
			{
				const char *msg = ("Mod \"" + modinfo->getString("Name") + "\" should be run from \"" + modexe + "\", but you are running \"" + exefilename + "\".\n\nContinue anyway?").c_str();
				if (MessageBoxA(NULL, msg, "SA2 Mod Loader", MB_ICONWARNING | MB_YESNO) == IDNO)
					ExitProcess(1);
			}
		}

		// Check if the mod has a DLL file.
		if (modinfo->hasKeyNonEmpty("DLLFile"))
		{
			// Prepend the mod directory.
			string dll_filename = mod_dir + '\\' + modinfo->getString("DLLFile");
			HMODULE module = LoadLibraryA(dll_filename.c_str());

			if (module == nullptr)
			{
				DWORD error = GetLastError();
				LPSTR buffer;
				size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);

				string message(buffer, size);
				LocalFree(buffer);

				PrintDebug("Failed loading mod DLL \"%s\": %s\n", dll_filename.c_str(), message.c_str());
				errors.push_back(std::pair<string, string>(mod_name, "DLL error - " + message));
			}
			else
			{
				const ModInfo *info = (ModInfo *)GetProcAddress(module, "SA2ModInfo");
				if (info)
				{
					if (info->Patches)
					{
						for (int j = 0; j < info->PatchCount; j++)
							WriteData(info->Patches[j].address, info->Patches[j].data, info->Patches[j].datasize);
					}
					if (info->Jumps)
					{
						for (int j = 0; j < info->JumpCount; j++)
							WriteJump(info->Jumps[j].address, info->Jumps[j].data);
					}
					if (info->Calls)
					{
						for (int j = 0; j < info->CallCount; j++)
							WriteCall(info->Calls[j].address, info->Calls[j].data);
					}
					if (info->Pointers)
					{
						for (int j = 0; j < info->PointerCount; j++)
							WriteData((void**)info->Pointers[j].address, info->Pointers[j].data);
					}
					if (info->Init)
					{
						initfuncs.push_back({ info->Init, mod_dir });
					}

					const ModInitFunc init = (const ModInitFunc)GetProcAddress(module, "Init");
					if (init)
						initfuncs.push_back({ init, mod_dir });

					const PatchList* patches = (const PatchList*)GetProcAddress(module, "Patches");
					if (patches)
					{
						for (int j = 0; j < patches->Count; j++)
							WriteData(patches->Patches[j].address, patches->Patches[j].data, patches->Patches[j].datasize);
					}

					const PointerList* jumps = (const PointerList*)GetProcAddress(module, "Jumps");
					if (jumps)
					{
						for (int j = 0; j < jumps->Count; j++)
							WriteJump(jumps->Pointers[j].address, jumps->Pointers[j].data);
					}

					const PointerList* calls = (const PointerList*)GetProcAddress(module, "Calls");
					if (calls)
					{
						for (int j = 0; j < calls->Count; j++)
							WriteJump(calls->Pointers[j].address, calls->Pointers[j].data);
					}

					const PointerList* pointers = (const PointerList*)GetProcAddress(module, "Pointers");
					if (pointers)
					{
						for (int j = 0; j < pointers->Count; j++)
							WriteData((void**)pointers->Pointers[j].address, pointers->Pointers[j].data);
					}

					RegisterEvent(modFrameEvents, module, "OnFrame");
					RegisterEvent(modInputEvents, module, "OnInput");
					RegisterEvent(modControlEvents, module, "OnControl");
				}
				else
				{
					PrintDebug("File \"%s\" is not a valid mod file.\n", dll_filename.c_str());
					errors.push_back(std::pair<string, string>(mod_name, "Not a valid mod file."));
				}
			}
		}

		if (modinfo->getBool("RedirectMainSave"))
			_mainsavepath = mod_dir + "\\SAVEDATA";

		if (modinfo->getBool("RedirectChaoSave"))
			_chaosavepath = mod_dir + "\\SAVEDATA";
	}

	if (!errors.empty())
	{
		std::stringstream message;
		message << "The following mods didn't load correctly:" << std::endl;

		for (auto& i : errors)
			message << std::endl << i.first << ": " << i.second;

		MessageBoxA(nullptr, message.str().c_str(), "Mods failed to load", MB_OK | MB_ICONERROR);
	}

	// Replace filenames. ("ReplaceFiles", "SwapFiles")
	for (auto it = filereplaces.begin(); it != filereplaces.end(); it++)
	{
		auto f = filemap.find(it->second);
		if (f != filemap.end())
			filemap[it->first] = f->second;
		else
		{
			char *buf = new char[it->second.length() + 1];
			filemap[it->first] = buf;
			it->second.copy(buf, it->second.length());
			buf[it->second.length()] = 0;
			PrintDebug("Replaced file: \"%s\" = \"%s\"", it->first.c_str(), buf);
		}
	}

	for (unsigned int i = 0; i < initfuncs.size(); i++)
		initfuncs[i].first(initfuncs[i].second.c_str(), helperFunctions);

	if (StartPositionsModified)
		WriteJump((void *)LoadStartPositionPtr, LoadStartPosition_r);
	if (_2PIntroPositionsModified)
		WriteJump((void *)Load2PIntroPosPtr, Load2PIntroPos_r);

	if (!_mainsavepath.empty())
	{
		char *buf = new char[_mainsavepath.size() + 1];
		strncpy(buf, _mainsavepath.c_str(), _mainsavepath.size() + 1);
		mainsavepath = buf;
		string tmp = "./" + _mainsavepath + "/SONIC2B__S%02d";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x445312, buf);
		WriteData((char **)0x689684, buf);
		WriteData((char **)0x689AA9, buf);
		WriteData((char **)0x689D22, buf);
		WriteData((char **)0x689D4D, buf);
		tmp = "./" + _mainsavepath + "/SONIC2B__D%02d";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x445332, buf);
		tmp = "./" + _mainsavepath + "/SONIC2B__S01";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x445317, buf);
		WriteData((char **)0x689689, buf);
		WriteData((char **)0x689AAE, buf);
		WriteData((char **)0x689D27, buf);
		WriteData((char **)0x689D52, buf);
		WriteData((char **)0x173D070, buf);
		tmp = "./" + _mainsavepath + "/SONIC2B__D01";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x445337, buf);
		WriteData((char **)0x173D07C, buf);
	}

	if (!_chaosavepath.empty())
	{
		char *buf = new char[_chaosavepath.size() + 1];
		strncpy(buf, _chaosavepath.c_str(), _chaosavepath.size() + 1);
		chaosavepath = buf;
		string tmp = "./" + _chaosavepath + "/SONIC2B__ALF";
		buf = new char[tmp.size() + 1];
		strncpy(buf, tmp.c_str(), tmp.size() + 1);
		WriteData((char **)0x457027, buf);
		WriteData((char **)0x52DE84, buf);
		WriteData((char **)0x52DF48, buf);
		WriteData((char **)0x52E063, buf);
		WriteData((char **)0x52E2A8, buf);
		WriteData((char **)0x52FEC7, buf);
		WriteData((char **)0x5323A1, buf);
		WriteData((char **)0x5323B5, buf);
		WriteData((char **)0x5324A2, buf);
		WriteData((char **)0x53257E, buf);
		WriteData((char **)0x532595, buf);
		WriteData((char **)0x532672, buf);
	}

	PrintDebug("Mod loading finished.");

	ifstream patches_str("mods\\Patches.dat", ifstream::binary);
	if (patches_str.is_open())
	{
		CodeParser patchParser;
		static const char codemagic[6] = { 'c', 'o', 'd', 'e', 'v', '4' };
		char buf[sizeof(codemagic)];
		patches_str.read(buf, sizeof(buf));
		if (!memcmp(buf, codemagic, sizeof(codemagic)))
		{
			int codecount_header;
			patches_str.read((char*)&codecount_header, sizeof(codecount_header));
			PrintDebug("Loading %d patches...\n", codecount_header);
			patches_str.seekg(0);
			int codecount = patchParser.readCodes(patches_str);
			if (codecount >= 0)
			{
				PrintDebug("Loaded %d patches.\n", codecount);
				patchParser.processCodeList();
			}
			else
			{
				PrintDebug("ERROR loading patches: ");
				switch (codecount)
				{
				case -EINVAL:
					PrintDebug("Patch file is not in the correct format.\n");
					break;
				default:
					PrintDebug("%s\n", strerror(-codecount));
					break;
				}
			}
		}
		else
		{
			PrintDebug("Patch file is not in the correct format.\n");
		}
		patches_str.close();
	}

	ifstream codes_str("mods\\Codes.dat", ifstream::binary);
	if (codes_str.is_open())
	{
		static const char codemagic[6] = { 'c', 'o', 'd', 'e', 'v', '4' };
		char buf[sizeof(codemagic)];
		codes_str.read(buf, sizeof(buf));
		if (!memcmp(buf, codemagic, sizeof(codemagic)))
		{
			int codecount_header;
			codes_str.read((char*)&codecount_header, sizeof(codecount_header));
			PrintDebug("Loading %d codes...\n", codecount_header);
			codes_str.seekg(0);
			int codecount = codeParser.readCodes(codes_str);
			if (codecount >= 0)
			{
				PrintDebug("Loaded %d codes.\n", codecount);
				codeParser.processCodeList();
			}
			else
			{
				PrintDebug("ERROR loading codes: ");
				switch (codecount)
				{
				case -EINVAL:
					PrintDebug("Code file is not in the correct format.\n");
					break;
				default:
					PrintDebug("%s\n", strerror(-codecount));
					break;
				}
			}
		}
		else
		{
			PrintDebug("Code file is not in the correct format.\n");
		}
		codes_str.close();
	}

	// Sets up code/event handling
	InitOnFrame();	// OnFrame
	WriteJump((void*)0x0077E897, OnInput);
	WriteJump((void*)0x00441D41, OnControl);

	if (MainUserConfig->FullScreen == 0)
	{
		int endWidth = (int)HorizontalResolution;
		int endHeight = (int)VerticalResolution;
		RECT winFrame, winClient;

		GetWindowRect(MainWindowHandle, &winFrame);
		GetClientRect(MainWindowHandle, &winClient);

		// The game uses GetSystemMetrics which appears to always report incorrect values,
		// so this code calculates the window frame size manually.
		endWidth += ((winFrame.right - winFrame.left) - (winClient.right - winClient.left));
		endHeight += ((winFrame.bottom - winFrame.top) - (winClient.bottom - winClient.top));

		SetWindowPos(MainWindowHandle, nullptr, 0, 0, endWidth, endHeight,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_ASYNCWINDOWPOS);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	int bufsize;
	char *buf;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		myhandle = hModule;
		bufsize = GetCurrentDirectoryA(0, NULL);
		buf = new char[bufsize];
		GetCurrentDirectoryA(bufsize, buf);
		sa2dir = buf;
		delete[] buf;
		transform(sa2dir.begin(), sa2dir.end(), sa2dir.begin(), ::tolower);
		sa2dir += "\\";
		WriteJump((void *)0x77DEEA, InitMods);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}