// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <DbgHelp.h>
#include <cstdio>
#include "SA2ModLoader.h"
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
unordered_map<string, void *> dataoverrides = unordered_map<string, void *>();
FARPROC __stdcall MyGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (hModule == myhandle)
	{
		unordered_map<string, void *>::iterator iter = dataoverrides.find(lpProcName);
		if (iter != dataoverrides.end())
			return (FARPROC)iter->second;
		else
			return GetProcAddress(datadllhandle, lpProcName);
	}
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
unordered_map<string, char *> filemap = unordered_map<string, char *>();
unordered_map<string, string> filereplaces = unordered_map<string, string>();
const string resourcedir = "resource\\gd_pc\\";
string sa2dir;
const string savedatadir = "resource\\gd_pc\\savedata\\";
CRITICAL_SECTION filereplacesection;
const char *_ReplaceFile(const char *lpFileName)
{
	EnterCriticalSection(&filereplacesection);
	string path = lpFileName;
	transform(path.begin(), path.end(), path.begin(), backslashes);
	if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
		path = path.substr(2, path.length() - 2);
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	if (path.length() > sa2dir.length() && path.compare(0, sa2dir.length(), sa2dir) == 0)
		path = path.substr(sa2dir.length(), path.length() - sa2dir.length());
	unordered_map<string, string>::iterator replIter = filereplaces.find(path);
	if (replIter != filereplaces.end())
		path = replIter->second;
	unordered_map<string, char *>::iterator fileIter = filemap.find(path);
	if (fileIter != filemap.end())
		lpFileName = fileIter->second;
	LeaveCriticalSection(&filereplacesection);
	return lpFileName;
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
	_else,
	endif
};

struct Code
{
	CodeType type;
	void *address;
	bool pointer;
	int offsetcount;
	int32_t *offsets;
	uint32_t value;
	list<Code> trueCodes;
	list<Code> falseCodes;
};

list<Code> codes = list<Code>();

