#include "stdafx.h"
#include <Shlwapi.h>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "IniFile.hpp"
#include "TextConv.hpp"
#include "SA2ModLoader.h"
#include "LandTableInfo.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "EXEData.h"
#include <FileSystem.h>

using std::unordered_map;
using std::vector;
using std::string;
using std::wstring;
using std::ifstream;

static vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	std::stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


static vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

static string trim(const string &s)
{
	auto st = s.find_first_not_of(' ');
	if (st == string::npos)
		st = 0;
	auto ed = s.find_last_not_of(' ');
	if (ed == string::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

template<typename T>
static inline T *arrcpy(T *dst, const T *src, size_t cnt)
{
	return (T *)memcpy(dst, src, cnt * sizeof(T));
}

template<typename T>
static inline void clrmem(T *mem)
{
	ZeroMemory(mem, sizeof(T));
}

static const unordered_map<string, uint8_t> levelidsnamemap = {
	{ "basictest", LevelIDs_BasicTest },
	{ "knucklestest", LevelIDs_KnucklesTest },
	{ "sonictest", LevelIDs_SonicTest },
	{ "greenforest", LevelIDs_GreenForest },
	{ "whitejungle", LevelIDs_WhiteJungle },
	{ "pumpkinhill", LevelIDs_PumpkinHill },
	{ "skyrail", LevelIDs_SkyRail },
	{ "aquaticmine", LevelIDs_AquaticMine },
	{ "securityhall", LevelIDs_SecurityHall },
	{ "prisonlane", LevelIDs_PrisonLane },
	{ "metalharbor", LevelIDs_MetalHarbor },
	{ "irongate", LevelIDs_IronGate },
	{ "weaponsbed", LevelIDs_WeaponsBed },
	{ "cityescape", LevelIDs_CityEscape },
	{ "radicalhighway", LevelIDs_RadicalHighway },
	{ "weaponsbed2p", LevelIDs_WeaponsBed2P },
	{ "wildcanyon", LevelIDs_WildCanyon },
	{ "missionstreet", LevelIDs_MissionStreet },
	{ "drylagoon", LevelIDs_DryLagoon },
	{ "sonicvsshadow1", LevelIDs_SonicVsShadow1 },
	{ "tailsvseggman1", LevelIDs_TailsVsEggman1 },
	{ "sandocean", LevelIDs_SandOcean },
	{ "crazygadget", LevelIDs_CrazyGadget },
	{ "hiddenbase", LevelIDs_HiddenBase },
	{ "eternalengine", LevelIDs_EternalEngine },
	{ "deathchamber", LevelIDs_DeathChamber },
	{ "eggquarters", LevelIDs_EggQuarters },
	{ "lostcolony", LevelIDs_LostColony },
	{ "pyramidcave", LevelIDs_PyramidCave },
	{ "tailsvseggman2", LevelIDs_TailsVsEggman2 },
	{ "finalrush", LevelIDs_FinalRush },
	{ "greenhill", LevelIDs_GreenHill },
	{ "meteorherd", LevelIDs_MeteorHerd },
	{ "knucklesvsrouge", LevelIDs_KnucklesVsRouge },
	{ "cannonscores", LevelIDs_CannonsCoreS },
	{ "cannonscoree", LevelIDs_CannonsCoreE },
	{ "cannonscoret", LevelIDs_CannonsCoreT },
	{ "cannonscorer", LevelIDs_CannonsCoreR },
	{ "cannonscorek", LevelIDs_CannonsCoreK },
	{ "missionstreet2p", LevelIDs_MissionStreet2P },
	{ "finalchase", LevelIDs_FinalChase },
	{ "wildcanyon2p", LevelIDs_WildCanyon2P },
	{ "sonicvsshadow2", LevelIDs_SonicVsShadow2 },
	{ "cosmicwall", LevelIDs_CosmicWall },
	{ "madspace", LevelIDs_MadSpace },
	{ "sandocean2p", LevelIDs_SandOcean2P },
	{ "drylagoon2p", LevelIDs_DryLagoon2P },
	{ "pyramidrace", LevelIDs_PyramidRace },
	{ "hiddenbase2p", LevelIDs_HiddenBase2P },
	{ "poolquest", LevelIDs_PoolQuest },
	{ "planetquest", LevelIDs_PlanetQuest },
	{ "deckrace", LevelIDs_DeckRace },
	{ "downtownrace", LevelIDs_DowntownRace },
	{ "cosmicwall2p", LevelIDs_CosmicWall2P },
	{ "grindrace", LevelIDs_GrindRace },
	{ "lostcolony2p", LevelIDs_LostColony2P },
	{ "eternalengine2p", LevelIDs_EternalEngine2P },
	{ "metalharbor2p", LevelIDs_MetalHarbor2P },
	{ "irongate2p", LevelIDs_IronGate2P },
	{ "deathchamber2p", LevelIDs_DeathChamber2P },
	{ "bigfoot", LevelIDs_BigFoot },
	{ "hotshot", LevelIDs_HotShot },
	{ "flyingdog", LevelIDs_FlyingDog },
	{ "kingboomboo", LevelIDs_KingBoomBoo },
	{ "egggolems", LevelIDs_EggGolemS },
	{ "biolizard", LevelIDs_Biolizard },
	{ "finalhazard", LevelIDs_FinalHazard },
	{ "egggoleme", LevelIDs_EggGolemE },
	{ "route101280", LevelIDs_Route101280 },
	{ "kartrace", LevelIDs_KartRace },
	{ "chaoworld", LevelIDs_ChaoWorld },
	{ "invalid", LevelIDs_Invalid }
};

static uint8_t ParseLevelID(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = levelidsnamemap.find(str2);
	if (lv != levelidsnamemap.end())
		return lv->second;
	else
		return (uint8_t)strtol(str.c_str(), nullptr, 10);
}

static const unordered_map<string, uint8_t> charnamemap = {
	{ "sonic", Characters_Sonic },
	{ "shadow", Characters_Shadow },
	{ "tails", Characters_Tails },
	{ "eggman", Characters_Eggman },
	{ "knuckles", Characters_Knuckles },
	{ "rouge", Characters_Rouge },
	{ "mechtails", Characters_MechTails },
	{ "mecheggman", Characters_MechEggman }
};

static uint8_t ParseCharacter(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto ch = charnamemap.find(str2);
	if (ch != charnamemap.end())
		return ch->second;
	else
		return (uint8_t)strtol(str.c_str(), nullptr, 10);
}

static const unordered_map<string, uint32_t> charflagsnamemap = {
	{ "sonic", CharacterFlags_Sonic },
	{ "shadow", CharacterFlags_Shadow },
	{ "tails", CharacterFlags_Tails },
	{ "eggman", CharacterFlags_Eggman },
	{ "knuckles", CharacterFlags_Knuckles },
	{ "rouge", CharacterFlags_Rouge },
	{ "mechtails", CharacterFlags_MechTails },
	{ "mecheggman", CharacterFlags_MechEggman },
	{ "sand", 0x1000000 },
	{ "water", 0x2000000 }
};

static uint32_t ParseCharacterFlags(const string &str)
{
	vector<string> strflags = split(str, ',');
	uint32_t flag = 0;
	for (auto iter = strflags.cbegin(); iter != strflags.cend(); ++iter)
	{
		string s = trim(*iter);
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		auto ch = charflagsnamemap.find(s);
		if (ch != charflagsnamemap.end())
			flag |= ch->second;
	}
	return flag;
}

static const unordered_map<string, uint8_t> storytypenamemap = {
	{ "event", StoryEntryType_Event },
	{ "level", StoryEntryType_Level },
	{ "end", StoryEntryType_End },
	{ "credits", StoryEntryType_Credits }
};

static uint8_t ParseStoryEntryType(const string& str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto ty = storytypenamemap.find(str2);
	if (ty != storytypenamemap.end())
		return ty->second;
	else
		return (uint8_t)strtol(str.c_str(), nullptr, 10);
}

static const unordered_map<string, uint8_t> languagesnamemap = {
	{ "japanese", 0 },
	{ "english", 1 },
	{ "french", 2 },
	{ "spanish", 3 },
	{ "german", 4 },
	{ "italian", 5 }
};

static uint8_t ParseLanguage(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = languagesnamemap.find(str2);
	if (lv != languagesnamemap.end())
		return lv->second;
	return 0;
}

static string DecodeUTF8(const string &str, int language)
{
	if (language == 0)
		return UTF8toSJIS(str);
	else
		return UTF8to1252(str);
}

static string UnescapeNewlines(const string &str)
{
	string result;
	result.reserve(str.size());
	for (unsigned int c = 0; c < str.size(); c++)
		switch (str[c])
		{
		case '\\': // escape character
			if (c + 1 == str.size())
			{
				result.push_back(str[c]);
				continue;
			}
			switch (str[++c])
			{
			case 'n': // line feed
				result.push_back('\n');
				break;
			case 'r': // carriage return
				result.push_back('\r');
				break;
			default: // literal character
				result.push_back(str[c]);
				break;
			}
			break;
		default:
			result.push_back(str[c]);
			break;
		}
	return result;
}

static void ParseVertex(const string &str, NJS_VECTOR &vert)
{
	vector<string> vals = split(str, ',');
	assert(vals.size() == 3);
	if (vals.size() < 3)
		return;
	vert.x = (float)strtod(vals[0].c_str(), nullptr);
	vert.y = (float)strtod(vals[1].c_str(), nullptr);
	vert.z = (float)strtod(vals[2].c_str(), nullptr);
}

static void ParseRotation(const string str, Rotation &rot)
{
	vector<string> vals = split(str, ',');
	assert(vals.size() == 3);
	if (vals.size() < 3)
		return;
	rot.x = (int)strtol(vals[0].c_str(), nullptr, 16);
	rot.y = (int)strtol(vals[1].c_str(), nullptr, 16);
	rot.z = (int)strtol(vals[2].c_str(), nullptr, 16);
}

static void ParseMinSec(const string str, MinSec& minsec)
{
	vector<string> vals = split(str, ':');
	assert(vals.size() == 2);
	if (vals.size() < 2)
		return;
	minsec.Minutes = (char)strtol(vals[0].c_str(), nullptr, 10);
	minsec.Seconds = (char)strtol(vals[1].c_str(), nullptr, 10);
}

template<typename T>
static void ProcessPointerList(const string &list, T *item)
{
	vector<string> ptrs = split(list, ',');
	for (unsigned int i = 0; i < ptrs.size(); i++)
		WriteData((T **)(strtol(ptrs[i].c_str(), nullptr, 16) + 0x400000), item);
}

static void ProcessLandTableINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	LandTableInfo *const landtableinfo = new LandTableInfo(filename);
	LandTable *const landtable = landtableinfo->getlandtable();
	ProcessPointerList(group->getString("pointer"), landtable);
}

static unordered_map<string, NJS_OBJECT *> inimodels;
static void GetModelLabels(ModelInfo *mdlinf, NJS_OBJECT *obj)
{
	while (obj)
	{
		inimodels[mdlinf->getlabel(obj)] = obj;
		if (obj->child)
			GetModelLabels(mdlinf, obj->child);
		obj = obj->sibling;
	}
}

static void ProcessModelINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address"))
	{
		return;
	}

	wchar_t filename[MAX_PATH]{};

	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());

	auto* const mdlinf = new ModelInfo(filename);
	NJS_OBJECT* newobject = mdlinf->getmodel();

	GetModelLabels(mdlinf, newobject);
	if (group->hasKeyNonEmpty("pointer"))
		ProcessPointerList(group->getString("pointer"), newobject);
	else
	{
		NJS_OBJECT* object = (NJS_OBJECT*)(strtol((group->getString("address")).c_str(), nullptr, 16) + 0x400000);
		if (object->chunkmodel != nullptr)
		{
			*object->chunkmodel = *newobject->chunkmodel;
		}
		*object = *newobject;
	}
}

