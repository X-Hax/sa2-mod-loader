#include "stdafx.h"
#include <shellapi.h>
#include <string>
#include <unordered_map>
#include "testspawn.h"

using std::wstring;
using std::unordered_map;
bool testSpawnCutscene = false;
bool testSpawnLvl = false;
bool useTestSpawn = false;
FunctionHook<void> LifeIconTex_t(0x44E4C0);

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
	{ L"mecheggman", Characters_MechEggman },
	{ L"amy", Characters_Amy },
	{ L"metalsonic", Characters_MetalSonic },
	{ L"chao", Characters_ChaoWalker },
	{ L"darkchao", Characters_DarkChaoWalker },
	{ L"tikal", Characters_Tikal },
	{ L"chaos", Characters_Chaos },
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

static const unordered_map<wstring, int> chaoareamap = {
	{ L"lobby", ChaoArea_Lobby },
	{ L"neutralgarden", ChaoArea_NeutralGarden },
	{ L"herogarden", ChaoArea_HeroGarden },
	{ L"darkgarden", ChaoArea_DarkGarden },
	{ L"race", ChaoArea_Race },
	{ L"entrance", ChaoArea_Entrance },
	{ L"kindergarten", ChaoArea_Kindergarten },
	{ L"stadium", ChaoArea_Stadium },
	{ L"karate", ChaoArea_Karate },
	{ L"namemachine", ChaoArea_NameMachine }
};

static int ParseChaoArea(const wstring& str)
{
	wstring str2 = trim(str);
	std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto area = chaoareamap.find(str2);
	if (area != chaoareamap.end())
		return area->second;
	else
		return (uint8_t)wcstol(str.c_str(), nullptr, 10);
}

static void LoadMenuButtonsTex()
{
	char buffer[40];

	if (*(int*)0x174B5FC)
	{
		sprintf(buffer, "\\SOC\\menuButton%d\\menu_button_%d.png", TextLanguage, TextLanguage);
	}
	else
	{
		sprintf(buffer, "\\SOC\\menuButtonK%d\\menu_button_%d.png", TextLanguage, TextLanguage);
	}

	MenuButtonImage = LoadPNG(buffer);
}

static void LifeIconTexStuff()
{
	if (MainCharObj2[0] == nullptr)
		return;

	if (MainCharObj2[0]->CharID2 == Characters_SuperSonic || MainCharObj2[0]->CharID2 == Characters_DarkChaoWalker)
		return;

	LifeIconTex_t.Original();
}

static void SetAltCharacter(uint8_t id) {

	switch (id)
	{
	case Characters_Amy:
		WriteJump(LoadSonic, LoadAmy);
		CurrentCharacter = Characters_Sonic;
		break;
	case Characters_MetalSonic:
		WriteJump(LoadShadow, LoadMetalSonic);
		CurrentCharacter = Characters_Shadow;
		break;
	case Characters_ChaoWalker:
		WriteJump(LoadMechTails, LoadChaoWalker);
		CurrentCharacter = Characters_MechTails;
		break;
	case Characters_DarkChaoWalker:
		WriteJump(LoadMechEggman, LoadDarkChaoWalker);
		CurrentCharacter = Characters_MechEggman;
		break;
	case Characters_Tikal:
		WriteJump(LoadKnuckles, LoadTikal);
		CurrentCharacter = Characters_Knuckles;
		break;
	case Characters_Chaos:
		WriteJump(LoadRouge, LoadChaos);
		CurrentCharacter = Characters_Rouge;
		break;
	}
}

VoidFunc(sub_6897D0, 0x6897D0);
VoidFunc(sub_444A90, 0x444A90);
static StartPosition gTestSpawnStartPos;
static bool setSaveOnce = false;
void TSLoadSave_r()
{
	if (!setSaveOnce)
	{
		ProbablyLoadsSave(0);
		setSaveOnce = true;
	}
	InitCurrentLevelAndScreenCount();
}

