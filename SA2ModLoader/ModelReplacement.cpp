#include "stdafx.h"
#include <UsercallFunctionHandler.h>
#include <Shlwapi.h>
#include <IniFile.hpp>
#include "ModelInfo.h"
// Standard
#include <string>
#include <algorithm> // for std::transform
#include <fstream>
#include <vector>
#include <sstream>
#include "ModelReplacement.h"
#include <FileSystem.h>

using namespace std;
static const string resourcedir = "resource\\gd_pc\\";

VoidFunc(sub_4297F0, 0x4297F0);
FunctionPointer(void, sub_48FA80, (NJS_OBJECT*, void*), 0x48FA80);
StdcallFunctionPointer(void, sub_419FC0, (void*), 0x419FC0);
FunctionPointer(void, sub_7A5974, (void*), 0x7A5974);
DataPointer(int, dword_1A55800, 0x1A55800);
DataPointer(int, dword_1AF191C, 0x1AF191C);
DataPointer(void*, dword_1AF1918, 0x1AF1918);

UsercallFunc(ModelIndex*, LoadMDLFile_t, (const char* filename), (filename), 0x459590, rEAX, rEAX);

struct MdlReplaceData
{
	string iniFile;
	int mod_index;
	string path;
};

unordered_map<ModelIndex*, list<ModelInfo>> modelfiles;
static unordered_map<string, unordered_map<string, vector<MdlReplaceData>>*> replaceMdl_cache;
unordered_map<ModelIndex*, list<ModelInfo>> replaceModelfiles;

void markobjswapped(NJS_OBJECT* obj)
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


void mdlpack::ScanModelReplaceFolder(const string& srcPath, int modIndex)
{
	if (srcPath.size() > MAX_PATH - 7)
		return;

	WIN32_FIND_DATAA data;
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "%s\\*", srcPath.c_str());
	auto hFind = FindFirstFileA(path, &data);
	string lower = srcPath;
	transform(lower.begin(), lower.end(), lower.begin(), tolower);

	// No files found.
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		// NOTE: This will hide *all* files starting with '.'.
		// SA2 doesn't use any files starting with '.',
		// so this won't cause any problems.

		if (data.cFileName[0] == '.')
		{
			continue;
		}

		const string fileName = string(data.cFileName);

		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			string original = fileName;
			transform(original.begin(), original.end(), original.begin(), ::tolower);

			string mdlPack = srcPath + '\\' + fileName;
			transform(mdlPack.begin(), mdlPack.end(), mdlPack.begin(), ::tolower);

			string nameNoExt = GetBaseName(mdlPack);
			auto indexPath = mdlPack + "\\" + nameNoExt + ".ini";

			if (!FileExists(indexPath))
			{
				continue;
			}

			unordered_map<string, vector<MdlReplaceData>>* mdldata;
			auto& iter = replaceMdl_cache.find(original);
			if (iter == replaceMdl_cache.end())
			{
				mdldata = new unordered_map<string, vector<MdlReplaceData>>;
				replaceMdl_cache.insert({ original, mdldata });
			}
			else
			{
				mdldata = iter->second;
			}

			transform(nameNoExt.begin(), nameNoExt.end(), nameNoExt.begin(), tolower);
			(*mdldata)[nameNoExt].push_back({ indexPath, modIndex, mdlPack });
		}
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);
}

static void GetReplacedModel(const char* filename)
{
	auto mdl_name = GetBaseName(filename);
	StripExtension(mdl_name);
	transform(mdl_name.begin(), mdl_name.end(), mdl_name.begin(), ::tolower);
	const auto& repiter = replaceMdl_cache.find(mdl_name);

	if (repiter != replaceMdl_cache.cend())
	{
		list<ModelInfo> files;
		vector<ModelIndex> modelindexes;

		for (const auto& iter2 : *repiter->second)
		{
			for (const auto& vec : iter2.second)
			{
				WIN32_FIND_DATAA data;
				HANDLE hfind = FindFirstFileA((string(vec.path) + "\\*.sa2mdl").c_str(), &data);

				if (hfind == INVALID_HANDLE_VALUE)
					continue;

				unique_ptr<IniFile> ini(new IniFile(vec.iniFile));
				const IniGroup* indexes = ini->getGroup("");

				do
				{
					string combinedPath = vec.path + "\\" + data.cFileName;
					ModelInfo modelfile(combinedPath);
					files.push_back(modelfile);
					markobjswapped(modelfile.getmodel());

					for (auto i = indexes->cbegin(); i != indexes->cend(); i++)
					{
						void* found = modelfile.getdata(i->second);
						if (found != nullptr)
						{
							int ind = stoi(i->first);
							ModelIndex index = { ind, (NJS_OBJECT*)found };

							if (ind >= 0 && ind < 532 && !CharacterModels[ind].Model)
							{
								PrintDebug("Replaced Model %s\n", i->second.c_str());
								CharacterModels[ind] = index;
							}

							modelindexes.push_back(index);
						}
					}
				} while (FindNextFileA(hfind, &data) != 0);


				FindClose(hfind);
			}
		}

		ModelIndex endmarker = { -1, (NJS_OBJECT*)-1 };
		modelindexes.push_back(endmarker);
		ModelIndex* result = new ModelIndex[modelindexes.size()];
		memcpy(result, modelindexes.data(), sizeof(ModelIndex) * modelindexes.size());
		replaceModelfiles[result] = files;
	}
}