static void ProcessActionINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	NJS_ACTION *action = new NJS_ACTION;
	AnimationFile *const animationFile = new AnimationFile(filename);
	action->motion = animationFile->getmotion();
	action->object = inimodels.find(group->getString("model"))->second;
	ProcessPointerList(group->getString("pointer"), action);
}

static void ProcessAnimationINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address"))
	{
		return;
	}

	wchar_t filename[MAX_PATH]{};

	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());

	auto* const animationFile = new AnimationFile(filename);
	NJS_MOTION* animation = animationFile->getmotion();
	if (group->hasKeyNonEmpty("pointer"))
		ProcessPointerList(group->getString("pointer"), animation);
	else
		*(NJS_MOTION*)(strtol((group->getString("address")).c_str(), nullptr, 16) + 0x400000) = *animation;
}

static void ProcessObjListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const objlistdata = new IniFile(filename);
	vector<ObjectListEntry> objs;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!objlistdata->hasGroup(key)) break;
		const IniGroup *objdata = objlistdata->getGroup(key);
		ObjectListEntry entry;
		// TODO: Make these ini fields match structure names
		entry.LoadFlags = (LoadObj)objdata->getInt("Arg1");
		entry.List = objdata->getInt("Arg2");
		entry.ObjectFlags = objdata->getInt("Flags");
		entry.Distance = objdata->getFloat("Distance");
		entry.Function = (ObjectFuncPtr)objdata->getIntRadix("Code", 16);
		entry.Name = UTF8toSJIS(objdata->getString("Name").c_str());
		objs.push_back(entry);
	}
	delete objlistdata;
	ObjectListHead *list = new ObjectListHead;
	list->Count = objs.size();
	list->List = new ObjectListEntry[list->Count];
	arrcpy(list->List, objs.data(), list->Count);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessStartPosINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const startposdata = new IniFile(filename);
	vector<StartPosition> poss;
	for (auto iter = startposdata->cbegin(); iter != startposdata->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		StartPosition pos;
		pos.Level = ParseLevelID(iter->first);
		pos.Rotation1P = iter->second->getIntRadix("YRotation", 16);
		ParseVertex(iter->second->getString("Position", "0,0,0"), pos.Position1P);
		pos.RotationP1 = iter->second->getIntRadix("P1YRotation", 16);
		ParseVertex(iter->second->getString("P1Position", "0,0,0"), pos.PositionP1);
		pos.RotationP2 = iter->second->getIntRadix("P2YRotation", 16);
		ParseVertex(iter->second->getString("P2Position", "0,0,0"), pos.PositionP2);
		poss.push_back(pos);
	}
	delete startposdata;
	auto numents = poss.size();
	StartPosition *list = new StartPosition[numents + 1];
	arrcpy(list, poss.data(), numents);
	clrmem(&list[numents]);
	list[numents].Level = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static vector<TexPackInfo> ProcessTexListINI_Internal(const IniFile *texlistdata)
{
	vector<TexPackInfo> texs;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!texlistdata->hasGroup(key)) break;
		const IniGroup *pvmdata = texlistdata->getGroup(key);
		TexPackInfo entry;
		entry.TexName = _strdup(pvmdata->getString("Name").c_str());
		entry.TexList = (NJS_TEXLIST *)pvmdata->getIntRadix("Textures", 16);
		texs.push_back(entry);
	}
	return texs;
}

