#include "stdafx.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include "IniFile.hpp"
#include "TextConv.hpp"
#include "SA2ModLoader.h"
#include "LandTableInfo.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "DLLData.h"
#include <DbgHelp.h>

using std::unordered_map;
using std::vector;
using std::string;
using std::wstring;

static unordered_map<string, void*> dlllabels;
static unordered_map<NJS_TEXLIST*, NJS_TEXLIST*> dlltexlists;

static vector<string>& split(const string& s, char delim, vector<string>& elems)
{
	std::stringstream ss(s);
	string item;

	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}

	return elems;
}


static vector<string> split(const string& s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

template<typename T>
static inline T* arrcpy(T* dst, const T* src, size_t cnt)
{
	return (T*)memcpy(dst, src, cnt * sizeof(T));
}

template<typename T>
static inline void clrmem(T* mem)
{
	ZeroMemory(mem, sizeof(T));
}

static inline void HookExport(LPCSTR exportName, const void* newdata)
{
	intptr_t hModule = (intptr_t)**datadllhandle;
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

static void LoadDLLLandTable(const wstring& path)
{
	auto info = new LandTableInfo(path);
	LandTable* land = info->getlandtable();

	auto tl = dlltexlists.find(land->TextureList);
	if (tl != dlltexlists.end())
	{
		land->TextureList = tl->second;
	}

	auto labels = info->getlabels();
	for (const auto& label : *labels)
	{
		dlllabels[label.first] = label.second;
	}
}

static void LoadDLLModel(const wstring& path)
{
	auto info = new ModelInfo(path);
	auto labels = info->getlabels();

	for (const auto& label : *labels)
	{
		dlllabels[label.first] = label.second;
	}
}

static void LoadDLLAnimation(const wstring& path)
{
	auto info = new AnimationFile(path);
	dlllabels[info->getlabel()] = info->getmotion();
}

using dllfilefunc_t = void(__cdecl*)(const wstring & path);
static const unordered_map<string, dllfilefunc_t> dllfilefuncmap = {
	{ "landtable",    LoadDLLLandTable },
	{ "model",        LoadDLLModel },
	{ "basicdxmodel", LoadDLLModel },
	{ "chunkmodel",   LoadDLLModel },
	{ "animation",    LoadDLLAnimation },
};

static void ProcessLandTableDLL(const IniGroup* group, void* exp)
{
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(LandTable));
}

static void ProcessLandTableArrayDLL(const IniGroup* group, void* exp)
{
	((LandTable**)exp)[group->getInt("Index")] = (LandTable*)dlllabels[group->getString("Label")];
}

static void ProcessModelDLL(const IniGroup* group, void* exp)
{
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(NJS_OBJECT));
}

static void ProcessModelArrayDLL(const IniGroup* group, void* exp)
{
	((NJS_OBJECT**)exp)[group->getInt("Index")] = (NJS_OBJECT*)dlllabels[group->getString("Label")];
}

static void ProcessModelsArrayDLL(const IniGroup* group, void* exp)
{
	((NJS_MODEL_SADX**)exp)[group->getInt("Index")] = (NJS_MODEL_SADX*)dlllabels[group->getString("Label")];
}

static void ProcessActionArrayDLL(const IniGroup* group, void* exp)
{
	string field = group->getString("Field");
	NJS_ACTION* act = ((NJS_ACTION**)exp)[group->getInt("Index")];

	if (field == "object")
	{
		act->object = (NJS_OBJECT*)dlllabels[group->getString("Label")];
	}
	else if (field == "motion")
	{
		act->motion = (NJS_MOTION*)dlllabels[group->getString("Label")];
	}
}

