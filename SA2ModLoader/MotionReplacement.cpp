#include "stdafx.h"
#include <UsercallFunctionHandler.h>
#include <Shlwapi.h>
#include <IniFile.hpp>
#include "MotionReplacement.h"
// Standard
#include <string>
#include <algorithm> // for std::transform
#include <fstream>
#include <vector>
#include <sstream>
#include <FileSystem.h>
#include "AnimationFile.h"

using namespace std;
static const string resourcedir = "resource\\gd_pc\\";

UsercallFunc(AnimationIndex*, LoadMTNFile_t, (const char* filename), (filename), 0x459740, rEAX, rEAX);

struct MtnReplaceData
{
	string iniFile;
	int mod_index;
	string path;
};

unordered_map<AnimationIndex*, list<AnimationFile>> animfiles;
static unordered_map<string, unordered_map<string, vector<MtnReplaceData>>*> replaceMtn_cache;
unordered_map<AnimationIndex*, list<AnimationFile>> replaceanimfiles;

void markanimswapped(NJS_MOTION* mtn)
{
	IsByteswapped(&mtn->mdata);
	IsByteswapped(&mtn->inp_fn);
	IsByteswapped(&mtn->nbFrame);
	IsByteswapped(&mtn->type);
}

static void GetReplacedMotion(const char* filename)
{
	auto mtn_name = GetBaseName(filename);
	StripExtension(mtn_name);
	transform(mtn_name.begin(), mtn_name.end(), mtn_name.begin(), ::tolower);
	const auto& repiter = replaceMtn_cache.find(mtn_name);

	if (repiter != replaceMtn_cache.cend())
	{
		list<AnimationFile> files;
		vector<AnimationIndex> animindexes;

		for (const auto& iter2 : *repiter->second)
		{
			for (const auto& vec : iter2.second)
			{
				WIN32_FIND_DATAA data;
				HANDLE hfind = FindFirstFileA((string(vec.path) + "\\*.saanim").c_str(), &data);

				if (hfind == INVALID_HANDLE_VALUE)
					continue;

				unique_ptr<IniFile> ini(new IniFile(vec.iniFile));
				const IniGroup* indexes = ini->getGroup("");

				do
				{
					string combinedPath = vec.path + "\\" + data.cFileName;
					AnimationFile animfile(combinedPath);
					files.push_back(animfile);
					markanimswapped(animfile.getmotion());

					for (auto i = indexes->cbegin(); i != indexes->cend(); i++)
					{
						void* found = animfile.getdata(i->second);
						if (found != nullptr)
						{
							int ind = stoi(i->first);
							AnimationIndex index = { ind, animfile.getmodelcount(), (NJS_MOTION*)found };

							if (ind >= 0 && ind < 300 && !CharacterAnimations[ind].Animation)
								CharacterAnimations[ind] = index;

							animindexes.push_back(index);
						}
					}
				} while (FindNextFileA(hfind, &data) != 0);
				FindClose(hfind);
			}


			AnimationIndex endmarker = { -1, -1, (NJS_MOTION*)-1 };
			animindexes.push_back(endmarker);
			AnimationIndex* result = new AnimationIndex[animindexes.size()];
			memcpy(result, animindexes.data(), sizeof(AnimationIndex) * animindexes.size());
			replaceanimfiles[result] = files;
		}
	}
}

