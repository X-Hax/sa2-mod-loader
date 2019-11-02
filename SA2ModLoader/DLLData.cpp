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
//#include "DLLData.h"

using std::unordered_map;
using std::vector;
using std::string;
using std::wstring;

static unordered_map<string, void*> dlllabels;
static unordered_map<NJS_TEXLIST*, NJS_TEXLIST*> dlltexlists;

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
	{ "animindex",    LoadDLLAnimation },
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

using dlldatafunc_t = void(__cdecl*)(const IniGroup * group, void* exp);

static const unordered_map<string, dlldatafunc_t> dlldatafuncmap = {
	{ "landtable",         ProcessLandTableDLL },
	{ "landtablearray",    ProcessLandTableArrayDLL },
	{ "model",             ProcessModelDLL },
	{ "modelarray",        ProcessModelArrayDLL },
	{ "chunkmodel",        ProcessModelDLL },
	{ "chunkmodelarray",   ProcessModelArrayDLL },
	{ "modelsarray",       ProcessModelsArrayDLL },
	{ "actionarray",       ProcessActionArrayDLL },
};

static HMODULE dllhandle;

struct dllexportinfo { void* address = nullptr; string type; };
static unordered_map<string, dllexportinfo> dllexports;

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
			inf.address = GetProcAddress(dllhandle, iter.first.c_str());
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

	for (unsigned int k = 0; k < 9999; k++)
	{
		snprintf(buf, sizeof(buf), "Item%u", k);

		if (!dlldata->hasGroup(buf))
		{
			continue;
		}

		group = dlldata->getGroup(buf);
		const dllexportinfo& exp = dllexports[group->getString("Export")];
		auto type = dlldatafuncmap.find(exp.type);

		if (type != dlldatafuncmap.end())
		{
			type->second(group, exp.address);
		}
	}

	delete dlldata;
}

void SetDLLHandle(HMODULE handle)
{
	dllhandle = handle;
}