using dllitemfunc_t = void(__cdecl*)(const IniGroup * group, void* exp);
static const unordered_map<string, dllitemfunc_t> dllitemfuncmap = {
	{ "landtable",         ProcessLandTableDLL },
	{ "battlelandtable",         ProcessLandTableDLL },
	{ "landtablearray",    ProcessLandTableArrayDLL },
	{ "model",             ProcessModelDLL },
	{ "modelarray",        ProcessModelArrayDLL },
	{ "chunkmodel",        ProcessModelDLL },
	{ "chunkmodelarray",   ProcessModelArrayDLL },
	{ "modelsarray",       ProcessModelsArrayDLL },
	{ "actionarray",       ProcessActionArrayDLL },
};

static void ProcessAnimIndexListDLL(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s\\*.saanim",
		mod_dir.c_str(), group->getWString("filename").c_str());
	WIN32_FIND_DATA data;

	HANDLE hFind = FindFirstFile(filename, &data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	vector<AnimationIndex> anims;
	do
	{
		wchar_t* end;
		auto ind = wcstol(data.cFileName, &end, 10);
		if (end == data.cFileName) continue; // filename was not a number
		swprintf(filename, LengthOfArray(filename), L"%s\\%s\\%s",
			mod_dir.c_str(), group->getWString("filename").c_str(), data.cFileName);
		auto animfile = new AnimationFile(filename);
		AnimationIndex entry{ (uint16_t)ind, (uint16_t)animfile->getmodelcount(), animfile->getmotion() };
		anims.push_back(entry);
	} while (FindNextFile(hFind, &data));
	auto numents = anims.size();
	AnimationIndex* list = new AnimationIndex[numents + 1];
	arrcpy(list, anims.data(), numents);
	memset(&list[numents], -1, sizeof(AnimationIndex));
	HookExport(group->getString("export").c_str(), list);
}

static void ProcessCharaObjectDataListDLL(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	unordered_map<string, void*> labels;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s\\*.sa2mdl",
		mod_dir.c_str(), group->getWString("filename").c_str());
	WIN32_FIND_DATA fdata;

	HANDLE hFind = FindFirstFile(filename, &fdata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		swprintf(filename, LengthOfArray(filename), L"%s\\%s\\%s",
			mod_dir.c_str(), group->getWString("filename").c_str(), fdata.cFileName);
		auto mdllbl = (new ModelInfo(filename))->getlabels();
		for (auto iter = mdllbl->cbegin(); iter != mdllbl->cend(); ++iter)
			labels[iter->first] = iter->second;
	} while (FindNextFile(hFind, &fdata));

	swprintf(filename, LengthOfArray(filename), L"%s\\%s\\*.saanim",
		mod_dir.c_str(), group->getWString("filename").c_str());

	hFind = FindFirstFile(filename, &fdata);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		swprintf(filename, LengthOfArray(filename), L"%s\\%s\\%s",
			mod_dir.c_str(), group->getWString("filename").c_str(), fdata.cFileName);
		auto anim = new AnimationFile(filename);
		labels[anim->getlabel()] = anim->getmotion();
	} while (FindNextFile(hFind, &fdata));
	swprintf(filename, LengthOfArray(filename), L"%s\\%s\\info.ini",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile* const data = new IniFile(filename);
	vector<CharaObjectData> chars;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!data->hasGroup(key)) break;
		const IniGroup* chrdata = data->getGroup(key);
		CharaObjectData entry{};
		entry.MainModel = (NJS_OBJECT*)labels[chrdata->getString("MainModel")];
		entry.Animation1 = (NJS_MOTION*)labels[chrdata->getString("Animation1")];
		entry.Animation2 = (NJS_MOTION*)labels[chrdata->getString("Animation2")];
		entry.Animation3 = (NJS_MOTION*)labels[chrdata->getString("Animation3")];
		if (chrdata->hasKeyNonEmpty("AccessoryModel"))
		{
			entry.AccessoryModel = (NJS_OBJECT*)labels[chrdata->getString("AccessoryModel")];
			entry.AccessoryAttachNode = (NJS_OBJECT*)labels[chrdata->getString("AccessoryAttachNode")];
		}
		if (chrdata->hasKeyNonEmpty("SuperModel"))
		{
			entry.SuperModel = (NJS_OBJECT*)labels[chrdata->getString("SuperModel")];
			entry.SuperAnimation1 = (NJS_MOTION*)labels[chrdata->getString("SuperAnimation1")];
			entry.SuperAnimation2 = (NJS_MOTION*)labels[chrdata->getString("SuperAnimation2")];
			entry.SuperAnimation3 = (NJS_MOTION*)labels[chrdata->getString("SuperAnimation3")];
		}
		entry.anonymous_9 = chrdata->getInt("Unknown1");
		entry.anonymous_10 = chrdata->getInt("Unknown2");
		entry.anonymous_11 = chrdata->getInt("Unknown3");
		entry.anonymous_12 = chrdata->getInt("Unknown4");
		entry.anonymous_13 = chrdata->getFloat("Unknown5");
		chars.push_back(entry);
	}
	delete data;
	auto numents = chars.size();
	auto list = new CharaObjectData[numents];
	arrcpy(list, chars.data(), numents);
	HookExport(group->getString("export").c_str(), list);
}

