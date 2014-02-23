#include <cstdint>
#include "ninja.h"
#ifndef SA2MODLOADER_H
#define SA2MODLOADER_H

static const int ModLoaderVer = 2;

#define arrayptrandlength(data) data, LengthOfArray(data)
#define arraylengthandptr(data) LengthOfArray(data), data
#define arrayptrandsize(data) data, SizeOfArray(data)
#define arraysizeandptr(data) SizeOfArray(data), data

struct PatchInfo
{
	void *address;
	void *data;
	int datasize;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }

struct PointerInfo
{
	void *address;
	void *data;
};

#define ptrdecl(address,data) { (void*)address, (void*)data }

struct ExportInfo
{
	const char *const name;
	void *data;
};

struct ModInfo
{
	int Version;
	void (*Init)(const char *path);
	PatchInfo *Patches;
	int PatchCount;
	PointerInfo *Jumps;
	int JumpCount;
	PointerInfo *Calls;
	int CallCount;
	PointerInfo *Pointers;
	int PointerCount;
	ExportInfo *Exports;
	int ExportCount;
};

// Utility Functions
template <typename T, size_t N>
inline size_t LengthOfArray( const T(&)[ N ] )
{
	return N;
}

template <typename T, size_t N>
inline size_t SizeOfArray( const T(&)[ N ] )
{
	return N * sizeof(T);
}

static BOOL WriteData(void *writeaddress, void *data, SIZE_T datasize, SIZE_T *byteswritten)
{
	return WriteProcessMemory(GetCurrentProcess(), writeaddress, data, datasize, byteswritten);
}

static BOOL WriteData(void *writeaddress, void *data, SIZE_T datasize)
{
	SIZE_T written;
	return WriteData(writeaddress, data, datasize, &written);
}

