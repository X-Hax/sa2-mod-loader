#pragma once

#include "../ModLoader/MemAccess.h"
#include "SA2Structs.h"

// SA2 Functions
FunctionPointer(int, PrintDebug, (char *format, ...), 0x426740);
FunctionPointer(signed int, ProcessChunkModel, (NJS_CNK_MODEL *a1), 0x42D650);
VoidFunc(LoadCharacters, 0x43D630);
FastcallFunctionPointer(signed int, LoadTexturePack, (char *filename, TexList *texlist), 0x44C350);
ThiscallFunctionPointer(TexList *, LoadCharTextures, (char *filename), 0x44C510);
ObjectFunc(DeathZoneObject, 0x46AD50);
FunctionPointer(bool, CheckBreakObject, (ObjectMaster *obj, ObjectMaster *other), 0x46EC00);
ObjectFunc(DeleteObject_, 0x46F720);
FunctionPointer(CharObj1 *, AllocCharObj1, (void), 0x470B40);
FunctionPointer(ObjectMaster *, LoadChildObject, (int a3, void(__cdecl *a4)(ObjectMaster *), ObjectMaster *parent), 0x470C00);
FunctionPointer(signed int, LoadLandManager, (LandTable *a1), 0x47BD30);
ObjectFunc(LandManager_Main, 0x47C180);
FunctionPointer(signed int, LoadSetObject, (ObjectListHead *list, void *setfile), 0x487E40);
ObjectFunc(SetObject_Main, 0x487F60);
VoidFunc(ReadSET_2P, 0x4883D0);
VoidFunc(ReadSET_1P, 0x488630);
ThiscallFunctionPointer(void *, LoadStageSETFile, (char *filename, int a2), 0x488F60);
VoidFunc(CountPerfectRings, 0x4890E0);
ThiscallFunctionPointer(unsigned int, PRSDec, (uint8_t *src, uint8_t *dst), 0x48F980);
FunctionPointer(signed int, LoadStagePaths, (LoopHead **a1), 0x490110);
FunctionPointer(void, LoadPathObjects, (LoopHead **a1), 0x490180);
VoidFunc(LoadSuperSonic, 0x49A950);
VoidFunc(LoadSuperShadow, 0x49ABB0);
ObjectFunc(SuperSonic_Main, 0x49AE10);
ObjectFunc(SuperSonic_Display, 0x49CA60);
ObjectFunc(LastBossPlayerManager_Main, 0x49E520);
VoidFunc(LoadEggGolemECharAnims, 0x4A6DC0);
ObjectFunc(ControlShadow_Main, 0x4C7ED0);
VoidFunc(LoadAquaticMineCharAnims, 0x4D4C30);
VoidFunc(LoadCannonsCoreRCharAnims, 0x4DDA70);
VoidFunc(LoadDryLagoon2PPoolQuestCharAnims, 0x4E63D0);
ObjectFunc(Beetle_Main, 0x501530);
FunctionPointer(void, Bomb_Main, (ObjectMaster *), 0x513830);
VoidFunc(LoadSandOcean2PCharAnims, 0x51D020);
VoidFunc(LoadChaomainModule, 0x52AB10);
ObjectFunc(Chao_Main, 0x54FE20);
FunctionPointer(ObjectMaster *, CreateChao, (ChaoData *chaoData, int a2, void *a3, Vertex *position, int yrot), 0x5501D0);
FunctionPointer(ObjectMaster *, CreateChaoEgg, (const void *a1, ChaoData *chaoData, int a3, Vertex *position, int a5), 0x57B9C0);
VoidFunc(CityEscapeInit, 0x5DCD50);
VoidFunc(LoadCityEscapeCharAnims, 0x5DDCF0);
ObjectFunc(MusicChanger_Main, 0x5E2D70);
VoidFunc(LoadGreenForestCharAnims, 0x5EE920);
FunctionPointer(ObjectMaster *, LoadCartExecuter, (const void *a1), 0x61C520);
ObjectFunc(CartExecuter_Main, 0x61CD60);
VoidFunc(LoadDryLagoonCharAnims, 0x647830);
VoidFunc(LoadSandOceanCharAnims, 0x658A10);
VoidFunc(LoadCannonsCoreKCharAnims, 0x65EF70);
ObjectFunc(StageMapExec_Main, 0x677350);
VoidFunc(LoadStageMapExec, 0x6779A0);
VoidFunc(LoadEggQuartersCharAnims, 0x693570);
ObjectFunc(Omochao_Main, 0x6C0780);
FunctionPointer(void, HurtPlayer, (int playerNum), 0x6C1AF0);
ObjectFunc(GoalRing_Main, 0x6C63C0);
ObjectFunc(ItemBoxAir_Main, 0x6C8EF0);
ObjectFunc(WoodenCrate_Main, 0x6CD7F0);
ObjectFunc(EmeraldPiece_Main, 0x6D00C0);
ObjectFunc(IronCrate_Main, 0x6D63F0);
ObjectFunc(SampleS_Main, 0x6D6EE0);
VoidFunc(LoadMetalHarborCharAnims, 0x6F36A0);
VoidFunc(LoadPyramidCaveCharAnims, 0x6FC640);
FunctionPointer(void, nullsub_1, (ObjectMaster *), 0x6FE430);
VoidFunc(LoadHiddenBaseCharAnims, 0x711C30);
FunctionPointer(void, LoadSonic, (int playerNum), 0x716E00);
FunctionPointer(void, LoadAmy, (int playerNum), 0x7170E0);
FunctionPointer(void, LoadShadow, (int playerNum), 0x717360);
FunctionPointer(void, LoadMetalSonic, (int playerNum), 0x717640);
FunctionPointer(void, Sonic_Main, (ObjectMaster *), 0x717840);
ObjectFunc(Sonic_Display, 0x720090);
ThiscallFunctionPointer(signed int, Sonic_CheckSpecial, (SonicCharObj2 *a1, CharObj1 *a2), 0x724250);
FunctionPointer(void, LoadKnuckles, (int playerNum), 0x728110);
FunctionPointer(void, LoadRouge, (int playerNum), 0x728460);
FunctionPointer(void, LoadTikal, (int playerNum), 0x7288B0);
FunctionPointer(void, LoadChaos, (int playerNum), 0x728B60);
ObjectFunc(Knuckles_Main, 0x728D70);
ObjectFunc(Knuckles_Display, 0x72EF20);
ObjectFunc(Tikal_Display, 0x72FB10);
ObjectFunc(Rouge_Display, 0x730970);
ObjectFunc(Chaos_Display, 0x731A30);
ObjectFunc(EmeraldManager_Main, 0x739570);
ObjectFunc(Eggman_Main, 0x73C380);
ObjectFunc(Eggman_Display, 0x73EF20);
FunctionPointer(void, LoadMechEggman, (int playerNum), 0x740C50);
FunctionPointer(void, LoadMechTails, (int playerNum), 0x740EB0);
FunctionPointer(void, LoadChaoWalker, (int playerNum), 0x741110);
FunctionPointer(void, LoadDarkChaoWalker, (int playerNum), 0x7412F0);
ObjectFunc(MechEggman_Main, 0x741530);
ObjectFunc(MechEggman_Delete, 0x743E90);
ObjectFunc(MechEggman_Display, 0x7444F0);
ObjectFunc(ChaoWalker_Display, 0x746290);
ObjectFunc(DarkChaoWalker_Display, 0x7470D0);
ObjectFunc(MechTails_Display, 0x747DD0);
ObjectFunc(Tails_Main, 0x74D170);
ObjectFunc(Tails_Display, 0x7507D0);
FunctionPointer(ObjectMaster *, LoadInvincibility, (char player), 0x7532F0);
ObjectFunc(Invincibility_Main, 0x754150);
ObjectFunc(LightAttackParticle_Render, 0x757B30);
ObjectFunc(BossPowerGuageExec_Main, 0x761C30);
VoidFunc(UpdateControllers, 0x77E780);
// Stops music playback
VoidFunc(StopMusic, 0x00442F50);
// Resets music playback to the last song as specified by PlayMusic
VoidFunc(ResetMusic, 0x00442D90);

