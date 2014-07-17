// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <DbgHelp.h>
#include <Shlwapi.h>
#include "SA2ModLoader.h"
#include "ModelInfo.h"
using namespace std;

typedef unordered_map<string, string> IniGroup;
struct IniGroupStr { IniGroup Element; };
typedef unordered_map<string, IniGroupStr> IniDictionary;
IniDictionary LoadINI(istream &textfile)
{
	IniDictionary result = IniDictionary();
	result[""] = IniGroupStr();
	IniGroupStr *curent = &result[""];
	while (textfile.good())
	{
		string line;
		getline(textfile, line);
		string sb = string();
		sb.reserve(line.length());
		bool startswithbracket = false;
		int firstequals = -1;
		int endbracket = -1;
		for (int c = 0; c < (int)line.length(); c++)
			switch (line[c])
		{
			case '\\': // escape character
				if (c + 1 == line.length())
					goto appendchar;
				c++;
				switch (line[c])
				{
				case 'n': // line feed
					sb += '\n';
					break;
				case 'r': // carriage return
					sb += '\r';
					break;
				default: // literal character
					goto appendchar;
				}
				break;
			case '=':
				if (firstequals == -1)
					firstequals = sb.length();
				goto appendchar;
			case '[':
				if (c == 0)
					startswithbracket = true;
				goto appendchar;
			case ']':
				endbracket = sb.length();
				goto appendchar;
			case ';': // comment character, stop processing this line
				c = line.length();
				break;
			default:
appendchar:
				sb += line[c];
				break;
		}
		line = sb;
		if (startswithbracket && endbracket != -1)
		{
			line = line.substr(1, endbracket - 1);
			result[line] = IniGroupStr();
			curent = &result[line];
		}
		else if (!line.empty())
		{
			string key;
			string value = "";
			if (firstequals > -1)
			{
				key = line.substr(0, firstequals);
				value = line.substr(firstequals + 1);
			}
			else
				key = line;
			(*curent).Element[key] = value;
		}
	}
	return result;
}

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
	if (c == '/')
		return '\\';
	else
		return c;
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
		ifstream str(repfn);
		unordered_map<string, string> indexes = LoadINI(str)[""].Element;
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
			for (auto i = indexes.cbegin(); i != indexes.cend(); i++)
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

enum CodeType : uint8_t
{
	write8, write16, write32, writefloat,
	add8, add16, add32, addfloat,
	sub8, sub16, sub32, subfloat,
	mulu8, mulu16, mulu32, mulfloat,
	muls8, muls16, muls32,
	divu8, divu16, divu32, divfloat,
	divs8, divs16, divs32,
	modu8, modu16, modu32,
	mods8, mods16, mods32,
	shl8, shl16, shl32,
	shru8, shru16, shru32,
	shrs8, shrs16, shrs32,
	rol8, rol16, rol32,
	ror8, ror16, ror32,
	and8, and16, and32,
	or8, or16, or32,
	xor8, xor16, xor32,
	ifeq8, ifeq16, ifeq32, ifeqfloat,
	ifne8, ifne16, ifne32, ifnefloat,
	ifltu8, ifltu16, ifltu32, ifltfloat,
	iflts8, iflts16, iflts32,
	ifltequ8, ifltequ16, ifltequ32, iflteqfloat,
	iflteqs8, iflteqs16, iflteqs32,
	ifgtu8, ifgtu16, ifgtu32, ifgtfloat,
	ifgts8, ifgts16, ifgts32,
	ifgtequ8, ifgtequ16, ifgtequ32, ifgteqfloat,
	ifgteqs8, ifgteqs16, ifgteqs32,
	ifmask8, ifmask16, ifmask32,
	ifkbkey,
	readreg8, readreg16, readreg32,
	writereg8, writereg16, writereg32,
	addreg8, addreg16, addreg32, addregfloat,
	subreg8, subreg16, subreg32, subregfloat,
	mulregu8, mulregu16, mulregu32, mulregfloat,
	mulregs8, mulregs16, mulregs32,
	divregu8, divregu16, divregu32, divregfloat,
	divregs8, divregs16, divregs32,
	modregu8, modregu16, modregu32,
	modregs8, modregs16, modregs32,
	shlreg8, shlreg16, shlreg32,
	shrregu8, shrregu16, shrregu32,
	shrregs8, shrregs16, shrregs32,
	rolreg8, rolreg16, rolreg32,
	rorreg8, rorreg16, rorreg32,
	andreg8, andreg16, andreg32,
	orreg8, orreg16, orreg32,
	xorreg8, xorreg16, xorreg32,
	ifeqreg8, ifeqreg16, ifeqreg32, ifeqregfloat,
	ifnereg8, ifnereg16, ifnereg32, ifneregfloat,
	ifltregu8, ifltregu16, ifltregu32, ifltregfloat,
	ifltregs8, ifltregs16, ifltregs32,
	iflteqregu8, iflteqregu16, iflteqregu32, iflteqregfloat,
	iflteqregs8, iflteqregs16, iflteqregs32,
	ifgtregu8, ifgtregu16, ifgtregu32, ifgtregfloat,
	ifgtregs8, ifgtregs16, ifgtregs32,
	ifgteqregu8, ifgteqregu16, ifgteqregu32, ifgteqregfloat,
	ifgteqregs8, ifgteqregs16, ifgteqregs32,
	ifmaskreg8, ifmaskreg16, ifmaskreg32,
	_else,
	endif,
	newregs
};