static void ProcessTexListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const texlistdata = new IniFile(filename);
	vector<TexPackInfo> texs = ProcessTexListINI_Internal(texlistdata);
	delete texlistdata;
	auto numents = texs.size();
	TexPackInfo *list = new TexPackInfo[numents + 1];
	arrcpy(list, texs.data(), numents);
	clrmem(&list[numents]);
	ProcessPointerList(group->getString("pointer"), list);
}

static vector<char *> ProcessStringArrayINI_Internal(const wchar_t *filename, uint8_t language)
{
	ifstream fstr(filename);
	vector<char *> strs;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		str = DecodeUTF8(UnescapeNewlines(str), language);
		strs.push_back(_strdup(str.c_str()));
	}
	fstr.close();
	return strs;
}

static void ProcessStringArrayINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	vector<char *> strs = ProcessStringArrayINI_Internal(filename,
		ParseLanguage(group->getString("language")));
	auto numents = strs.size();
	char **list = (char**)(group->getIntRadix("address", 16) + 0x400000);;
	arrcpy(list, strs.data(), numents);
}

static void ProcessDeathZoneINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t dzinipath[MAX_PATH];
	swprintf(dzinipath, LengthOfArray(dzinipath), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const dzdata = new IniFile(dzinipath);

	// Remove the filename portion of the path.
	// NOTE: This might be a lower directory than mod_dir,
	// since the filename can have subdirectories.
	PathRemoveFileSpec(dzinipath);

	vector<DeathZone> deathzones;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!dzdata->hasGroup(key)) break;
		uint32_t flag = ParseCharacterFlags(dzdata->getString(key, "Flags"));

		wchar_t dzpath[MAX_PATH];
		swprintf(dzpath, LengthOfArray(dzpath), L"%s\\%u.sa1mdl", dzinipath, i);
		ModelInfo *dzmdl = new ModelInfo(dzpath);
		DeathZone dz = { flag, dzmdl->getmodel() };
		deathzones.push_back(dz);
		// NOTE: dzmdl is not deleted because NJS_OBJECT
		// points to allocated memory in the ModelInfo.
	}

	DeathZone *newlist = new DeathZone[deathzones.size() + 1];
	arrcpy(newlist, deathzones.data(), deathzones.size());
	clrmem(&newlist[deathzones.size()]);
	ProcessPointerList(group->getString("pointer"), newlist);
}

static void ProcessLevelRankScoresINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile* const data = new IniFile(filename);
	vector<LevelRankScores> scores;
	for (auto iter = data->cbegin(); iter != data->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		LevelRankScores entry;
		entry.Level = ParseLevelID(iter->first);
		entry.DRank = (short)iter->second->getInt("DRank");
		entry.CRank = (short)iter->second->getInt("CRank");
		entry.BRank = (short)iter->second->getInt("BRank");
		entry.ARank = (short)iter->second->getInt("ARank");
		scores.push_back(entry);
	}
	delete data;
	auto numents = scores.size();
	LevelRankScores* list = new LevelRankScores[numents + 1];
	arrcpy(list, scores.data(), numents);
	list[numents].Level = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessLevelRankTimesINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile* const data = new IniFile(filename);
	vector<LevelRankTimes> scores;
	for (auto iter = data->cbegin(); iter != data->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		LevelRankTimes entry;
		entry.Level = ParseLevelID(iter->first);
		ParseMinSec(iter->second->getString("DRank"), entry.DRank);
		ParseMinSec(iter->second->getString("CRank"), entry.CRank);
		ParseMinSec(iter->second->getString("BRank"), entry.BRank);
		ParseMinSec(iter->second->getString("ARank"), entry.ARank);
		scores.push_back(entry);
	}
	delete data;
	auto numents = scores.size();
	LevelRankTimes* list = new LevelRankTimes[numents + 1];
	arrcpy(list, scores.data(), numents);
	list[numents].Level = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessEndPosINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile* const startposdata = new IniFile(filename);
	vector<LevelEndPosition> poss;
	for (auto iter = startposdata->cbegin(); iter != startposdata->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		LevelEndPosition pos;
		pos.Level = ParseLevelID(iter->first);
		pos.Mission2YRotation = iter->second->getIntRadix("Mission2YRotation", 16);
		pos.Mission3YRotation = iter->second->getIntRadix("Mission3YRotation", 16);
		pos.field_6 = iter->second->getIntRadix("Unknown", 16);
		ParseVertex(iter->second->getString("Mission2Position", "0,0,0"), pos.Mission2Position);
		ParseVertex(iter->second->getString("Mission3Position", "0,0,0"), pos.Mission3Position);
		poss.push_back(pos);
	}
	delete startposdata;
	auto numents = poss.size();
	LevelEndPosition* list = new LevelEndPosition[numents + 1];
	arrcpy(list, poss.data(), numents);
	clrmem(&list[numents]);
	list[numents].Level = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessAnimationListINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile* const data = new IniFile(filename);
	vector<AnimationInfo> anims;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!data->hasGroup(key)) break;
		const IniGroup* animdata = data->getGroup(key);
		AnimationInfo entry;
		entry.AnimNum = (short)animdata->getInt("Animation");
		entry.ModelNum = (short)animdata->getInt("Model");
		entry.mtnmode = (short)animdata->getInt("Property");
		entry.NextAnimation = (short)animdata->getInt("NextAnimation");
		entry.TransitionSpeed = animdata->getFloat("TransitionSpeed");
		entry.AnimationSpeed = animdata->getFloat("AnimationSpeed");
		anims.push_back(entry);
	}
	delete data;
	auto numents = anims.size();
	AnimationInfo* list = new AnimationInfo[numents];
	arrcpy(list, anims.data(), numents);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessPathListINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	vector<LoopHead*> paths;
	for (unsigned int i = 0; i < 999; i++)
	{
		wchar_t levelpath[MAX_PATH];
		swprintf(levelpath, LengthOfArray(levelpath), L"%s\\%u.ini",
			filename, i);

		if (!Exists(levelpath))
		{
			break;
		}

		auto inidata = new IniFile(levelpath);
		const IniGroup* entdata;
		vector<LoopPoint> points;
		char buf2[8];

		for (unsigned int j = 0; j < 999; j++)
		{
			snprintf(buf2, LengthOfArray(buf2), "%u", j);

			if (!inidata->hasGroup(buf2))
			{
				break;
			}

			entdata = inidata->getGroup(buf2);
			LoopPoint point;
			point.XRot = (int16_t)entdata->getIntRadix("XRotation", 16);
			point.YRot = (int16_t)entdata->getIntRadix("ZRotation", 16);
			point.Distance = entdata->getFloat("Distance");
			ParseVertex(entdata->getString("Position", "0,0,0"), point.Position);
			points.push_back(point);
		}

		entdata = inidata->getGroup("");

		auto* path = new LoopHead;

		path->anonymous_0 = (int16_t)entdata->getInt("Unknown");
		path->Count = (int16_t)points.size();
		path->TotalDistance = entdata->getFloat("TotalDistance");
		path->Points = new LoopPoint[path->Count];

		arrcpy(path->Points, points.data(), path->Count);

		path->Object = (ObjectFuncPtr)entdata->getIntRadix("Code", 16);
		paths.push_back(path);

		delete inidata;
	}
	auto numents = paths.size();
	LoopHead** list = new LoopHead*[numents + 1];
	arrcpy(list, paths.data(), numents);
	list[numents] = nullptr;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessSoundListINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address"))
	{
		return;
	}

	wchar_t filename[MAX_PATH]{};

	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());

	auto inidata = new IniFile(filename);
	vector<MLTSoundEntry> sounds;

	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8]{};
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key))
			break;
		const IniGroup* snddata = inidata->getGroup(key);
		MLTSoundEntry entry{};
		entry.Bank = snddata->getIntRadix("Bank", 8);
		entry.ID = snddata->getIntRadix("ID", 8);
		if (snddata->hasKeyNonEmpty("SecondaryBank"))
		entry.SecondaryBank = snddata->getIntRadix("SecondaryBank", 8);
		entry.DefaultFlags = snddata->getIntRadix("DefaultFlags", 8);
		if (snddata->hasKeyNonEmpty("Unknown"))
		entry.Unknown = snddata->getIntRadix("Unknown", 8);
		if (snddata->hasKeyNonEmpty("DefaultDistance"))
		entry.DefaultDistance = snddata->getIntRadix("DefaultDistance", 8);
		sounds.push_back(entry);
	}
	delete inidata;
	size_t numents = sounds.size();
	MLTSoundList* list = (MLTSoundList*)(group->getIntRadix("address", 16) + 0x400000);;
	list->List = new MLTSoundEntry[numents];
	arrcpy(list->List, sounds.data(), numents);
	list->Size = (int)numents;
}

