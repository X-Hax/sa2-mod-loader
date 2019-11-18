#include "stdafx.h"
#include <shellapi.h>
#include <string>
#include <unordered_map>
#include "testspawn.h"

using std::wstring;
using std::unordered_map;

static wstring trim(const wstring& s)
{
	auto st = s.find_first_not_of(L' ');
	if (st == wstring::npos)
		st = 0;
	auto ed = s.find_last_not_of(L' ');
	if (ed == wstring::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

static const unordered_map<wstring, uint8_t> levelidsnamemap = {
	{ L"basictest", LevelIDs_BasicTest },
	{ L"knucklestest", LevelIDs_KnucklesTest },
	{ L"sonictest", LevelIDs_SonicTest },
	{ L"greenforest", LevelIDs_GreenForest },
	{ L"whitejungle", LevelIDs_WhiteJungle },
	{ L"pumpkinhill", LevelIDs_PumpkinHill },
	{ L"skyrail", LevelIDs_SkyRail },
	{ L"aquaticmine", LevelIDs_AquaticMine },
	{ L"securityhall", LevelIDs_SecurityHall },
	{ L"prisonlane", LevelIDs_PrisonLane },
	{ L"metalharbor", LevelIDs_MetalHarbor },
	{ L"irongate", LevelIDs_IronGate },
	{ L"weaponsbed", LevelIDs_WeaponsBed },
	{ L"cityescape", LevelIDs_CityEscape },
	{ L"radicalhighway", LevelIDs_RadicalHighway },
	{ L"weaponsbed2p", LevelIDs_WeaponsBed2P },
	{ L"wildcanyon", LevelIDs_WildCanyon },
	{ L"missionstreet", LevelIDs_MissionStreet },
	{ L"drylagoon", LevelIDs_DryLagoon },
	{ L"sonicvsshadow1", LevelIDs_SonicVsShadow1 },
	{ L"tailsvseggman1", LevelIDs_TailsVsEggman1 },
	{ L"sandocean", LevelIDs_SandOcean },
	{ L"crazygadget", LevelIDs_CrazyGadget },
	{ L"hiddenbase", LevelIDs_HiddenBase },
	{ L"eternalengine", LevelIDs_EternalEngine },
	{ L"deathchamber", LevelIDs_DeathChamber },
	{ L"eggquarters", LevelIDs_EggQuarters },
	{ L"lostcolony", LevelIDs_LostColony },
	{ L"pyramidcave", LevelIDs_PyramidCave },
	{ L"tailsvseggman2", LevelIDs_TailsVsEggman2 },
	{ L"finalrush", LevelIDs_FinalRush },
	{ L"greenhill", LevelIDs_GreenHill },
	{ L"meteorherd", LevelIDs_MeteorHerd },
	{ L"knucklesvsrouge", LevelIDs_KnucklesVsRouge },
	{ L"cannonscores", LevelIDs_CannonsCoreS },
	{ L"cannonscoree", LevelIDs_CannonsCoreE },
	{ L"cannonscoret", LevelIDs_CannonsCoreT },
	{ L"cannonscorer", LevelIDs_CannonsCoreR },
	{ L"cannonscorek", LevelIDs_CannonsCoreK },
	{ L"missionstreet2p", LevelIDs_MissionStreet2P },
	{ L"finalchase", LevelIDs_FinalChase },
	{ L"wildcanyon2p", LevelIDs_WildCanyon2P },
	{ L"sonicvsshadow2", LevelIDs_SonicVsShadow2 },
	{ L"cosmicwall", LevelIDs_CosmicWall },
	{ L"madspace", LevelIDs_MadSpace },
	{ L"sandocean2p", LevelIDs_SandOcean2P },
	{ L"drylagoon2p", LevelIDs_DryLagoon2P },
	{ L"pyramidrace", LevelIDs_PyramidRace },
	{ L"hiddenbase2p", LevelIDs_HiddenBase2P },
	{ L"poolquest", LevelIDs_PoolQuest },
	{ L"planetquest", LevelIDs_PlanetQuest },
	{ L"deckrace", LevelIDs_DeckRace },
	{ L"downtownrace", LevelIDs_DowntownRace },
	{ L"cosmicwall2p", LevelIDs_CosmicWall2P },
	{ L"grindrace", LevelIDs_GrindRace },
	{ L"lostcolony2p", LevelIDs_LostColony2P },
	{ L"eternalengine2p", LevelIDs_EternalEngine2P },
	{ L"metalharbor2p", LevelIDs_MetalHarbor2P },
	{ L"irongate2p", LevelIDs_IronGate2P },
	{ L"deathchamber2p", LevelIDs_DeathChamber2P },
	{ L"bigfoot", LevelIDs_BigFoot },
	{ L"hotshot", LevelIDs_HotShot },
	{ L"flyingdog", LevelIDs_FlyingDog },
	{ L"kingboomboo", LevelIDs_KingBoomBoo },
	{ L"egggolems", LevelIDs_EggGolemS },
	{ L"biolizard", LevelIDs_Biolizard },
	{ L"finalhazard", LevelIDs_FinalHazard },
	{ L"egggoleme", LevelIDs_EggGolemE },
	{ L"route101280", LevelIDs_Route101280 },
	{ L"kartrace", LevelIDs_KartRace },
	{ L"chaoworld", LevelIDs_ChaoWorld },
	{ L"invalid", LevelIDs_Invalid }
};

static uint8_t ParseLevelID(const wstring& str)
{
	wstring str2 = trim(str);
	std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = levelidsnamemap.find(str2);
	if (lv != levelidsnamemap.end())
		return lv->second;
	else
		return (uint8_t)wcstol(str.c_str(), nullptr, 10);
}

static const unordered_map<wstring, uint8_t> charnamemap = {
	{ L"sonic", Characters_Sonic },
	{ L"shadow", Characters_Shadow },
	{ L"tails", Characters_Tails },
	{ L"eggman", Characters_Eggman },
	{ L"knuckles", Characters_Knuckles },
	{ L"rouge", Characters_Rouge },
	{ L"mechtails", Characters_MechTails },
	{ L"mecheggman", Characters_MechEggman }
};

static uint8_t ParseCharacter(const wstring& str)
{
	wstring str2 = trim(str);
	std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto ch = charnamemap.find(str2);
	if (ch != charnamemap.end())
		return ch->second;
	else
		return (uint8_t)wcstol(str.c_str(), nullptr, 10);
}

void TestSpawnCheckArgs(const HelperFunctions &helperFunctions)
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	char level = -1;
	short event = -1;
	char chara = -1;

	for (int i = 1; i < argc; i++)
	{
		if (!wcscmp(argv[i], L"--level") || !wcscmp(argv[i], L"-l"))
		{
			level = ParseLevelID(argv[++i]);
			PrintDebug("Loading level: %d\n", level);
		}
		else if (!wcscmp(argv[i], L"--character") || !wcscmp(argv[i], L"-c"))
		{
			chara = ParseCharacter(argv[++i]);
			PrintDebug("Loading character: %d\n", chara);
		}
		else if (!wcscmp(argv[i], L"--event") || !wcscmp(argv[i], L"-e"))
			event = (short)wcstol(argv[++i], nullptr, 10);
		else if (!wcscmp(argv[i], L"--position") || !wcscmp(argv[i], L"-p"))
		{
			if (level == -1 || chara == -1)
			{
				MessageBoxA(nullptr, "Insufficient arguments for parameter: --position.\n"
					"--level and --character must be specified before --position.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			if (i + 3 >= argc)
			{
				MessageBoxA(nullptr, "Insufficient arguments for parameter: --position.\n"
					"All 3 components (X, Y, Z) of the spawn position must be provided. Default spawn point will be used.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			NJS_VECTOR pos = { (float)_wtof(argv[++i]), (float)_wtof(argv[++i]), (float)_wtof(argv[++i]) };

			StartPosition position = {
				level,
				// YRot
				0, 0, 0,
				// Position
				pos, pos, pos
			};

			helperFunctions.RegisterStartPosition(chara, position);
		}
		else if (!wcscmp(argv[i], L"--savenum") || !wcscmp(argv[i], L"-s"))
		{
			SetWorkingSave(wcstol(argv[++i], nullptr, 10));
			ProbablyLoadsSave(0);
		}
	}

	StoryEntry* story = (StoryEntry*)0x173A5E0;
	if (level != -1 && chara != -1)
	{
		story->Type = StoryEntryType_Level;
		story->Level = level;
		story->Character = chara;
	}
	else if (event != -1)
		story->Events[0] = event;

	LocalFree(argv);
}