union valuetype
{
	uint32_t u32;
	int32_t s32;
	uint16_t u16;
	int16_t s16;
	uint8_t u8;
	int8_t s8;
	float f;
};

struct Code
{
	bool newregs;
	CodeType type;
	void *address;
	bool pointer;
	int offsetcount;
	int32_t *offsets;
	valuetype value;
	uint32_t repeatcount;
	list<Code> trueCodes;
	list<Code> falseCodes;
};

list<Code> codes;
vector<valuetype *> registers;

void *GetAddress(Code &code, valuetype *regs)
{
	void *addr = code.address;
	if (addr < (void *)16)
		addr = &regs[(int)addr];
	if (!code.pointer)
		return addr;
	addr = *(void **)addr;
	if (code.offsetcount == 0 || addr == nullptr)
		return addr;
	for (int i = 0; i < code.offsetcount - 1; i++)
	{
		addr = (void *)((uint32_t)addr + code.offsets[i]);
		addr = *(void **)addr;
		if (addr == nullptr)
			return nullptr;
	}
	addr = (void *)((uint32_t)addr + code.offsets[code.offsetcount - 1]);
	return addr;
}

#define ifcode(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addru##size op it->value.u##size; \
	addru##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodes(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrs##size op it->value.s##size; \
	addrs##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodef(op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrf op it->value.f; \
	addrf++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodereg(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addru##size op regs[it->value.u8].u##size; \
	addru##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcoderegs(size,op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrs##size op regs[it->value.u8].s##size; \
	addrs##size++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcoderegf(op) for (uint32_t i = 0; i < it->repeatcount; i++) \
{ \
	cond &= *addrf op regs[it->value.u8].f; \
	addrf++; \
} \
	if (cond) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

template<typename T>
inline void writecode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, data);
		address++;
	}
}

template<typename T>
inline void addcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address + data));
		address++;
	}
}

template<typename T>
inline void subcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address - data));
		address++;
	}
}

template<typename T>
inline void mulcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address * data));
		address++;
	}
}

template<typename T>
inline void divcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address / data));
		address++;
	}
}

template<typename T>
inline void modcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address % data));
		address++;
	}
}

template<typename T>
inline void shlcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address << data));
		address++;
	}
}

template<typename T>
inline void shrcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address >> data));
		address++;
	}
}

template<typename T>
inline void andcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address & data));
		address++;
	}
}

template<typename T>
inline void orcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address | data));
		address++;
	}
}

template<typename T>
inline void xorcode(T *address, uint32_t repeatcount, T data)
{
	for (uint32_t i = 0; i < repeatcount; i++)
	{
		WriteData(address, (T)(*address ^ data));
		address++;
	}
}