void *GetAddress(Code &code)
{
	if (!code.pointer)
		return code.address;
	void *addr = code.address;
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

#define ifcode(size,op) if (*(uint##size##_t *)address op (uint##size##_t)it->value) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodes(size,op) if (*(int##size##_t *)address op (int##size##_t)it->value) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

#define ifcodef(op) if (*(float *)address op *(float *)&it->value) \
	ProcessCodeList(it->trueCodes); \
else \
	ProcessCodeList(it->falseCodes);

void ProcessCodeList(list<Code> &codes)
{
	for (list<Code>::iterator it = codes.begin(); it != codes.end(); it++)
	{
		void *address = GetAddress(*it);
		if (it->type != ifkbkey && address == nullptr)
		{
			if (distance(it->falseCodes.begin(), it->falseCodes.end()) > 0)
				ProcessCodeList(it->falseCodes);
			continue;
		}
		switch (it->type)
		{
		case write8:
			WriteData(address, (uint8_t)it->value);
			break;
		case write16:
			WriteData(address, (uint16_t)it->value);
			break;
		case write32:
		case writefloat:
			WriteData(address, (uint32_t)it->value);
			break;
		case and8:
			WriteData(address, (uint8_t)(*(uint8_t *)address & it->value));
			break;
		case and16:
			WriteData(address, (uint16_t)(*(uint16_t *)address & it->value));
			break;
		case and32:
			WriteData(address, (uint32_t)(*(uint32_t *)address & it->value));
			break;
		case or8:
			WriteData(address, (uint8_t)(*(uint8_t *)address | it->value));
			break;
		case or16:
			WriteData(address, (uint16_t)(*(uint16_t *)address | it->value));
			break;
		case or32:
			WriteData(address, (uint32_t)(*(uint32_t *)address | it->value));
			break;
		case xor8:
			WriteData(address, (uint8_t)(*(uint8_t *)address ^ it->value));
			break;
		case xor16:
			WriteData(address, (uint16_t)(*(uint16_t *)address ^ it->value));
			break;
		case xor32:
			WriteData(address, (uint32_t)(*(uint32_t *)address ^ it->value));
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
			if ((*(uint8_t *)address & (uint8_t)it->value) == (uint8_t)it->value)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmask16:
			if ((*(uint16_t *)address & (uint16_t)it->value) == (uint16_t)it->value)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifmask32:
			if ((*(uint32_t *)address & it->value) == it->value)
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		case ifkbkey:
			if (GetAsyncKeyState(it->value))
				ProcessCodeList(it->trueCodes);
			else
				ProcessCodeList(it->falseCodes);
			break;
		}
	}
}

void __cdecl ProcessCodes()
{
	ProcessCodeList(codes);
}

int __cdecl SA2DebugOutput_i(const char *Format, ...)
{
	va_list ap;
	va_start(ap, Format);
	int result = vprintf(Format, ap);
	va_end(ap);
	printf("\n");
	return result;
}

const char *addrfmt = "[0x%08X] ";
__declspec(naked) int __cdecl SA2DebugOutput(const char *Format, ...)
{
	__asm
	{
		/*mov eax, [esp]
		push eax
		push addrfmt
		call printf
		add esp, 8*/
		jmp SA2DebugOutput_i
	}
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
			printf("Replaced file: \"%s\" = \"%s\"\n", origfile.c_str(), buf);
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
		code.pointer = (t & 0x80) == 0x80;
		code.type = (CodeType)(t & 0x7F);
		stream.read((char *)&code.address, sizeof(void *));
		if (code.pointer)
		{
			code.offsetcount = stream.get();
			code.offsets = new int[code.offsetcount];
			for (int i = 0; i < code.offsetcount; i++)
				stream.read((char *)&code.offsets[i], sizeof(int32_t));
		}
		stream.read((char *)&code.value, sizeof(uint32_t));
		if (code.type >= ifeq8 && code.type <= ifkbkey)
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

void __cdecl InitMods(void)
{
	datadllhandle = LoadLibrary(L".\\resource\\gd_PC\\DLL\\Win32\\Data_DLL_orig.dll");
	if (!datadllhandle)
	{
		MessageBox(NULL, L"Data_DLL_orig.dll could not be loaded!\n\nSA2 will now proceed to abruptly exit.", L"SA2 Mod Loader", MB_ICONERROR);
		ExitProcess(1);
	}
	HookTheAPI();
	ifstream str = ifstream("mods\\SA2ModLoader.ini");
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
	string item = settings["ShowConsole"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	bool console = false;
	if (item == "true")
	{
		AllocConsole();
		SetConsoleTitle(L"SA2 Mod Loader output");
		freopen("CONOUT$", "wb", stdout);
		console = true;
		printf("SA2 Mod Loader version %d, built %s\n", ModLoaderVer, __TIMESTAMP__);
		printf("Loading mods...\n");
	}
	item = settings["ShowSA2DebugOutput"];
	transform(item.begin(), item.end(), item.begin(), ::tolower);
	if (item == "true")
		WriteJump((void *)0x426740, SA2DebugOutput);
	InitializeCriticalSection(&filereplacesection);
	DWORD oldprot;
	VirtualProtect((void *)0x87342C, 0xA3BD4, PAGE_WRITECOPY, &oldprot);
	char key[8];
	for (int i = 1; i < 999; i++)
	{
		sprintf_s(key, "Mod%d", i);
		if (settings.find(key) == settings.end())
			break;
		string dir = "mods\\" + settings[key];
		str = ifstream(dir + "\\mod.ini");
		if (!str.is_open())
		{
			if (console)
				printf("Could not open file mod.ini in \"mods\\%s\".\n", settings[key].c_str());
			continue;
		}
		IniDictionary modini = LoadINI(str);
		IniGroup modinfo = modini[""].Element;
		if (console)
			printf("%d. %s\n", i, modinfo["Name"].c_str());
		IniDictionary::iterator gr = modini.find("IgnoreFiles");
		if (gr != modini.end())
		{
			IniGroup replaces = gr->second.Element;
			for (IniGroup::iterator it = replaces.begin(); it != replaces.end(); it++)
			{
				filemap[NormalizePath(it->first)] = "nullfile";
				if (console)
					printf("Ignored file: %s\n", it->first.c_str());
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
				ModInfo *info = (ModInfo *)GetProcAddress(module, "SA2ModInfo");
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
							WriteData(info->Pointers[i].address, &info->Pointers[i].data, sizeof(void*));
					if (info->Version >= 2)
						if (info->Exports)
							for (int i = 0; i < info->ExportCount; i++)
								dataoverrides[info->Exports[i].name] = info->Exports[i].data;
					if (info->Init)
						info->Init(dir.c_str());
				}
				else if (console)
					printf("File \"%s\" is not a valid mod file.\n", filename.c_str());
			}
			else if (console)
				printf("Failed loading file \"%s\".\n", filename.c_str());
		}
	}
	printf("Mod loading finished.\n");
	str = ifstream("mods\\Codes.dat", ifstream::binary);
	if (str.is_open())
	{
		int32_t codecount;
		str.read((char *)&codecount, sizeof(int32_t));
		printf("Loading %d codes...\n", codecount);
		ReadCodes(str, codes);
	}
	str.close();
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