void TestSpawnCheckArgs(const HelperFunctions& helperFunctions)
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);


	uint8_t alt = 0;
	bool posEdited = false;
	
	for (int i = 1; i < argc; i++)
	{
		if (!wcscmp(argv[i], L"--level") || !wcscmp(argv[i], L"-l"))
		{
			CurrentLevel = ParseLevelID(argv[++i]);
			PrintDebug("Loading level: %d\n", CurrentLevel);
			
			LoadTipsTexs(TextLanguage); // Skipped. Loaded during copyright screen.
			LoadMenuButtonsTex(); // Skipped. Loaded during menu initialization.
			ChaoSaveIndexThing = 0; // Allow Chao World saves to work.

			//Prevent the game to save (fix overwritten savedata.)
			WriteData((uint8_t*)0x4436A0, 0xC3ui8);
			WriteData((uint8_t*)0x426760, 0xC3ui8);

			// NOP. Prevents CurrentLevel from being overwritten for illegal level.
			WriteData<13>(reinterpret_cast<void*>(0x43C983), 0x90u);

			// Swap GameMode
			WriteData(reinterpret_cast<int*>(0x43459A), static_cast<int>(GameMode_StartLevel));

			testSpawnLvl = true;
		}
		else if (!wcscmp(argv[i], L"--character") || !wcscmp(argv[i], L"-c"))
		{
			alt = ParseCharacter(argv[++i]);

			CurrentCharacter = alt;
			SetAltCharacter(alt);
	
			PrintDebug("Loading character: %d\n", CurrentCharacter);

			// NOP. Prevents CurrentCharacter from being overwritten.
			WriteData<10>(reinterpret_cast<void*>(0x4395F3), 0x90u);

			LifeIconTex_t.Hook(LifeIconTexStuff);

		
		}
		else if (!wcscmp(argv[i], L"--player2") || !wcscmp(argv[i], L"-p2"))
		{
			CurrentCharacter2P = ParseCharacter(argv[++i]);
			PrintDebug("Loading player 2: %d\n", CurrentCharacter2P);

			TwoPlayerMode = 1;
		}
		else if (!wcscmp(argv[i], L"--event") || !wcscmp(argv[i], L"-e"))
		{
			((StoryEntry*)0x173A5E0)->Events[0] = static_cast<int16_t>(_wtoi(argv[++i]));
			if (((StoryEntry*)0x173A5E0)->Events[0] == FMV_HERO_ENDING)
				helperFunctions.ReplaceFile("resource\\gd_PC\\event\\e0428_m.sfd", "resource\\gd_PC\\e0428_m.sfd");
			if (((StoryEntry*)0x173A5E0)->Events[0] == FMV_LAST_ARK)
				helperFunctions.ReplaceFile("resource\\gd_PC\\event\\e0609_m.sfd", "resource\\gd_PC\\e0609_m.sfd");
			((StoryEntry*)0x173A5E0)->Events[1] = E_NONE;
			((StoryEntry*)0x173A5E0)->Events[2] = E_NONE;
			((StoryEntry*)0x173A5E0)->Events[3] = E_NONE;
			testSpawnCutscene = true;
		}
		else if (!wcscmp(argv[i], L"--position") || !wcscmp(argv[i], L"-p"))
		{
			if (testSpawnLvl == false)
			{
				MessageBoxA(MainWindowHandle, "Insufficient arguments for parameter: --position.\n"
					"--level must be specified before --position.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			if (i + 3 >= argc)
			{
				MessageBoxA(MainWindowHandle, "Insufficient arguments for parameter: --position.\n"
					"All 3 components (X, Y, Z) of the spawn position must be provided. Default spawn point will be used.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			NJS_VECTOR pos = { std::stof(argv[++i]), std::stof(argv[++i]), std::stof(argv[++i]) };

			gTestSpawnStartPos.Level = CurrentLevel;
			gTestSpawnStartPos.Position1P = pos;
			gTestSpawnStartPos.PositionP1 = pos;
			gTestSpawnStartPos.PositionP2 = pos;

			posEdited = true;
		}
		else if (!wcscmp(argv[i], L"--rotation") || !wcscmp(argv[i], L"-r"))
		{
			if (!posEdited)
			{
				MessageBoxA(MainWindowHandle, "Insufficient arguments for parameter: --rotation.\n"
					"--position must be specified before --rotation.",
					"Insufficient arguments", MB_OK);

				continue;
			}

			gTestSpawnStartPos.Rotation1P = static_cast<int16_t>(_wtoi(argv[++i]));	
		}
		else if (!wcscmp(argv[i], L"--savenum") || !wcscmp(argv[i], L"-s"))
		{
			SetWorkingSave(_wtoi(argv[++i]));
			sub_6897D0();
			ProbablyLoadsSave(0);
			sub_444A90();
			WriteCall((void*)0x43A9A1, TSLoadSave_r);
		
		}
		else if (!wcscmp(argv[i], L"--chaoarea"))
		{
			WriteData((int*)0x52AD5B, ParseChaoArea(argv[++i]));
		}
		else if (!wcscmp(argv[i], L"--mission") || !wcscmp(argv[i], L"-m"))
		{
			MissionNum = _wtoi(argv[++i]);
			PrintDebug("Loading Mission: %d\n", MissionNum);
		}
	}

	if (posEdited)
		helperFunctions.RegisterStartPosition(CurrentCharacter, gTestSpawnStartPos);

	LocalFree(argv);

	useTestSpawn = testSpawnCutscene || testSpawnLvl;
}