int regnum;
void ProcessCodeList(list<Code> &codes)
{
	for (list<Code>::iterator it = codes.begin(); it != codes.end(); it++)
	{
		if (it->newregs)
			regnum++;
		valuetype *regs = nullptr;
		if (regnum >= 0)
			regs = registers[regnum];
		void *address = GetAddress(*it, regs);
		if (it->type != ifkbkey && address == nullptr)
		{
			if (distance(it->falseCodes.begin(), it->falseCodes.end()) > 0)
				ProcessCodeList(it->falseCodes);
			continue;
		}
		uint8_t *addru8 = (uint8_t *)address;
		uint16_t *addru16 = (uint16_t *)address;
		uint32_t *addru32 = (uint32_t *)address;
		float *addrf = (float *)address;
		int8_t *addrs8 = (int8_t *)address;
		int16_t *addrs16 = (int16_t *)address;
		int32_t *addrs32 = (int32_t *)address;
		bool cond = true;
		switch (it->type)
		{
		case write8:
			writecode(addru8, it->repeatcount, it->value.u8);
			break;
		case write16:
			writecode(addru16, it->repeatcount, it->value.u16);
			break;
		case write32:
		case writefloat:
			writecode(addru32, it->repeatcount, it->value.u32);
			break;
		case add8:
			addcode(addru8, it->repeatcount, it->value.u8);
			break;
		case add16:
			addcode(addru16, it->repeatcount, it->value.u16);
			break;
		case add32:
			addcode(addru32, it->repeatcount, it->value.u32);
			break;
		case addfloat:
			addcode(addrf, it->repeatcount, it->value.f);
			break;
		case sub8:
			subcode(addru8, it->repeatcount, it->value.u8);
			break;
		case sub16:
			subcode(addru16, it->repeatcount, it->value.u16);
			break;
		case sub32:
			subcode(addru32, it->repeatcount, it->value.u32);
			break;
		case subfloat:
			subcode(addrf, it->repeatcount, it->value.f);
			break;
		case mulu8:
			mulcode(addru8, it->repeatcount, it->value.u8);
			break;
		case mulu16:
			mulcode(addru16, it->repeatcount, it->value.u16);
			break;
		case mulu32:
			mulcode(addru32, it->repeatcount, it->value.u32);
			break;
		case mulfloat:
			mulcode(addrf, it->repeatcount, it->value.f);
			break;
		case muls8:
			mulcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case muls16:
			mulcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case muls32:
			mulcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case divu8:
			divcode(addru8, it->repeatcount, it->value.u8);
			break;
		case divu16:
			divcode(addru16, it->repeatcount, it->value.u16);
			break;
		case divu32:
			divcode(addru32, it->repeatcount, it->value.u32);
			break;
		case divfloat:
			divcode(addrf, it->repeatcount, it->value.f);
			break;
		case divs8:
			divcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case divs16:
			divcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case divs32:
			divcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case modu8:
			modcode(addru8, it->repeatcount, it->value.u8);
			break;
		case modu16:
			modcode(addru16, it->repeatcount, it->value.u16);
			break;
		case modu32:
			modcode(addru32, it->repeatcount, it->value.u32);
			break;
		case mods8:
			modcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case mods16:
			modcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case mods32:
			modcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case shl8:
			shlcode(addru8, it->repeatcount, it->value.u8);
			break;
		case shl16:
			shlcode(addru16, it->repeatcount, it->value.u16);
			break;
		case shl32:
			shlcode(addru32, it->repeatcount, it->value.u32);
			break;
		case shru8:
			shrcode(addru8, it->repeatcount, it->value.u8);
			break;
		case shru16:
			shrcode(addru16, it->repeatcount, it->value.u16);
			break;
		case shru32:
			shrcode(addru32, it->repeatcount, it->value.u32);
			break;
		case shrs8:
			shrcode(addrs8, it->repeatcount, it->value.s8);
			break;
		case shrs16:
			shrcode(addrs16, it->repeatcount, it->value.s16);
			break;
		case shrs32:
			shrcode(addrs32, it->repeatcount, it->value.s32);
			break;
		case rol8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotl8(*addru8, it->value.u8)));
				addru8++;
			}
			break;
		case rol16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotl16(*addru16, it->value.u8)));
				addru16++;
			}
			break;
		case rol32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotl(*addru32, it->value.s32)));
				addru32++;
			}
			break;
		case ror8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotr8(*addru8, it->value.u8)));
				addru8++;
			}
			break;
		case ror16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotr16(*addru16, it->value.u8)));
				addru16++;
			}
			break;
		case ror32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotr(*addru32, it->value.s32)));
				addru32++;
			}
			break;
		case and8:
			andcode(addru8, it->repeatcount, it->value.u8);
			break;
		case and16:
			andcode(addru16, it->repeatcount, it->value.u16);
			break;
		case and32:
			andcode(addru32, it->repeatcount, it->value.u32);
			break;
		case or8:
			orcode(addru8, it->repeatcount, it->value.u8);
			break;
		case or16:
			orcode(addru16, it->repeatcount, it->value.u16);
			break;
		case or32:
			orcode(addru32, it->repeatcount, it->value.u32);
			break;
		case xor8:
			xorcode(addru8, it->repeatcount, it->value.u8);
			break;
		case xor16:
			xorcode(addru16, it->repeatcount, it->value.u16);
			break;
		case xor32:
			xorcode(addru32, it->repeatcount, it->value.u32);
			break;
		case ifeq8:
			ifcode(8,==)
				break;
		case ifeq16:
			ifcode(16,==)
				break;
		case ifeq32:
			ifcode(32,==)
				break;
		case ifeqfloat:
			ifcodef(==)
				break;
		case ifne8:
			ifcode(8,!=)
				break;
		case ifne16:
			ifcode(16,!=)
				break;
		case ifne32:
			ifcode(32,!=)
				break;
		case ifnefloat:
			ifcodef(!=)
				break;
		case ifltu8:
			ifcode(8,<)
				break;
		case ifltu16:
			ifcode(16,<)
				break;
		case ifltu32:
			ifcode(32,<)
				break;
		case ifltfloat:
			ifcodef(<)
				break;
		case iflts8:
			ifcodes(8,<)
				break;
		case iflts16:
			ifcodes(16,<)
				break;
		case iflts32:
			ifcodes(32,<)
				break;
		case ifltequ8:
			ifcode(8,<=)
				break;
		case ifltequ16:
			ifcode(16,<=)
				break;
		case ifltequ32:
			ifcode(32,<=)
				break;
		case iflteqfloat:
			ifcodef(<=)
				break;
		case iflteqs8:
			ifcodes(8,<=)
				break;
		case iflteqs16:
			ifcodes(16,<=)
				break;
		case iflteqs32:
			ifcodes(32,<=)
				break;
		case ifgtu8:
			ifcode(8,>)
				break;
		case ifgtu16:
			ifcode(16,>)
				break;
		case ifgtu32:
			ifcode(32,>)
				break;
		case ifgtfloat:
			ifcodef(>)
				break;
		case ifgts8:
			ifcodes(8,>)
				break;
		case ifgts16:
			ifcodes(16,>)
				break;
		case ifgts32:
			ifcodes(32,>)
				break;
		case ifgtequ8:
			ifcode(8,>=)
				break;
		case ifgtequ16:
			ifcode(16,>=)
				break;
		case ifgtequ32:
			ifcode(32,>=)
				break;
		case ifgteqfloat:
			ifcodef(>=)
				break;
		case ifgteqs8:
			ifcodes(8,>=)
				break;
		case ifgteqs16:
			ifcodes(16,>=)
				break;
		case ifgteqs32:
			ifcodes(32,>=)
				break;
		case ifmask8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru8 & it->value.u8) == it->value.u8;
				addru8++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmask16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u16) == it->value.u16;
				addru16++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmask32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u32) == it->value.u32;
				addru32++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifkbkey:
			if (GetAsyncKeyState(it->value.s32))
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case readreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				regs[it->value.u8 + i].u8 = *addru8;
				addru8++;
			}
			break;
		case readreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				regs[it->value.u8 + i].u16 = *addru16;
				addru16++;
			}
			break;
		case readreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				regs[it->value.u8 + i].u32 = *addru32;
				addru32++;
			}
			break;
		case writereg8:
			writecode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case writereg16:
			writecode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case writereg32:
			writecode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case addreg8:
			addcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case addreg16:
			addcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case addreg32:
			addcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case addregfloat:
			addcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case subreg8:
			subcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case subreg16:
			subcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case subreg32:
			subcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case subregfloat:
			subcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case mulregu8:
			mulcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case mulregu16:
			mulcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case mulregu32:
			mulcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case mulregfloat:
			mulcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case mulregs8:
			mulcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case mulregs16:
			mulcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case mulregs32:
			mulcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case divregu8:
			divcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case divregu16:
			divcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case divregu32:
			divcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case divregfloat:
			divcode(addrf, it->repeatcount, regs[it->value.u8].f);
			break;
		case divregs8:
			divcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case divregs16:
			divcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case divregs32:
			divcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case modregu8:
			modcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case modregu16:
			modcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case modregu32:
			modcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case modregs8:
			modcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case modregs16:
			modcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case modregs32:
			modcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case shlreg8:
			shlcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case shlreg16:
			shlcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case shlreg32:
			shlcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case shrregu8:
			shrcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case shrregu16:
			shrcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case shrregu32:
			shrcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case shrregs8:
			shrcode(addrs8, it->repeatcount, regs[it->value.u8].s8);
			break;
		case shrregs16:
			shrcode(addrs16, it->repeatcount, regs[it->value.u8].s16);
			break;
		case shrregs32:
			shrcode(addrs32, it->repeatcount, regs[it->value.u8].s32);
			break;
		case rolreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotl8(*addru8, regs[it->value.u8].u8)));
				addru8++;
			}
			break;
		case rolreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotl16(*addru16, regs[it->value.u8].u8)));
				addru8++;
			}
			break;
		case rolreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotl(*addru32, regs[it->value.u8].s32)));
				addru32++;
			}
			break;
		case rorreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru8, (uint8_t)(_rotr8(*addru8, regs[it->value.u8].u8)));
				addru8++;
			}
			break;
		case rorreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru16, (uint16_t)(_rotr16(*addru16, regs[it->value.u8].u8)));
				addru16++;
			}
			break;
		case rorreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				WriteData(addru32, (uint32_t)(_rotr(*addru32, regs[it->value.u8].s32)));
				addru32++;
			}
			break;
		case andreg8:
			andcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case andreg16:
			andcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case andreg32:
			andcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case orreg8:
			orcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case orreg16:
			orcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case orreg32:
			orcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case xorreg8:
			xorcode(addru8, it->repeatcount, regs[it->value.u8].u8);
			break;
		case xorreg16:
			xorcode(addru16, it->repeatcount, regs[it->value.u8].u16);
			break;
		case xorreg32:
			xorcode(addru32, it->repeatcount, regs[it->value.u8].u32);
			break;
		case ifeqreg8:
			ifcodereg(8,==)
				break;
		case ifeqreg16:
			ifcodereg(16,==)
				break;
		case ifeqreg32:
			ifcodereg(32,==)
				break;
		case ifeqregfloat:
			ifcoderegf(==)
				break;
		case ifnereg8:
			ifcodereg(8,!=)
				break;
		case ifnereg16:
			ifcodereg(16,!=)
				break;
		case ifnereg32:
			ifcodereg(32,!=)
				break;
		case ifneregfloat:
			ifcoderegf(!=)
				break;
		case ifltregu8:
			ifcodereg(8,<)
				break;
		case ifltregu16:
			ifcodereg(16,<)
				break;
		case ifltregu32:
			ifcodereg(32,<)
				break;
		case ifltregfloat:
			ifcoderegf(<)
				break;
		case ifltregs8:
			ifcoderegs(8,<)
				break;
		case ifltregs16:
			ifcoderegs(16,<)
				break;
		case ifltregs32:
			ifcoderegs(32,<)
				break;
		case iflteqregu8:
			ifcodereg(8,<=)
				break;
		case iflteqregu16:
			ifcodereg(16,<=)
				break;
		case iflteqregu32:
			ifcodereg(32,<=)
				break;
		case iflteqregfloat:
			ifcoderegf(<=)
				break;
		case iflteqregs8:
			ifcoderegs(8,<=)
				break;
		case iflteqregs16:
			ifcoderegs(16,<=)
				break;
		case iflteqregs32:
			ifcoderegs(32,<=)
				break;
		case ifgtregu8:
			ifcodereg(8,>)
				break;
		case ifgtregu16:
			ifcodereg(16,>)
				break;
		case ifgtregu32:
			ifcodereg(32,>)
				break;
		case ifgtregfloat:
			ifcoderegf(>)
				break;
		case ifgtregs8:
			ifcoderegs(8,>)
				break;
		case ifgtregs16:
			ifcoderegs(16,>)
				break;
		case ifgtregs32:
			ifcoderegs(32,>)
				break;
		case ifgteqregu8:
			ifcodereg(8,>=)
				break;
		case ifgteqregu16:
			ifcodereg(16,>=)
				break;
		case ifgteqregu32:
			ifcodereg(32,>=)
				break;
		case ifgteqregfloat:
			ifcoderegf(>=)
				break;
		case ifgteqregs8:
			ifcoderegs(8,>=)
				break;
		case ifgteqregs16:
			ifcoderegs(16,>=)
				break;
		case ifgteqregs32:
			ifcoderegs(32,>=)
				break;
		case ifmaskreg8:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru8 & it->value.u8) == regs[it->value.u8].u8;
				addru8++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmaskreg16:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u16) == regs[it->value.u8].u16;
				addru16++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmaskreg32:
			for (uint32_t i = 0; i < it->repeatcount; i++)
			{
				cond &= (*addru16 & it->value.u32) == regs[it->value.u8].u32;
				addru32++;
			}
			if (cond)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		}
	}
}