template<typename T> static BOOL WriteData(void *writeaddress, T data, SIZE_T *byteswritten)
{
	return WriteData(writeaddress, &data, sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(void *writeaddress, T data)
{
	SIZE_T written;
	return WriteData(writeaddress, data, &written);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, T(&data)[N], SIZE_T *byteswritten)
{
	return WriteData(writeaddress, data, SizeOfArray(data), byteswritten);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, T(&data)[N])
{
	SIZE_T written;
	return WriteData(writeaddress, data, &written);
}

static BOOL WriteJump(void *writeaddress, void *funcaddress)
{
	unsigned char data[5];
	data[0] = 0xE9;
	*(signed int *)(data + 1) = (unsigned int)funcaddress - ((unsigned int)writeaddress + 5);
	return WriteData(writeaddress, data);
}

static BOOL WriteCall(void *writeaddress, void *funcaddress)
{
	unsigned char data[5];
	data[0] = 0xE8;
	*(signed int *)(data + 1) = (unsigned int)funcaddress - ((unsigned int)writeaddress + 5);
	return WriteData(writeaddress, data);
}

// SA2 Enums
#define makemask(enumname,membername) enumname##_##membername = (1 << enumname##Bits_##membername)
#define makemasks(enumname,membername) enumname##s_##membername = (1 << enumname##Bits_##membername)

enum Characters
{
	Characters_Sonic,
	Characters_Shadow,
	Characters_Tails,
	Characters_Eggman,
	Characters_Knuckles,
	Characters_Rouge,
	Characters_MechTails,
	Characters_MechEggman,
	Characters_Amy,
	Characters_SuperSonic,
	Characters_SuperShadow,
	Characters_B,
	Characters_MetalSonic,
	Characters_ChaoWalker,
	Characters_DarkChaoWalker,
	Characters_Tikal,
	Characters_Chaos
};

enum LevelIDs
{
	LevelIDs_BasicTest,
	LevelIDs_KnucklesTest,
	LevelIDs_SonicTest,
	LevelIDs_GreenForest,
	LevelIDs_WhiteJungle,
	LevelIDs_PumpkinHill,
	LevelIDs_SkyRail,
	LevelIDs_AquaticMine,
	LevelIDs_SecurityHall,
	LevelIDs_PrisonLane,
	LevelIDs_MetalHarbor,
	LevelIDs_IronGate,
	LevelIDs_WeaponsBed,
	LevelIDs_CityEscape,
	LevelIDs_RadicalHighway,
	LevelIDs_WeaponsBed2P,
	LevelIDs_WildCanyon,
	LevelIDs_MissionStreet,
	LevelIDs_DryLagoon,
	LevelIDs_SonicVsShadow1,
	LevelIDs_TailsVsEggman1,
	LevelIDs_SandOcean,
	LevelIDs_CrazyGadget,
	LevelIDs_HiddenBase,
	LevelIDs_EternalEngine,
	LevelIDs_DeathChamber,
	LevelIDs_EggQuarters,
	LevelIDs_LostColony,
	LevelIDs_PyramidCave,
	LevelIDs_TailsVsEggman2,
	LevelIDs_FinalRush,
	LevelIDs_GreenHill,
	LevelIDs_MeteorHerd,
	LevelIDs_KnucklesVsRouge,
	LevelIDs_CannonsCoreS,
	LevelIDs_CannonsCoreE,
	LevelIDs_CannonsCoreT,
	LevelIDs_CannonsCoreR,
	LevelIDs_CannonsCoreK,
	LevelIDs_MissionStreet2P,
	LevelIDs_FinalChase,
	LevelIDs_WildCanyon2P,
	LevelIDs_SonicVsShadow2,
	LevelIDs_CosmicWall,
	LevelIDs_MadSpace,
	LevelIDs_SandOcean2P,
	LevelIDs_DryLagoon2P,
	LevelIDs_PyramidRace,
	LevelIDs_HiddenBase2P,
	LevelIDs_PoolQuest,
	LevelIDs_PlanetQuest,
	LevelIDs_DeckRace,
	LevelIDs_DowntownRace,
	LevelIDs_CosmicWall2P,
	LevelIDs_GrindRace,
	LevelIDs_LostColony2P,
	LevelIDs_EternalEngine2P,
	LevelIDs_MetalHarbor2P,
	LevelIDs_IronGate2P,
	LevelIDs_DeathChamber2P,
	LevelIDs_BigFoot,
	LevelIDs_HotShot,
	LevelIDs_FlyingDog,
	LevelIDs_KingBoomBoo,
	LevelIDs_EggGolemS,
	LevelIDs_Biolizard,
	LevelIDs_FinalHazard,
	LevelIDs_EggGolemE,
	LevelIDs_Route101280 = 70,
	LevelIDs_KartRace,
	LevelIDs_ChaoWorld = 90,
	LevelIDs_Invalid
};

enum UpgradeBits : int
{
	UpgradeBits_SonicLightShoes,
	UpgradeBits_SonicAncientLight,
	UpgradeBits_SonicMagicGloves,
	UpgradeBits_SonicFlameRing,
	UpgradeBits_SonicBounceBracelet,
	UpgradeBits_SonicMysticMelody,
	UpgradeBits_TailsBooster,
	UpgradeBits_TailsBazooka,
	UpgradeBits_TailsLaserBlaster,
	UpgradeBits_TailsMysticMelody,
	UpgradeBits_KnucklesShovelClaw,
	UpgradeBits_KnucklesSunglasses,
	UpgradeBits_KnucklesHammerGloves,
	UpgradeBits_KnucklesAirNecklace,
	UpgradeBits_KnucklesMysticMelody,
	UpgradeBits_SuperSonic,
	UpgradeBits_ShadowAirShoes,
	UpgradeBits_ShadowAncientLight,
	UpgradeBits_ShadowFlameRing,
	UpgradeBits_ShadowMysticMelody,
	UpgradeBits_EggmanJetEngine,
	UpgradeBits_EggmanLargeCannon,
	UpgradeBits_EggmanLaserBlaster,
	UpgradeBits_EggmanProtectiveArmor,
	UpgradeBits_EggmanMysticMelody,
	UpgradeBits_RougePickNails,
	UpgradeBits_RougeTreasureScope,
	UpgradeBits_RougeIronBoots,
	UpgradeBits_RougeMysticMelody
};

enum Upgrades : int
{
	makemasks(Upgrade, SonicLightShoes),
	makemasks(Upgrade, SonicAncientLight),
	makemasks(Upgrade, SonicMagicGloves),
	makemasks(Upgrade, SonicFlameRing),
	makemasks(Upgrade, SonicBounceBracelet),
	makemasks(Upgrade, SonicMysticMelody),
	makemasks(Upgrade, TailsBooster),
	makemasks(Upgrade, TailsBazooka),
	makemasks(Upgrade, TailsLaserBlaster),
	makemasks(Upgrade, TailsMysticMelody),
	makemasks(Upgrade, KnucklesShovelClaw),
	makemasks(Upgrade, KnucklesSunglasses),
	makemasks(Upgrade, KnucklesHammerGloves),
	makemasks(Upgrade, KnucklesAirNecklace),
	makemasks(Upgrade, KnucklesMysticMelody),
	makemasks(Upgrade, SuperSonic),
	makemasks(Upgrade, ShadowAirShoes),
	makemasks(Upgrade, ShadowAncientLight),
	makemasks(Upgrade, ShadowFlameRing),
	makemasks(Upgrade, ShadowMysticMelody),
	makemasks(Upgrade, EggmanJetEngine),
	makemasks(Upgrade, EggmanLargeCannon),
	makemasks(Upgrade, EggmanLaserBlaster),
	makemasks(Upgrade, EggmanProtectiveArmor),
	makemasks(Upgrade, EggmanMysticMelody),
	makemasks(Upgrade, RougePickNails),
	makemasks(Upgrade, RougeTreasureScope),
	makemasks(Upgrade, RougeIronBoots),
	makemasks(Upgrade, RougeMysticMelody)
};

enum PowerupBits
{
	PowerupBits_Barrier,
	PowerupBits_MagneticBarrier,
	PowerupBits_Invincibility = 15
};

enum Powerups : unsigned short
{
	makemasks(Powerup, Barrier),
	makemasks(Powerup, MagneticBarrier),
	makemasks(Powerup, Invincibility)
};

enum ButtonBits
{
	ButtonBits_B = 1,
	ButtonBits_A,
	ButtonBits_Start,
	ButtonBits_Up,
	ButtonBits_Down,
	ButtonBits_Left,
	ButtonBits_Right,
	ButtonBits_Z,
	ButtonBits_Y,
	ButtonBits_X,
	ButtonBits_R = 16,
	ButtonBits_L
};

enum Buttons : int
{
	makemasks(Button, B),
	makemasks(Button, A),
	makemasks(Button, Start),
	makemasks(Button, Up),
	makemasks(Button, Down),
	makemasks(Button, Left),
	makemasks(Button, Right),
	makemasks(Button, Z),
	makemasks(Button, Y),
	makemasks(Button, X),
	makemasks(Button, R),
	makemasks(Button, L)
};

// SA2 Structs
typedef NJS_VECTOR Vertex;
typedef NJS_TEXLIST TexList;

#pragma pack(push, 1)
struct Rotation {
	int x, y, z;
};

struct struct_v5
{
  __int16 char0;
  char gap_2[2];
  short word4;
  short word6;
  float f8;
  void *dwordC;
  char f10[140];
  int dword9C;
  __int16 field_A0;
  __int16 field_A2;
  int field_A4;
};

struct CharObj1 {
	char Action;
	char field_1;
	char field_2;
	char field_3;
	unsigned short Status;
	char field_6;
	char field_7;
	Rotation Rotation;
	Vertex Position;
	Vertex Scale;
	struct_v5 *field_2C;
};

struct PhysicsData
{
	int HangTime;
	float anonymous_0;
	float HSpeedCap;
	float VSpeedCap;
	float BaseSpeed;
	float anonymous_4;
	float InitialJumpSpeed;
	float anonymous_6;
	float anonymous_7;
	float anonymous_8;
	float anonymous_9;
	float anonymous_10;
	float anonymous_11;
	float anonymous_12;
	float JumpHoldSpeed;
	float GroundAccel;
	float AirAccel;
	float GroundDecel;
	float BrakeSpeed;
	float AirBrakeSpeed;
	float AirDecel;
	float anonymous_20;
	float anonymous_21;
	float anonymous_22;
	float anonymous_23;
	float anonymous_24;
	float anonymous_25;
	float anonymous_26;
	float anonymous_27;
	float CollisionSize;
	float Gravity;
	float CameraYOff;
	float YOff;
};

struct AnimationInfo
{
	__int16 AnimNum;
	__int16 ModelNum;
	__int16 anonymous_2;
	__int16 NextAnimation;
	float TransitionSpeed;
	float AnimationSpeed;
};

struct CharAnimInfo
{
	__int16 AnimationFrame;
	__int16 Animation;
	__int16 Animation2;
	__int16 Animation3;
	__int16 field_8;
	__int16 field_A;
	__int16 field_C;
	char field_E;
	char field_F;
	float field_10;
	float field_14;
	char field_18;
	char field_19;
	char field_1A;
	char field_1B;
	char field_1C;
	char field_1D;
	char field_1E;
	char field_1F;
	char field_20;
	char field_21;
	char field_22;
	char field_23;
	AnimationInfo *Animations;
	void *field_28;
};

struct ModelIndex
{
	int Index;
	NJS_OBJECT *Model;
};

struct AnimationIndex
{
	short Index;
	short Count;
	NJS_MOTION *Animation;
};

struct CharObj2 {
	char PlayerNum;
	char CharID;
	char field_2;
	char CharID2;
	char ActionWindowItems[8];
	char ActionWindowItemCount;
	char field_D[3];
	Powerups Powerups;
	char field_12[18];
	Upgrades Upgrades;
	char field_28[32];
	int field_48;
	char field_4C[24];
	float HSpeed;
	float VSpeed;
	char field_6C[84];
	PhysicsData PhysData;
	char field_144[48];
	CharAnimInfo AnimInfo;
	char field_1A0[20];
	float field_1B4;
};

struct SonicCharObj2 : CharObj2 {
	char field_1B8[476];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct KnucklesCharObj2 : CharObj2 {
	char field_1B8[568];
	TexList *TextureList;
	TexList *EffectTextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_400[32];
};

struct EggmanCharObj2 : CharObj2 {
	char field_1B8[424];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct MechEggmanCharObj2 : CharObj2 {
	char field_1B8[652];
	TexList *CommonTextureList;
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct TailsCharObj2 : CharObj2 {
	char field_1B8[504];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_3BC[36];
};

struct SuperSonicCharObj2 : CharObj2 {
	char field_1B8[440];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct SETEntry
{
	__int16 ID;
	__int16 XRot;
	__int16 YRot;
	__int16 ZRot;
	Vertex Position;
	Vertex Scale;
};

struct ObjectMaster;
typedef void (__cdecl *ObjectFuncPtr)(ObjectMaster *);

struct SETObjectData
{
	char LoadCount;
	char field_1;
	__int16 Flags;
	ObjectMaster *Object;
	SETEntry *SETEntry;
	float field_C;
};

struct ObjectMaster {
	ObjectMaster *PrevObject;
	ObjectMaster *NextObject;
	void *field_8;
	void *field_C;
	ObjectFuncPtr MainSub;
	ObjectFuncPtr DisplaySub;
	ObjectFuncPtr field_18;
	ObjectFuncPtr field_1C;
	void *field_20;
	void *field_24;
	void *field_28;
	void *field_2C;
	SETObjectData *field_30;
	CharObj1 *Data1;
	void *field_38;
	void *field_3C;
	CharObj2 *Data2;
	char *Name;
	void *field_48;
	void *field_4C;
};

struct ObjectListEntry
{
	char Flags;
	char List;
	__int16 anonymous_2;
	float anonymous_3;
	ObjectFuncPtr Function;
	char *Name;
};

struct ObjectListHead
{
	int Count;
	ObjectListEntry *List;
};

struct LevelItemData
{
	int Character;
	char *Name;
	int Mask;
	int Index;
	int field_10;
	void *anonymous_1;
	void *anonymous_2;
	void *anonymous_3;
	int field_20;
};

struct COL
{
	Vertex Center;
	float Radius;
	NJS_OBJECT *Model;
	int field_14;
	int field_18;
	int Flags;
};

struct LandTable
{
	__int16 COLCount;
	__int16 ChunkModelCount;
	__int16 field_4;
	__int16 field_6;
	__int16 field_8;
	__int16 field_A;
	float field_C;
	COL *COLList;
	float *field_14;
	char *TextureName;
	TexList *TextureList;
};

struct StartPosition
{
	__int16 Level;
	__int16 Rotation1P;
	__int16 RotationP1;
	__int16 RotationP2;
	Vertex Position1P;
	Vertex PositionP1;
	Vertex PositionP2;
};

struct StageSelectLevel
{
	int Level;
	int Character;
	int Column;
	int Row;
};

struct LevelHeader
{
	char Name[8];
	int field_8;
	void (__cdecl *Init)();
	void *anonymous_2;
	void *anonymous_3;
};

struct ChaoItemStats
{
	__int16 Mood;
	__int16 Belly;
	__int16 Swim;
	__int16 Fly;
	__int16 Run;
	__int16 Power;
	__int16 Stamina;
	__int16 Luck;
	__int16 Intelligence;
	__int16 anonymous_7;
};

struct LevelCutscene
{
	__int16 Level;
	__int16 Character;
	__int16 Cutscene;
};

typedef struct
{
	char *Name;
	void (__cdecl *Prolog)();
	void (__cdecl *Epilog)();
} ChaoSegmentData, RELFileInfo;

struct MemManFuncs
{
	void *(__cdecl *Allocate)(int size, char *file, int line);
	void (__cdecl *Deallocate)(void *data, char *file, int line);
	void *anonymous_2;
	void *anonymous_3;
};

struct ControllerData
{
	uint32_t ID;
	uint32_t Support;
	uint32_t HeldButtons;
	uint32_t NotHeldButtons;
	uint32_t PressedButtons;
	uint32_t ReleasedButtons;
	uint16_t RTriggerPressure;
	uint16_t LTriggerPressure;
	int16_t LeftStickX;
	int16_t LeftStickY;
	int16_t RightStickX;
	int16_t RightStickY;
	char *Name;
	void *Extend;
	uint32_t Old;
	void *Info;
};

struct ChaoData
{
	char char0;
	char gap_1[17];
	char Name[7];
	char gap_19[7];
	char SwimFraction;
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char LuckFraction;
	char IntelligenceFraction;
	char gap_27[1];
	char SwimGrade;
	char FlyGrade;
	char RunGrade;
	char PowerGrade;
	char StaminaGrade;
	char LuckGrade;
	char IntelligenceGrade;
	char gap_2f[1];
	char SwimLevel;
	char FlyLevel;
	char RunLevel;
	char PowerLevel;
	char StaminaLevel;
	char LuckLevel;
	char IntelligenceLevel;
	char gap_37[1];
	__int16 SwimStat;
	__int16 FlyStat;
	__int16 RunStat;
	__int16 PowerStat;
	__int16 StaminaStat;
	__int16 LuckStat;
	__int16 IntelligenceStat;
	char gap_46[58];
	char Type;
	char Garden;
	char Reincarnate;
	char gap_83[3];
	char ClockRollovers;
	char gap_87[3];
	__int16 LifeTimeLeft;
	char gap_8c[28];
	float PowerRun;
	float FlySwim;
	float HeroDark;
	char gap_b4[12];
	float EvolutionProgress;
	char gap_c4[13];
	char EyeType;
	char MouthType;
	char BallType;
	char gap_d4[1];
	char Headgear;
	char HideFeet;
	char Medal;
	char Color;
	char Highlights;
	char Texture;
	char Shiny;
	char EggColor;
	char Model;
	char byteDE;
	char gap_df[1824];
	char field_7FF;
};

struct TexPackInfo
{
	char *TexName;
	TexList *TexList;
};

struct LevelRankScores
{
	__int16 Level;
	__int16 DRank;
	__int16 CRank;
	__int16 BRank;
	__int16 ARank;
};

struct MinSec
{
	char Minutes;
	char Seconds;
};

struct LevelRankTimes
{
	char Level;
	MinSec DRank;
	MinSec CRank;
	MinSec BRank;
	MinSec ARank;
};

struct LevelEndPosition
{
	__int16 Level;
	__int16 Mission2YRotation;
	__int16 Mission3YRotation;
	__int16 field_6;
	Vertex Mission2Position;
	Vertex Mission3Position;
};

struct CharacterSoundBank
{
	int Character;
	char *Name;
	void *anonymous_1;
};

struct CharacterVoiceBank
{
	char anonymous_0;
	char Character;
	char anonymous_2[2];
	void *anonymous_3;
	void *anonymous_4;
	void *anonymous_5;
};

struct DeathZone
{
	int Flags;
	NJS_OBJECT *Model;
};

struct ModelFileHeader
{
	int Magic : 24;
	int Version : 8;
	NJS_OBJECT *Model;
	void *Metadata;
};

struct LevelFileHeader
{
	long long Magic : 56;
	long long Version : 8;
	LandTable *LandTable;
	void *Metadata;
};

struct AnimationFileHeader
{
	long long Magic : 56;
	long long Version : 8;
	NJS_MOTION *Animation;
	char *Name;
	int ModelNum;
};
#pragma pack(pop)

// SA2 Variables
#define DataPointer(type,name,address) static type &name = *(type *)address
#define DataArray(type,name,address,length) static type *const name = (type *)address; static const int name##_Length = length
DataArray(AnimationInfo, TailsAnimList, 0xA0D608, 118);
DataArray(AnimationInfo, MechEggmanAnimList, 0xA0E1F0, 139);
DataArray(AnimationInfo, MechTailsAnimList, 0xA0EAA0, 190);
DataArray(AnimationInfo, ChaoWalkerAnimList, 0xA0F680, 198);
DataArray(AnimationInfo, DarkChaoWalkerAnimList, 0xA102E0, 202);
DataArray(AnimationInfo, EggmanAnimList, 0xA12750, 164);
DataArray(AnimationInfo, KnucklesAnimList, 0xA13320, 217);
DataArray(AnimationInfo, TikalAnimList, 0xA140B0, 260);
DataArray(AnimationInfo, RougeAnimList, 0xA150F0, 217);
DataArray(AnimationInfo, ChaosAnimList, 0xA15E80, 272);
DataArray(AnimationInfo, SonicAnimList, 0xA17C18, 203);
DataArray(AnimationInfo, ShadowAnimList, 0xA188C8, 249);
DataArray(AnimationInfo, AmyAnimList, 0xA19858, 249);
DataArray(AnimationInfo, MetalSonicAnimList, 0xA1A7E8, 249);
DataArray(AnimationInfo, SuperSonicAnimList, 0x170D228, 110);
DataArray(AnimationInfo, SuperShadowAnimList, 0x170D908, 117);
DataArray(AnimationIndex, HiddenBaseCharAnims, 0xA93F28, 3);
DataArray(AnimationIndex, CannonsCoreKCharAnims, 0xCCDEB8, 7);
DataArray(AnimationIndex, SandOceanCharAnims, 0xCF9980, 3);
DataArray(AnimationIndex, DryLagoonCharAnims, 0xDEA388, 11);
DataArray(AnimationIndex, SandOcean2PCharAnims, 0x1413218, 3);
DataArray(AnimationIndex, DryLagoon2PPoolQuestCharAnims, 0x15623B8, 13);
DataArray(AnimationIndex, CannonsCoreRCharAnims, 0x15AFBC8, 7);
DataArray(AnimationIndex, AquaticMineCharAnims, 0x16383A0, 8);
DataArray(AnimationIndex, EggGolemECharAnims, 0x16D1E30, 2);
DataArray(char *, MusicList, 0x171A2B0, 156);
DataPointer(char, TimerMinutes, 0x174AFDB);
DataPointer(char, TimerSeconds, 0x174AFDC);
DataPointer(char, TwoPlayerMode, 0x174AFDE);
DataPointer(char, MissionNum, 0x174AFE3);
DataArray(char, P1SpecialAttacks, 0x174AFED, 3);
DataPointer(char, P1FirstSpecialAttack, &P1SpecialAttacks[0]);
DataPointer(char, P1SecondSpecialAttack, &P1SpecialAttacks[1]);
DataPointer(char, P1ThirdSpecialAttack, &P1SpecialAttacks[2]);
DataArray(char, P2SpecialAttacks, 0x174AFF0, 3);
DataPointer(char, P2FirstSpecialAttack, &P2SpecialAttacks[0]);
DataPointer(char, P2SecondSpecialAttack, &P2SpecialAttacks[1]);
DataPointer(char, P2ThirdSpecialAttack, &P2SpecialAttacks[2]);
DataPointer(char, HaveChaoKey, 0x174B004);
DataArray(char, AltCostume, 0x174B013, 2);
DataArray(char, AltCharacter, 0x174B015, 2);
DataArray(short, RingCount, 0x174B028, 2);
DataPointer(short, PerfectRings, 0x174B02E);
DataPointer(int, CurrentLevel, 0x1934B70);
DataPointer(int, CurrentCharacter, 0x1934B80);
DataPointer(int, CurrentCharacter2P, 0x1934BE4);
DataArray(ControllerData, ControllersRaw, 0x1A52918, 4);
DataPointer(ControllerData, Controller1Raw, &ControllersRaw[0]);
DataPointer(ControllerData, Controller2Raw, &ControllersRaw[0]);
DataPointer(ControllerData, Controller3Raw, &ControllersRaw[0]);
DataPointer(ControllerData, Controller4Raw, &ControllersRaw[0]);
DataArray(CharObj2 *, MainCharObj2, 0x1DE9600, 2);
DataArray(ModelIndex, CharacterModels, 0x1DE9620, 532);
DataArray(CharObj1 *, MainCharObj1, 0x1DEA6C0, 2);
DataArray(ObjectMaster *, MainCharacter, 0x1DEA6E0, 2);
DataArray(AnimationIndex, CharacterAnimations, 0x1DEA700, 300);
DataArray(uint32_t, MenuPressedButtons, 0x1DEFB10, 4);
DataArray(ControllerData, Controllers, 0x1DEFC00, 4);
DataPointer(ControllerData, Controller1, &Controllers[0]);
DataPointer(ControllerData, Controller2, &Controllers[0]);
DataPointer(ControllerData, Controller3, &Controllers[0]);
DataPointer(ControllerData, Controller4, &Controllers[0]);

// SA2 Functions
#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__cdecl *const NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS
#define StdcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__stdcall *const NAME)ARGS = (RETURN_TYPE (*)ARGS)ADDRESS
#define FastcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__fastcall *const NAME)ARGS = (RETURN_TYPE (__fastcall *)ARGS)ADDRESS
#define ThiscallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__thiscall *const NAME)ARGS = (RETURN_TYPE (__thiscall *)ARGS)ADDRESS
#define VoidFunc(name,address) FunctionPointer(void,name,(void),address)
#define ObjectFunc(name,address) FunctionPointer(void,name,(ObjectMaster *obj),address)
#define CharLoadFunc(name,address) FunctionPointer(void,name,(int playerNum),address)
FunctionPointer(int, PrintDebug, (char *format, ...), 0x426740);
CharLoadFunc(LoadSonic, 0x716E00);
CharLoadFunc(LoadAmy, 0x7170E0);
CharLoadFunc(LoadShadow, 0x717360);
CharLoadFunc(LoadMetalSonic, 0x717640);
CharLoadFunc(LoadKnuckles, 0x728110);
CharLoadFunc(LoadTikal, 0x7288B0);
CharLoadFunc(LoadRouge, 0x728460);
CharLoadFunc(LoadChaos, 0x728B60);
CharLoadFunc(LoadMechEggman, 0x740C50);
CharLoadFunc(LoadDarkChaoWalker, 0x7412F0);
CharLoadFunc(LoadMechTails, 0x740EB0);
CharLoadFunc(LoadChaoWalker, 0x741110);
FunctionPointer(signed int, ProcessChunkModel, (NJS_CNK_MODEL *a1), 0x42D650);
VoidFunc(LoadCharacters, 0x43D630);
FastcallFunctionPointer(signed int, LoadTexturePack, (char *filename, NJS_TEXLIST *texlist), 0x44C350);
ThiscallFunctionPointer(NJS_TEXLIST *, LoadCharTextures, (char *filename), 0x44C510);
ObjectFunc(DeathZoneObject, 0x459740);
ObjectFunc(DeleteObject_, 0x46F720);
FunctionPointer(CharObj1 *, LoadCharObj1, (void), 0x470B40);
FunctionPointer(signed int, LoadLandManager, (LandTable *a1), 0x47BD30);
FunctionPointer(signed int, LoadSetObject, (ObjectListHead *list, void *setfile), 0x487E40);
ObjectFunc(SETObject_Main, 0x487F60);
ThiscallFunctionPointer(void *, LoadStageSETFile, (char *filename, int a2), 0x488F60);
VoidFunc(CountPerfectRings, 0x4890E0);
VoidFunc(LoadSuperSonic, 0x49A950);
VoidFunc(LoadSuperShadow, 0x49ABB0);
ObjectFunc(SuperSonic_Main, 0x49AE10);
ObjectFunc(LastBossPlayerManager_Main, 0x49E520);
VoidFunc(LoadLastBossPlayerManager, 0x49ECE0);
VoidFunc(LoadAquaticMineCharAnims, 0x4D4C30);
VoidFunc(LoadEggGolemECharAnims, 0x4A6DC0);
VoidFunc(LoadCannonsCoreRCharAnims, 0x4DDA70);
VoidFunc(LoadDryLagoon2PPoolQuestCharAnims, 0x4E63D0);
VoidFunc(LoadSandOcean2PCharAnims, 0x51D020);
VoidFunc(LoadDryLagoonCharAnims, 0x647830);
VoidFunc(LoadSandOceanCharAnims, 0x658A10);
VoidFunc(LoadCannonsCoreKCharAnims, 0x65EF70);
VoidFunc(LoadHiddenBaseCharAnims, 0x711C30);
VoidFunc(LoadEmeraldManager, 0x73AA70);

static const void *InitPlayerPtr = (void*)0x43DA40;
static void InitPlayer(int playerNum)
{
	__asm
	{
		mov eax, [playerNum]
		call InitPlayerPtr
	}
}

static const void *IsByteswappedPtr = (void*)0x429840;
static char IsByteswapped(void *a1)
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

static const void *LoadStartPositionPtr = (void*)0x43D8E0;
static signed int LoadStartPosition(int playerNum, NJS_VECTOR *position, int *rotation)
{
	signed int result;
	__asm
	{
		mov ecx, [playerNum]
		mov edi, [position]
		push [rotation]
		call LoadStartPositionPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void *Load2PIntroPosPtr = (void*)0x43DBD0;
static void Load2PIntroPos(int playerNum)
{
	__asm
	{
		mov eax, [playerNum]
		call Load2PIntroPosPtr
	}
}

static const void *LoadEndPositionPtr = (void*)0x43DD50;
static signed int LoadEndPosition(int playerNum)
{
	int result;
	__asm
	{
		mov eax, [playerNum]
		call LoadEndPositionPtr
		mov result, eax
	}
	return result;
}

static const void *PlayMusicPtr = (void*)0x442CF0;
static void PlayMusic(const char *song)
{
	__asm
	{
		mov edi, [song]
		call PlayMusicPtr
	}
}

static const void *PlaySoundPtr = (void*)0x443130;
static signed int PlayVoice(int a1, int a2)
{
	int result;
	__asm
	{
		mov edx, [a1]
		push [a2]
		call PlaySoundPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void *AddRingsPtr = (void*)0x44CE10;
static void AddRings(char playerNum, int numRings)
{
	__asm
	{
		mov al, [playerNum]
		mov edx, [numRings]
		call AddRingsPtr
	}
}

static const void *LoadMDLFilePtr = (void*)0x459590;
static ModelIndex *LoadMDLFile(char *filename)
{
	ModelIndex *result;
	__asm
	{
		mov eax, [filename]
		call LoadMDLFilePtr
		mov result, eax
	}
	return result;
}

static const void *LoadMTNFilePtr = (void*)0x459740;
static AnimationIndex *LoadMTNFile(char *filename)
{
	AnimationIndex *result;
	__asm
	{
		mov eax, [filename]
		call LoadMTNFilePtr
		mov result, eax
	}
	return result;
}

static const void *LoadDeathZonesPtr = (void*)0x46B090;
static signed int LoadDeathZones(DeathZone *a1)
{
	int result;
	__asm
	{
		mov ebx, [a1]
		call LoadDeathZonesPtr
		mov result, eax
	}
	return result;
}

static const void *KillCharacterPtr = (void*)0x46B110;
static void KillCharacter(int playerNum)
{
	__asm
	{
		mov ebx, [playerNum]
		call KillCharacterPtr
	}
}

static const void *LoadObject2Ptr = (void*)0x46F610;
static ObjectMaster *LoadObject2(int list, char *name, ObjectFuncPtr mainSub, char flags)
{
	ObjectMaster *result;
	__asm
	{
		xor eax, eax
		mov al, [flags]
		push eax
		mov ecx, [list]
		mov eax, [name]
		mov edi, [mainSub]
		call LoadObject2Ptr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void *LoadObjectPtr = (void*)0x46F680;
static ObjectMaster * LoadObject(ObjectFuncPtr mainSub, int list, char *name)
{
	ObjectMaster *result;
	__asm
	{
		mov edi, [mainSub]
		mov esi, [list]
		push [name]
		call LoadObjectPtr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void * LoadSetFilePtr = (void*)0x488DD0;
static void *LoadSETFile(int a1, char *filename_s, char *filename_u)
{
	void *result;
	__asm
	{
		mov eax, [a1]
		mov ecx, [filename_s]
		push [filename_u]
		call LoadSetFilePtr
		add esp, 4
		mov result, eax
	}
	return result;
}

static const void * LoadTailsPtr = (void*)0x74CF00;
static void LoadTails(int playerNum)
{
	__asm
	{
		mov eax, [playerNum]
		call LoadTailsPtr
	}
}

static const void * LoadEggmanPtr = (void*)0x73C220;
static void LoadEggman(int playerNum)
{
	__asm
	{
		mov eax, [playerNum]
		call LoadEggmanPtr
	}
}
#endif