static void ProcessAnimIndexListINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
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
		wchar_t *end;
		const auto ind = static_cast<uint16_t>(wcstol(data.cFileName, &end, 10));
		if (end == data.cFileName) continue; // filename was not a number
		swprintf(filename, LengthOfArray(filename), L"%s\\%s\\%s",
			mod_dir.c_str(), group->getWString("filename").c_str(), data.cFileName);
		auto animfile = new AnimationFile(filename);
		AnimationIndex entry { ind, static_cast<uint16_t>(animfile->getmodelcount()), animfile->getmotion() };
		anims.push_back(entry);
	} while (FindNextFile(hFind, &data));
	auto numents = anims.size();
	AnimationIndex* list = new AnimationIndex[numents + 1];
	arrcpy(list, anims.data(), numents);
	memset(&list[numents], -1, sizeof(AnimationIndex));
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessStorySequenceINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile* const data = new IniFile(filename);
	vector<StoryEntry> seqs;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!data->hasGroup(key)) break;
		const IniGroup* seqdata = data->getGroup(key);
		StoryEntry entry;
		entry.Type = ParseStoryEntryType(seqdata->getString("Type"));
		entry.Character = ParseCharacter(seqdata->getString("Character"));
		entry.Level = ParseLevelID(seqdata->getString("Level"));
		memset(entry.Events, -1, SizeOfArray(entry.Events));
		if (seqdata->hasKeyNonEmpty("Events"))
		{
			auto events = split(seqdata->getString("Events", "-1"), ',');
			for (size_t j = 0; j < min(events.size(), 4u); ++j)
				entry.Events[j] = static_cast<int16_t>(strtol(events[j].c_str(), nullptr, 10));
		}
		seqs.push_back(entry);
	}
	delete data;
	auto numents = seqs.size();
	StoryEntry* list = new StoryEntry[numents + 1];
	arrcpy(list, seqs.data(), numents);
	list[numents].Type = StoryEntryType_End;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessStringINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	uint8_t language = ParseLanguage(group->getString("language"));
	ifstream fstr(filename);
	string str;
	while (fstr.good())
	{
		string str2;
		getline(fstr, str2);
		if (str2[0] == '\r')
			str2[0] = '\n';
		str.append(DecodeUTF8(str2, language));
	}
	fstr.close();
	ProcessPointerList(group->getString("pointer"), _strdup(str.c_str()));
}