void __cdecl ProcessCodes()
{
	regnum = -1;
	ProcessCodeList(codes);
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

unsigned char ReadCodes(istream &stream, list<Code> &list)
{
	while (true)
	{
		uint8_t t = stream.get();
		if (t == 0xFF || t == _else || t == endif)
			return t;
		Code code = { };
		if (t == newregs)
		{
			code.newregs = true;
			valuetype *regs = new valuetype[16];
			memset(regs, 0, sizeof(valuetype) * 16);
			registers.push_back(regs);
			t = stream.get();
		}
		code.type = (CodeType)t;
		uintptr_t addr;
		stream.read((char *)&addr, sizeof(uintptr_t));
		code.pointer = (addr & 0x80000000u) == 0x80000000u;
		code.address = (void *)(addr & 0x7FFFFFFF);
		if (code.pointer)
		{
			code.offsetcount = stream.get();
			code.offsets = new int[code.offsetcount];
			for (int i = 0; i < code.offsetcount; i++)
				stream.read((char *)&code.offsets[i], sizeof(int32_t));
		}
		stream.read((char *)&code.value, sizeof(code.value));
		stream.read((char *)&code.repeatcount, sizeof(uint32_t));
		if ((code.type >= ifeq8 && code.type <= ifkbkey) || (code.type >= ifeqreg8 && code.type <= ifmaskreg32))
			switch (ReadCodes(stream, code.trueCodes))
			{
				case _else:
					if (ReadCodes(stream, code.falseCodes) == 0xFF)
						return 0xFF;
					break;
				case 0xFF:
					return 0xFF;
			}
		list.push_back(code);
	}
	return 0;
}

const char codemagic[] = "codev3";
void __cdecl InitMods(void)
{
	datadllhandle = LoadLibrary(L".\\resource\\gd_PC\\DLL\\Win32\\Data_DLL_orig.dll");
	if (!datadllhandle)
	{
		MessageBox(NULL, L"Data_DLL_orig.dll could not be loaded!\n\nSA2 will now proceed to abruptly exit.", L"SA2 Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	HookTheAPI();
	ifstream str("mods\\SA2ModLoader.ini");
	if (!str.is_open())
	{
		MessageBox(NULL, L"mods\\SA2ModLoader.ini could not be read!", L"SA2 Mod Loader", MB_ICONWARNING);
		return;
	}
	IniDictionary ini = LoadINI(str);
	str.close();
	settings = ini[""].Element;
	char pathbuf[MAX_PATH];
	GetModuleFileNameA(NULL, pathbuf, MAX_PATH);
	string exefilename = pathbuf;
	exefilename = exefilename.substr(exefilename.find_last_of("/\\") + 1);
	transform(exefilename.begin(), exefilename.end(), exefilename.begin(), ::tolower);
	string item = settings["DebugConsole"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
	{
		AllocConsole();
		SetConsoleTitle(L"SA2 Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		dbgConsole = true;
	}
	item = settings["DebugFile"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
	{
		dbgstr = ofstream("mods\\SA2ModLoader.log", ios_base::ate | ios_base::app);
		dbgFile = dbgstr.is_open();
	}
	if (dbgConsole || dbgFile)
	{
		WriteJump(PrintDebug, SA2DebugOutput);
		PrintDebug("SA2 Mod Loader version %d, built %s", ModLoaderVer, __TIMESTAMP__);
	}
	DWORD oldprot;
	VirtualProtect((void *)0x87342C, 0xA3BD4, PAGE_WRITECOPY, &oldprot);
	WriteJump((void *)LoadMDLFilePtr, LoadMDLFile_r);
	WriteJump((void *)ReleaseMDLFilePtr, ReleaseMDLFile_r);
	unordered_map<string, string> filereplaces = unordered_map<string, string>();
	char key[8];
	for (int i = 1; i < 999; i++)
	{
		sprintf_s(key, "Mod%d", i);
		if (settings.find(key) == settings.end())
			break;
		string dir = "mods\\" + settings[key];
		ifstream mstr(dir + "\\mod.ini");
		if (!mstr.is_open())
		{
			PrintDebug("Could not open file mod.ini in \"mods\\%s\".", settings[key].c_str());
			continue;
		}
		IniDictionary modini = LoadINI(mstr);
		IniGroup modinfo = modini[""].Element;
		PrintDebug("%d. %s", i, modinfo["Name"].c_str());
		IniDictionary::iterator gr = modini.find("IgnoreFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
			{
				filemap[NormalizePath(it->first)] = "nullfile";
				PrintDebug("Ignored file: %s", it->first.c_str());
			}
		}
		gr = modini.find("ReplaceFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
				filereplaces[NormalizePath(it->first)] = NormalizePath(it->second);
		}
		gr = modini.find("SwapFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
			{
				filereplaces[NormalizePath(it->first)] = NormalizePath(it->second);
				filereplaces[NormalizePath(it->second)] = NormalizePath(it->first);
			}
		}
		string sysfol = dir + "\\gd_pc";
		transform(sysfol.begin(), sysfol.end(), sysfol.begin(), ::tolower);
		if (GetFileAttributesA(sysfol.c_str()) & FILE_ATTRIBUTE_DIRECTORY)
			ScanFolder(sysfol, sysfol.length() + 1);
		if (modinfo.find("EXEFile") != modinfo.end())
		{
			string modexe = modinfo["EXEFile"];
			transform(modexe.begin(), modexe.end(), modexe.begin(), ::tolower);
			if (modexe.compare(exefilename) != 0)
			{
				const char *msg = ("Mod \"" + modinfo["Name"] + "\" should be run from \"" + modexe + "\", but you are running \"" + exefilename + "\".\n\nContinue anyway?").c_str();
				if (MessageBoxA(NULL, msg, "SA2 Mod Loader", MB_ICONWARNING | MB_YESNO) == IDNO)
					ExitProcess(1);
			}
		}
		string filename = modinfo["DLLFile"];
		if (!filename.empty())
		{
			filename = dir + "\\" + filename;
			HMODULE module = LoadLibraryA(filename.c_str());
			if (module)
			{
				const ModInfo *info = (ModInfo *)GetProcAddress(module, "SA2ModInfo");
				if (info)
				{
					if (info->Patches)
						for (int i = 0; i < info->PatchCount; i++)
							WriteData(info->Patches[i].address, info->Patches[i].data, info->Patches[i].datasize);
					if (info->Jumps)
						for (int i = 0; i < info->JumpCount; i++)
							WriteJump(info->Jumps[i].address, info->Jumps[i].data);
					if (info->Calls)
						for (int i = 0; i < info->CallCount; i++)
							WriteCall(info->Calls[i].address, info->Calls[i].data);
					if (info->Pointers)
						for (int i = 0; i < info->PointerCount; i++)
							WriteData((void**)info->Pointers[i].address, info->Pointers[i].data);
					if (info->Init)
						info->Init(dir.c_str());
				}
				else
					PrintDebug("File \"%s\" is not a valid mod file.", filename.c_str());
			}
			else
				PrintDebug("Failed loading file \"%s\".", filename.c_str());
		}
	}
	for (unordered_map<string,string>::iterator it = filereplaces.begin(); it != filereplaces.end(); it++)
	{
		unordered_map<string,char *>::iterator f = filemap.find(it->second);
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
	PrintDebug("Mod loading finished.");
	ifstream cstr("mods\\Codes.dat", ifstream::binary);
	if (cstr.is_open())
	{
		char buf[6];
		cstr.read(buf, sizeof(buf));
		if (memcmp(buf, codemagic, 6) != 0)
		{
			PrintDebug("Code file not in correct format.");
			goto closecodefile;
		}
		int32_t codecount;
		cstr.read((char *)&codecount, sizeof(int32_t));
		PrintDebug("Loading %d codes...", codecount);
		ReadCodes(cstr, codes);
	}
closecodefile:
	cstr.close();
	WriteJump((void *)0x77E897, ProcessCodes);
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