void BuildIniPath(char* dir, char* combinedpath, const char* filename);
ModelIndex* __cdecl LoadMDLFile_ri(const char* filename)
{
	GetReplacedModel(filename); //check for individual file replacement

	ModelIndex* result = nullptr;
	char dir[MAX_PATH];
	char combinedpath[MAX_PATH];

	BuildIniPath(dir, combinedpath, filename);

	const char* repfn = sadx_fileMap.replaceFile(combinedpath);

	//check for folder that does full models replacement
	if (PathFileExistsA(repfn) == false)  //it not found, let the game load the original models 
		return LoadMDLFile_t.Original((char*)filename);


	FILE* f_mod_ini = fopen(repfn, "r");
	unique_ptr<IniFile> ini(new IniFile(f_mod_ini));
	fclose(f_mod_ini);
	const IniGroup* indexes = ini->getGroup("");
	strncpy_s(dir, repfn, MAX_PATH);
	PathRemoveFileSpecA(dir);
	WIN32_FIND_DATAA data;
	HANDLE hfind = FindFirstFileA((string(dir) + "\\*.sa2mdl").c_str(), &data);

	if (hfind == INVALID_HANDLE_VALUE)
		return LoadMDLFile_t.Original((char*)filename);

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
			void* found = modelfile.getdata(i->second);
			if (found != nullptr)
			{
				int ind = stoi(i->first);
				ModelIndex index = { ind, (NJS_OBJECT*)found };

				if (ind >= 0 && ind < 532 && !CharacterModels[ind].Model)
					CharacterModels[ind] = index;

				modelindexes.push_back(index);
			}
		}
	} while (FindNextFileA(hfind, &data) != 0);

	FindClose(hfind);

	ModelIndex endmarker = { -1, (NJS_OBJECT*)-1 };
	modelindexes.push_back(endmarker);
	result = new ModelIndex[modelindexes.size()];
	memcpy(result, modelindexes.data(), sizeof(ModelIndex) * modelindexes.size());
	modelfiles[result] = files;

	--dword_1A55800;
	if (dword_1AF191C)
	{
		auto v13 = dword_1AF1918;
		auto v14 = *((DWORD*)dword_1AF1918 + 1);
		auto v15 = *((DWORD*)dword_1AF1918 + 1);
		if (!*(BYTE*)(v14 + 21))
		{
			do
			{
				sub_419FC0(*(void**)(v15 + 8));
				v15 = *(DWORD*)v15;
				sub_7A5974((void*)v14);
				v14 = v15;
			} while (!*(BYTE*)(v15 + 21));
			v13 = dword_1AF1918;
		}
		*((DWORD*)v13 + 1) = (DWORD)v13;
		dword_1AF191C = 0;
		*(DWORD*)dword_1AF1918 = (DWORD)dword_1AF1918;
		*((DWORD*)dword_1AF1918 + 2) = (DWORD)dword_1AF1918;
	}

	return result;
}

void __cdecl ReleaseMDLFile_ri(ModelIndex* mdl)
{
	if (mdl->Index != -1)
	{
		ModelIndex* curMdl = mdl;
		do
		{
			if (curMdl->Index >= 0 && curMdl->Index < 532 && CharacterModels[curMdl->Index].Model == curMdl->Model)
				CharacterModels[curMdl->Index].Model = 0;

			for (auto it = replaceModelfiles.begin(); it != replaceModelfiles.end();) //free memory from individual file replacement
			{
				if (curMdl->Index == it->first->Index && curMdl->Model != it->first->Model)
				{
					delete it->first;
					it = replaceModelfiles.erase(it);
				}
				else
				{
					++it;
				}
			}

			++curMdl;

		} while (curMdl->Index != -1);
	}

	if (modelfiles.find(mdl) != modelfiles.cend())
	{
		modelfiles.erase(mdl);
		delete[] mdl;
	}
	else
	{
		FreeMemory((int*)mdl, (char*)"..\\..\\src\\file_ctl.c", 1091);
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

void mdlpack::init()
{
	LoadMDLFile_t.Hook(LoadMDLFile_ri);
	WriteJump((void*)ReleaseMDLFilePtr, ReleaseMDLFile_r);
}