void BuildIniPath(char* dir, char* combinedpath, const char* filename);
AnimationIndex* LoadMTNFile_r(const char* filename)
{
	GetReplacedMotion(filename);

	AnimationIndex* result = nullptr;

	char dir[MAX_PATH];
	char combinedpath[MAX_PATH];

	BuildIniPath(dir, combinedpath, filename);

	const char* repfn = sadx_fileMap.replaceFile(combinedpath);

	//check for folder that does full anims replacement
	if (PathFileExistsA(repfn) == false)  //it not found, let the game load the original anims
		return LoadMTNFile_t.Original((char*)filename);

	FILE* f_mod_ini = fopen(repfn, "r");
	unique_ptr<IniFile> ini(new IniFile(f_mod_ini));
	fclose(f_mod_ini);
	const IniGroup* indexes = ini->getGroup("");
	strncpy_s(dir, repfn, MAX_PATH);
	PathRemoveFileSpecA(dir);
	WIN32_FIND_DATAA data;
	HANDLE hfind = FindFirstFileA((string(dir) + "\\*.saanim").c_str(), &data);

	if (hfind == INVALID_HANDLE_VALUE)
		return LoadMTNFile_t.Original((char*)filename);

	list<AnimationFile> files;
	vector<AnimationIndex> animindexes;

	do
	{
		PathCombineA(combinedpath, dir, data.cFileName);
		AnimationFile animfile(combinedpath);
		files.push_back(animfile);
		markanimswapped(animfile.getmotion());

		for (auto i = indexes->cbegin(); i != indexes->cend(); i++)
		{
			void* found = animfile.getdata(i->second);
			if (found != nullptr)
			{
				int ind = stoi(i->first);
				AnimationIndex index = { ind, animfile.getmodelcount(), (NJS_MOTION*)found };

				if (ind >= 0 && ind < 300 && !CharacterAnimations[ind].Animation)
					CharacterAnimations[ind] = index;

				animindexes.push_back(index);
			}
		}
	} while (FindNextFileA(hfind, &data) != 0);
	FindClose(hfind);

	AnimationIndex endmarker = { -1, -1, (NJS_MOTION*)-1 };
	animindexes.push_back(endmarker);
	result = new AnimationIndex[animindexes.size()];
	memcpy(result, animindexes.data(), sizeof(AnimationIndex) * animindexes.size());
	animfiles[result] = files;

	return result;
}

void __cdecl ReleaseMTNFile_ri(AnimationIndex* anim)
{
	int index = 0;

	if (anim->Index != 0xFFFF)
	{
		auto curAnim = anim;

		do
		{
			auto curIndex = curAnim->Index;

			if (curAnim->Index >= 0 && curIndex < 300 && CharacterAnimations[curIndex].Animation == curAnim->Animation)
			{
				CharacterAnimations[curAnim->Index].Animation = 0;
				CharacterAnimations[curAnim->Index].Count = 0;
			}


			for (auto it = replaceanimfiles.begin(); it != replaceanimfiles.end();) //free memory from individual file replacement
			{
				if (curAnim->Index == it->first->Index && curAnim->Animation != it->first->Animation)
				{
					delete it->first;
					it = replaceanimfiles.erase(it);
				}
				else
				{
					++it;
				}
			}

			curAnim = &anim[++index];
		} while (curAnim->Index != 0xFFFF);
	}

	if (animfiles.find(anim) != animfiles.cend())
	{
		animfiles.erase(anim);
		delete[] anim;
	}
	else
	{
		FreeMemory((int*)anim, (char*)"..\\..\\src\\file_ctl.c", 1091);
	}
}

static void __declspec(naked) ReleaseAnimASM()
{
	__asm
	{
		push esi
		call ReleaseMTNFile_ri
		pop esi
		retn
	}
}

void mtnpack::ScanMotionReplaceFolder(const string& srcPath, int modIndex)
{
	if (srcPath.size() > MAX_PATH - 12)
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

			string mtnPack = srcPath + '\\' + fileName;
			transform(mtnPack.begin(), mtnPack.end(), mtnPack.begin(), ::tolower);

			string nameNoExt = GetBaseName(mtnPack);
			auto indexPath = mtnPack + "\\" + nameNoExt + ".ini";

			if (!FileExists(indexPath))
			{
				continue;
			}

			unordered_map<string, vector<MtnReplaceData>>* mtndata;
			auto& iter = replaceMtn_cache.find(original);
			if (iter == replaceMtn_cache.end())
			{
				mtndata = new unordered_map<string, vector<MtnReplaceData>>;
				replaceMtn_cache.insert({ original, mtndata });
			}
			else
			{
				mtndata = iter->second;
			}

			transform(nameNoExt.begin(), nameNoExt.end(), nameNoExt.begin(), tolower);
			(*mtndata)[nameNoExt].push_back({ indexPath, modIndex, mtnPack });
		}
	} while (FindNextFileA(hFind, &data) != 0);

	FindClose(hFind);
}

void mtnpack::init()
{
	LoadMTNFile_t.Hook(LoadMTNFile_r);
	WriteJump((void*)UnloadAnimPtr, ReleaseAnimASM);
}