static void ProcessPhysicsDataINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename"))
	{
		return;
	}

	auto addr = (player_parameter*)group->getIntRadix("address", 16);

	if (addr == nullptr)
	{
		return;
	}

	addr = (player_parameter*)((intptr_t)addr + 0x400000);
	wchar_t filename[MAX_PATH]{};

	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());

	auto inidata = new IniFile(filename);
	auto inigrp = inidata->getGroup("");
	addr->jump2_timer = inigrp->getInt("jump2_timer");
	addr->pos_error = inigrp->getFloat("pos_error");
	addr->lim_h_spd = inigrp->getFloat("lim_h_spd");
	addr->lim_v_spd = inigrp->getFloat("lim_v_spd");
	addr->max_x_spd = inigrp->getFloat("max_x_spd");
	addr->max_psh_spd = inigrp->getFloat("max_psh_spd");
	addr->jmp_y_spd = inigrp->getFloat("jmp_y_spd");
	addr->nocon_speed = inigrp->getFloat("nocon_speed");
	addr->slide_speed = inigrp->getFloat("slide_speed");
	addr->jog_speed = inigrp->getFloat("jog_speed");
	addr->run_speed = inigrp->getFloat("run_speed");
	addr->rush_speed = inigrp->getFloat("rush_speed");
	addr->crash_speed = inigrp->getFloat("crash_speed");
	addr->dash_speed = inigrp->getFloat("dash_speed");
	addr->jmp_addit = inigrp->getFloat("jmp_addit");
	addr->run_accel = inigrp->getFloat("run_accel");
	addr->air_accel = inigrp->getFloat("air_accel");
	addr->slow_down = inigrp->getFloat("slow_down");
	addr->run_break = inigrp->getFloat("run_break");
	addr->air_break = inigrp->getFloat("air_break");
	addr->air_resist_air = inigrp->getFloat("air_resist_air");
	addr->air_resist = inigrp->getFloat("air_resist");
	addr->air_resist_y = inigrp->getFloat("air_resist_y");
	addr->air_resist_z = inigrp->getFloat("air_resist_z");
	addr->grd_frict = inigrp->getFloat("grd_frict");
	addr->grd_frict_z = inigrp->getFloat("grd_frict_z");
	addr->lim_frict = inigrp->getFloat("lim_frict");
	addr->rat_bound = inigrp->getFloat("rat_bound");
	addr->rad = inigrp->getFloat("rad");
	addr->height = inigrp->getFloat("height");
	addr->weight = inigrp->getFloat("weight");
	addr->eyes_height = inigrp->getFloat("eyes_height");
	addr->center_height = inigrp->getFloat("center_height");
	delete inidata;
}