// TODO: Rename all of the music related functions to more clearly describe their behavior.

// char __usercall<al>(_DWORD *a1<esi>)
static const void *const IsByteswappedPtr = (void*)0x429840;
static inline char IsByteswapped(void *a1)
{
	char result;
	__asm
	{
		mov esi, [a1]
		call IsByteswappedPtr
		mov result, al
	}
	return result;
}

// signed int __usercall<eax>(int character<ecx>, Vertex *position<edi>, Rotation *rotation)
static const void *const LoadStartPositionPtr = (void*)0x43D8E0;
static inline signed int LoadStartPosition(int character, Vertex *position, Rotation *rotation)
{
	signed int result;
	__asm
	{
		push[rotation]
		mov edi, [position]
		mov ecx, [character]
		call LoadStartPositionPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// void __usercall(int a1<eax>)
static const void *const InitPlayerPtr = (void*)0x43DA40;
static inline void InitPlayer(int a1)
{
	__asm
	{
		mov eax, [a1]
		call InitPlayerPtr
	}
}

// void __usercall(int a1<eax>)
static const void *const Load2PIntroPosPtr = (void*)0x43DBD0;
static inline void Load2PIntroPos(int a1)
{
	__asm
	{
		mov eax, [a1]
		call Load2PIntroPosPtr
	}
}

// signed int __usercall<eax>(int playerNum<eax>)
static const void *const LoadEndPositionPtr = (void*)0x43DD50;
static inline signed int LoadEndPosition(int playerNum)
{
	signed int result;
	__asm
	{
		mov eax, [playerNum]
		call LoadEndPositionPtr
		mov result, eax
	}
	return result;
}

// void __usercall(const char *song<edi>)
static const void *const PlayMusicPtr = (void*)0x442CF0;
// Sets the next song to play after the next call to StopMusic()
// followed by ResetMusic()
static inline void PlayMusic(const char *song)
{
	__asm
	{
		mov edi, [song]
		call PlayMusicPtr
	}
}
static const void *const PlayJinglePtr = (void*)0x00443480;
// Plays specified song once, then restores previous song as set by PlayMusic.
static inline void PlayJingle(int a1, const char *song)
{
	__asm
	{
		mov ecx, [a1]
		mov ebx, [song]
		call PlayJinglePtr
	}
}
static const void *const PlayMusicOncePtr = (void*)0x00442EF0;
// Plays the specified song once.
// Takes effect immediately.
static inline void PlayMusicOnce(void *a1, const char *song)
{
	__asm
	{
		mov ecx, [a1]
		mov edi, [song]
		call PlayMusicOncePtr
	}
}
static const void *const _PlayMusicOncePtr = (void*)0x00442E60;
// Plays the specified song once.
// Requires StopMusic and ResetMusic to be called.
static inline void _PlayMusicOnce(const char *song)
{
	__asm
	{
		mov edi, [song]
		call _PlayMusicOncePtr
	}
}
// Immediately changes the music to the specified song.
// Automatically calls StopMusic() and ResetMusic().
inline void ChangeMusic(const char* song)
{
	StopMusic();
	PlayMusic(song);
	ResetMusic();
}

// signed int __usercall<eax>(int a1<edx>, int a2)
static const void *const PlayVoicePtr = (void*)0x443130;
static inline signed int PlayVoice(int a1, int a2)
{
	signed int result;
	__asm
	{
		push[a2]
		mov edx, [a1]
		call PlayVoicePtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// void __usercall(char a1<al>, __int16 a2<bx>)
static const void *const AddLivesPtr = (void*)0x44CB10;
static inline void AddLives(char a1, __int16 a2)
{
	__asm
	{
		mov bx, [a2]
		mov al, [a1]
		call AddLivesPtr
	}
}

// void __usercall(char playerNum<al>, int numRings<edx>)
static const void *const AddRingsPtr = (void*)0x44CE10;
static inline void AddRings(char playerNum, int numRings)
{
	__asm
	{
		mov edx, [numRings]
		mov al, [playerNum]
		call AddRingsPtr
	}
}

// ModelIndex *__usercall<eax>(char *filename<eax>)
static const void *const LoadMDLFilePtr = (void*)0x459590;
static inline ModelIndex * LoadMDLFile(char *filename)
{
	ModelIndex * result;
	__asm
	{
		mov eax, [filename]
		call LoadMDLFilePtr
		mov result, eax
	}
	return result;
}

// void __usercall(ModelIndex *a1<esi>)
static const void *const ReleaseMDLFilePtr = (void*)0x4596D0;
static inline void ReleaseMDLFile(ModelIndex *a1)
{
	__asm
	{
		mov esi, [a1]
		call ReleaseMDLFilePtr
	}
}

// AnimationIndex *__usercall<eax>(char *filename<eax>)
static const void *const LoadMTNFilePtr = (void*)0x459740;
static inline AnimationIndex * LoadMTNFile(char *filename)
{
	AnimationIndex * result;
	__asm
	{
		mov eax, [filename]
		call LoadMTNFilePtr
		mov result, eax
	}
	return result;
}

// signed int __usercall<eax>(DeathZone *a1<ebx>)
static const void *const LoadDeathZonesPtr = (void*)0x46B090;
static inline signed int LoadDeathZones(DeathZone *a1)
{
	signed int result;
	__asm
	{
		mov ebx, [a1]
		call LoadDeathZonesPtr
		mov result, eax
	}
	return result;
}

// void __usercall(int playerNum<ebx>)
static const void *const KillCharacterPtr = (void*)0x46B110;
static inline void KillCharacter(int playerNum)
{
	__asm
	{
		mov ebx, [playerNum]
		call KillCharacterPtr
	}
}

// ObjectMaster *__usercall<eax>(int list<ecx>, char *name<eax>, void (__cdecl *mainSub)(ObjectMaster *)<edi>, char flags)
static const void *const LoadObject2Ptr = (void*)0x46F610;
static inline ObjectMaster * LoadObject2(int list, char *name, void(__cdecl *mainSub)(ObjectMaster *), char flags)
{
	ObjectMaster * result;
	__asm
	{
		xor eax, eax
		mov al, [flags]
		push eax
		mov edi, [mainSub]
		mov eax, [name]
		mov ecx, [list]
		call LoadObject2Ptr
		add esp, 4
		mov result, eax
	}
	return result;
}

// ObjectMaster *__usercall<eax>(void (__cdecl *mainSub)(ObjectMaster *)<edi>, int list<esi>, char *name)
static const void *const LoadObjectPtr = (void*)0x46F680;
static inline ObjectMaster * LoadObject(void(__cdecl *mainSub)(ObjectMaster *), int list, char *name)
{
	ObjectMaster * result;
	__asm
	{
		push[name]
		mov esi, [list]
		mov edi, [mainSub]
		call LoadObjectPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// signed int __usercall<eax>(ObjectMaster *a1<eax>, CollisionData *a2, int count, unsigned __int8 a4)
static const void *const InitCollisionPtr = (void*)0x47E520;
static inline signed int InitCollision(ObjectMaster *a1, void *a2, int count, unsigned __int8 a4)
{
	signed int result;
	__asm
	{
		xor eax, eax
		mov al, [a4]
		push eax
		push[count]
		push[a2]
		mov eax, [a1]
		call InitCollisionPtr
		add esp, 12
		mov result, eax
	}
	return result;
}

// void *__usercall<eax>(int a1<eax>, char *name_s<ecx>, char *name_u)
static const void *const LoadSETFilePtr = (void*)0x488DD0;
static inline void * LoadSETFile(int a1, char *name_s, char *name_u)
{
	void * result;
	__asm
	{
		push[name_u]
		mov ecx, [name_s]
		mov eax, [a1]
		call LoadSETFilePtr
		add esp, 4
		mov result, eax
	}
	return result;
}

// ObjectMaster *__usercall<eax>(ObjectMaster *parent<eax>, Vertex *position<ebx>, int a1, int a2)
static const void *const SpawnBombPtr = (void*)0x513FE0;
static inline ObjectMaster * SpawnBomb(ObjectMaster *parent, Vertex *position, int a1, int a2)
{
	ObjectMaster * result;
	__asm
	{
		push[a2]
		push[a1]
		mov ebx, [position]
		mov eax, [parent]
		call SpawnBombPtr
		add esp, 8
		mov result, eax
	}
	return result;
}

// void __usercall(char *a1<esi>)
static const void *const WriteChaoSaveChecksumPtr = (void*)0x52EEE0;
static inline void WriteChaoSaveChecksum(char *a1)
{
	__asm
	{
		mov esi, [a1]
		call WriteChaoSaveChecksumPtr
	}
}

// void __usercall(Vertex *a1<ebx>, float a2, float a3)
static const void *const LoadChaoKeyPtr = (void*)0x6E9D10;
static inline void LoadChaoKey(Vertex *a1, float a2, float a3)
{
	__asm
	{
		push[a3]
		push[a2]
		mov ebx, [a1]
		call LoadChaoKeyPtr
		add esp, 8
	}
}

// bool __usercall<eax>(CharObj1 *a1<eax>, SonicCharObj2 *a2<ecx>)
static const void *const Knuckles_LevelBoundsPtr = (void*)0x737B50;
static inline bool Knuckles_LevelBounds(CharObj1 *a1, KnucklesCharObj2 *a2)
{
	bool result;
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call Knuckles_LevelBoundsPtr
		mov result, al
	}
	return result;
}

// void __usercall(int playerNum<eax>)
static const void *const LoadEggmanPtr = (void*)0x73C220;
static inline void LoadEggman(int playerNum)
{
	__asm
	{
		mov eax, [playerNum]
		call LoadEggmanPtr
	}
}

// void __usercall(CharObj2Base *a1<eax>, CharObj1 *a2<edi>)
static const void *const FreeMovement_MechEggmanPtr = (void*)0x749020;
static inline void FreeMovement_MechEggman(CharObj2Base *a1, CharObj1 *a2)
{
	__asm
	{
		mov edi, [a2]
		mov eax, [a1]
		call FreeMovement_MechEggmanPtr
	}
}

// void __usercall(int playerNum<eax>)
static const void *const LoadTailsPtr = (void*)0x74CF00;
static inline void LoadTails(int playerNum)
{
	__asm
	{
		mov eax, [playerNum]
		call LoadTailsPtr
	}
}

// void __usercall(CharObj2Base *a1<eax>, CharObj1 *a2<edi>)
static const void *const FreeMovement_TailsPtr = (void*)0x7519B0;
static inline void FreeMovement_Tails(CharObj2Base *a1, CharObj1 *a2)
{
	__asm
	{
		mov edi, [a2]
		mov eax, [a1]
		call FreeMovement_TailsPtr
	}
}

// void *__usercall<eax>(char *filename<eax>)
static const void *const LoadPRSFilePtr = (void*)0x4548C0;
static inline void *LoadPRSFile(const char *filename)
{
	void *result;
	__asm
	{
		mov eax, [filename]
		call LoadPRSFilePtr
		mov result, eax
	}
	return result;
}

//double __usercall CheckDistance@<st0>(NJS_VECTOR *a1@<eax>, NJS_VECTOR *a2@<ecx>)
static const void* const CheckDistancePtr = (void*)0x0077FBD0;
static inline double CheckDistance(NJS_VECTOR* a1, NJS_VECTOR* a2)
{
	double result;
	__asm
	{
		mov eax, [a1]
		mov ecx, [a2]
		call CheckDistancePtr
		fstp result
	}
	return result;
}