typedef void(__cdecl* dlldatafunc_t)(const IniGroup* group, const wstring& mod_dir);
static const unordered_map<string, dlldatafunc_t> dlldatafuncmap = {
	{ "animindexlist", ProcessAnimIndexListDLL },
	{ "charaobjectdatalist", ProcessCharaObjectDataListDLL },
};

struct dllexportinfo { void* address = nullptr; string type; };
static unordered_map<string, dllexportinfo> dllexports;

void ProcessDLLData(const wchar_t* filename, const wstring& mod_dir)
{
	auto dlldata = new IniFile(filename);
	IniGroup* group;

	if (dllexports.empty())
	{
		group = dlldata->getGroup("Exports");
		for (const auto& iter : *group)
		{
			dllexportinfo inf;
			inf.address = GetProcAddress(**datadllhandle, iter.first.c_str());
			inf.type = iter.second;
			dllexports[iter.first] = inf;
		}
	}

	dlltexlists.clear();

	if (dlldata->hasGroup("TexLists"))
	{
		group = dlldata->getGroup("TexLists");

		for (const auto& iter : *group)
		{
			auto* key = (NJS_TEXLIST*)std::stoul(iter.first, nullptr, 16);
			vector<string> valstr = split(iter.second, ',');
			NJS_TEXLIST* value;

			if (valstr.size() > 1)
			{
				value = ((NJS_TEXLIST**)dllexports[valstr[0]].address)[std::stoul(valstr[1])];
			}
			else
			{
				value = (NJS_TEXLIST*)dllexports[valstr[0]].address;
			}

			dlltexlists[key] = value;
		}
	}

	dlllabels.clear();
	group = dlldata->getGroup("Files");

	for (const auto& iter : *group)
	{
		auto type = dllfilefuncmap.find(split(iter.second, '|')[0]);

		if (type != dllfilefuncmap.end())
		{
			type->second(mod_dir + L'\\' + MBStoUTF16(iter.first, CP_UTF8));
		}
	}

	char buf[16]{};

	for (unsigned int k = 1; k < 9999; k++)
	{
		snprintf(buf, sizeof(buf), "Item%u", k);

		if (!dlldata->hasGroup(buf))
		{
			continue;
		}

		group = dlldata->getGroup(buf);
		const dllexportinfo& exp = dllexports[group->getString("Export")];
		auto type = dllitemfuncmap.find(exp.type);

		if (type != dllitemfuncmap.end())
		{
			type->second(group, exp.address);
		}
	}

	for (unsigned int k = 1; k < 9999; k++)
	{
		snprintf(buf, sizeof(buf), "DataItem%u", k);

		if (!dlldata->hasGroup(buf))
		{
			continue;
		}

		group = dlldata->getGroup(buf);
		auto typestr = group->getString("type");
		auto type = dlldatafuncmap.find(typestr);
		if (type != dlldatafuncmap.end())
			type->second(group, mod_dir);
		else
			PrintDebug("Handler for INI data type \"%s\" not found!", typestr.c_str());
	}

	delete dlldata;
}