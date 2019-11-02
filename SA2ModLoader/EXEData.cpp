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

static const unordered_map<string, uint8_t> languagesnamemap = {
	{ "japanese", 0 },
	{ "english", 1 },
	{ "french", 2 },
	{ "spanish", 3 },
	{ "german", 4 }
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
	if (language <= 2)
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

static void ProcessModelINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	ModelInfo *const mdlinf = new ModelInfo(filename);
	NJS_OBJECT *model = mdlinf->getmodel();
	GetModelLabels(mdlinf, model);
	ProcessPointerList(group->getString("pointer"), model);
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

static void ProcessAnimationINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	AnimationFile *const animationFile = new AnimationFile(filename);
	NJS_MOTION *animation = animationFile->getmotion();
	ProcessPointerList(group->getString("pointer"), animation);
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
		entry.DistanceMaybe = objdata->getFloat("Distance");
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
		entry.TexName = strdup(pvmdata->getString("Name").c_str());
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
		strs.push_back(strdup(str.c_str()));
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
		entry.anonymous_2 = (short)animdata->getInt("Property");
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

typedef void(__cdecl *exedatafunc_t)(const IniGroup *group, const wstring &mod_dir);
static const unordered_map<string, exedatafunc_t> exedatafuncmap = {
	{ "landtable", ProcessLandTableINI },
	{ "model", ProcessModelINI },
	{ "basicmodel", ProcessModelINI },
	{ "chunkmodel", ProcessModelINI },
	{ "action", ProcessActionINI },
	{ "animation", ProcessAnimationINI },
	{ "objlist", ProcessObjListINI },
	{ "startpos", ProcessStartPosINI },
	{ "texlist", ProcessTexListINI },
	{ "stringarray", ProcessStringArrayINI },
	{ "deathzone", ProcessDeathZoneINI },
	//{ "stageselectlist", ProcessStageSelectListINI },
	{ "levelrankscores", ProcessLevelRankScoresINI },
	{ "levelranktimes", ProcessLevelRankTimesINI },
	{ "endpos", ProcessEndPosINI },
	{ "animationlist", ProcessAnimationListINI },
	{ "pathlist", ProcessPathListINI },
};

void ProcessEXEData(const wchar_t *filename, const wstring &mod_dir)
{
	const IniFile *const exedata = new IniFile(filename);
	for (auto iter = exedata->cbegin(); iter != exedata->cend(); ++iter)
	{
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