static void ProcessKartCourseINI(const IniGroup* group, const wstring& mod_dir)
{
	if (!group->hasKeyNonEmpty("filename"))
	{
		return;
	}

	auto addr = (KartCourse*)group->getIntRadix("address", 16);

	if (addr == nullptr)
	{
		return;
	}

	addr = (KartCourse*)((intptr_t)addr + 0x400000);
	wchar_t filename[MAX_PATH]{};

	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());

	ifstream fstr(filename);
	vector<char> course;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		course.push_back(std::stoi(str));
	}
	fstr.close();
	addr->Length = course.size();
	addr->Course = new char[addr->Length];
	memcpy(addr->Course, course.data(), addr->Length);
}

typedef void(__cdecl *exedatafunc_t)(const IniGroup *group, const wstring &mod_dir);
static const unordered_map<string, exedatafunc_t> exedatafuncmap = {
	{ "landtable", ProcessLandTableINI },
	{ "model", ProcessModelINI },
	{ "basicmodel", ProcessModelINI },
	{ "chunkmodel", ProcessModelINI },
	{ "gcmodel", ProcessModelINI },
	{ "action", ProcessActionINI },
	{ "animation", ProcessAnimationINI },
	{ "objlist", ProcessObjListINI },
	{ "startpos", ProcessStartPosINI },
	{ "texturedata", ProcessTexListINI },
	{ "stringarray", ProcessStringArrayINI },
	{ "deathzone", ProcessDeathZoneINI },
	//{ "stageselectlist", ProcessStageSelectListINI },
	{ "levelrankscores", ProcessLevelRankScoresINI },
	{ "levelranktimes", ProcessLevelRankTimesINI },
	{ "endpos", ProcessEndPosINI },
	{ "soundlist", ProcessSoundListINI },
	{ "animationlist", ProcessAnimationListINI },
	{ "pathlist", ProcessPathListINI },
	//{ "bmitemattrlist", ProcessBMItemAttrListINI },
	{ "animindexlist", ProcessAnimIndexListINI },
	{ "storysequence", ProcessStorySequenceINI },
	{ "string", ProcessStringINI },
	{ "physicsdata", ProcessPhysicsDataINI },
	{ "kartcourse", ProcessKartCourseINI },
};

void ProcessEXEData(const wchar_t *filename, const wstring &mod_dir)
{
	const IniFile *const exedata = new IniFile(filename);
	for (auto iter = exedata->cbegin(); iter != exedata->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		IniGroup *group = iter->second;
		auto typestr = group->getString("type");
		auto type = exedatafuncmap.find(typestr);
		if (type != exedatafuncmap.end())
			type->second(group, mod_dir);
		else
			PrintDebug("Handler for INI data type \"%s\" not found!", typestr.c_str());
	}
	delete exedata;
}