#ifndef SA2MODLOADER_H
#define SA2MODLOADER_H

#include <WTypes.h>
#include <cstdint>
#include "ninja.h"

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

static BOOL WriteData(void *writeaddress, const void *data, SIZE_T datasize, SIZE_T *byteswritten)
{
	return WriteProcessMemory(GetCurrentProcess(), writeaddress, data, datasize, byteswritten);
}

static BOOL WriteData(void *writeaddress, const void *data, SIZE_T datasize)
{
	return WriteData(writeaddress, data, datasize, nullptr);
}

template<typename T> static BOOL WriteData(T const *writeaddress, const T data, SIZE_T *byteswritten)
{
	return WriteData((void*)writeaddress, (void*)&data, (SIZE_T)sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(T const *writeaddress, const T data)
{
	return WriteData(writeaddress, data, nullptr);
}

template<typename T> static BOOL WriteData(T *writeaddress, const T &data, SIZE_T *byteswritten)
{
	return WriteData(writeaddress, &data, sizeof(data), byteswritten);
}

template<typename T> static BOOL WriteData(T *writeaddress, const T &data)
{
	return WriteData(writeaddress, data, nullptr);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, const T(&data)[N], SIZE_T *byteswritten)
{
	return WriteData(writeaddress, data, SizeOfArray(data), byteswritten);
}

template <typename T, size_t N> static BOOL WriteData(void *writeaddress, const T(&data)[N])
{
	return WriteData(writeaddress, data, nullptr);
}

static BOOL WriteData(void *address, const char data, int count, SIZE_T *byteswritten)
{
	char *buf = new char[count];
	memset(buf, data, count);
	int result = WriteData(address, buf, count, byteswritten);
	delete[] buf;
	return result;
}

static BOOL WriteData(void *address, char data, int count)
{
	return WriteData(address, data, count, nullptr);
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
	__int16 Next;
	__int16 Current;
	__int16 dCurrent;
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
	float MechHP;
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

typedef CharObj2 CharObj2Base;

// Sonic's CharObj2
// Shared with: Shadow, Amy, MetalSonic
struct SonicCharObj2 : CharObj2 {
	char field_1B8[432];
	short SpindashTimer;
	char filler[42];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Knuckles's CharObj2
// Shared with: Rouge
struct KnucklesCharObj2 : CharObj2 {
	char field_1B8[568];
	TexList *TextureList;
	TexList *EffectTextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_400[32];
};

// Mechless Eggman's CharObj2
// Not Shared
struct EggmanCharObj2 : CharObj2 {
	char field_1B8[424];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Mech Eggman's CharObj2
// Shared with: Tails Mech
struct MechEggmanCharObj2 : CharObj2 {
	char field_1B8[652];
	TexList *CommonTextureList;
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Mechless Tails's CharObj2
// Not Shared
struct TailsCharObj2 : CharObj2 {
	char field_1B8[504];
	TexList *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_3BC[36];
};

// Super Sonic's CharObj2
// Shared with: Super Shadow
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

struct Loop
{
	__int16 Ang_X;
	__int16 Ang_Y;
	float Dist;
	NJS_VECTOR Position;
};

struct LoopHead
{
	__int16 Unknown_0;
	__int16 Count;
	float TotalDist;
	Loop *LoopList;
	ObjectFuncPtr Object;
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
DataPointer(ControllerData, Controller2Raw, &ControllersRaw[1]);
DataPointer(ControllerData, Controller3Raw, &ControllersRaw[2]);
DataPointer(ControllerData, Controller4Raw, &ControllersRaw[3]);
DataArray(CharObj2 *, MainCharObj2, 0x1DE9600, 2);
DataArray(ModelIndex, CharacterModels, 0x1DE9620, 532);
DataArray(CharObj1 *, MainCharObj1, 0x1DEA6C0, 2);
DataArray(ObjectMaster *, MainCharacter, 0x1DEA6E0, 2);
DataArray(AnimationIndex, CharacterAnimations, 0x1DEA700, 300);
DataArray(uint32_t, MenuPressedButtons, 0x1DEFB10, 4);
DataArray(ControllerData, Controllers, 0x1DEFC00, 4);
DataPointer(ControllerData, Controller1, &Controllers[0]);
DataPointer(ControllerData, Controller2, &Controllers[1]);
DataPointer(ControllerData, Controller3, &Controllers[2]);
DataPointer(ControllerData, Controller4, &Controllers[3]);

// SA2 Functions
#define FunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__cdecl *const NAME)ARGS = (RETURN_TYPE (__cdecl *)ARGS)ADDRESS
#define StdcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__stdcall *const NAME)ARGS = (RETURN_TYPE (__stdcall *)ARGS)ADDRESS
#define FastcallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__fastcall *const NAME)ARGS = (RETURN_TYPE (__fastcall *)ARGS)ADDRESS
#define ThiscallFunctionPointer(RETURN_TYPE, NAME, ARGS, ADDRESS) static RETURN_TYPE (__thiscall *const NAME)ARGS = (RETURN_TYPE (__thiscall *)ARGS)ADDRESS
#define VoidFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(void),ADDRESS)
#define ObjectFunc(NAME, ADDRESS) FunctionPointer(void,NAME,(ObjectMaster *obj),ADDRESS)

FunctionPointer(int, PrintDebug, (char *format, ...), 0x426740);
FunctionPointer(signed int, ProcessChunkModel, (NJS_CNK_MODEL *a1), 0x42D650);
VoidFunc(LoadCharacters, 0x43D630);
FastcallFunctionPointer(signed int, LoadTexturePack, (char *filename, TexList *texlist), 0x44C350);
ThiscallFunctionPointer(TexList *, LoadCharTextures, (char *filename), 0x44C510);
ObjectFunc(DeathZoneObject, 0x46AD50);
FunctionPointer(bool, CheckBreakObject, (ObjectMaster *obj, ObjectMaster *other), 0x46EC00);
ObjectFunc(DeleteObject_, 0x46F720);
FunctionPointer(CharObj1 *, AllocCharObj1, (void), 0x470B40);
FunctionPointer(ObjectMaster *, LoadChildObject, (int a3, void (__cdecl *a4)(ObjectMaster *), ObjectMaster *parent), 0x470C00);
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
		push [rotation]
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
		push [a2]
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
static inline ObjectMaster * LoadObject2(int list, char *name, void (__cdecl *mainSub)(ObjectMaster *), char flags)
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
static inline ObjectMaster * LoadObject(void (__cdecl *mainSub)(ObjectMaster *), int list, char *name)
{
	ObjectMaster * result;
	__asm
	{
		push [name]
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
		push [count]
		push [a2]
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
		push [name_u]
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
		push [a2]
		push [a1]
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
		push [a3]
		push [a2]
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

// ModInfo

static const int ModLoaderVer = 3;

#define arrayptrandlength(data) data, LengthOfArray(data)
#define arraylengthandptr(data) LengthOfArray(data), data
#define arrayptrandsize(data) data, SizeOfArray(data)
#define arraysizeandptr(data) SizeOfArray(data), data

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

#define patchdecl(address,data) { (void*)address, arrayptrandsize(data) }

struct PointerInfo
{
	void *address;
	void *data;
};

#define ptrdecl(address,data) { (void*)address, (void*)data }

struct HelperFunctions_v3
{
	// The version of the structure.
	int Version;
	// Registers a start position for a character.
	void (__cdecl *RegisterStartPosition)(unsigned char character, const StartPosition &position);
	// Clears the list of registered start positions for a character.
	void (__cdecl *ClearStartPositionList)(unsigned char character);
	// Registers a 2P intro position for a character.
	void (__cdecl *Register2PIntroPosition)(unsigned char character, const LevelEndPosition &position);
	// Clears the list of registered 2P intro positions for a character.
	void (__cdecl *Clear2PIntroPositionList)(unsigned char character);
};

typedef HelperFunctions_v3 HelperFunctions;

struct ModInfo
{
	int Version;
	void (__cdecl *Init)(const char *path, const HelperFunctions &helperFunctions);
	const PatchInfo *Patches;
	int PatchCount;
	const PointerInfo *Jumps;
	int JumpCount;
	const PointerInfo *Calls;
	int CallCount;
	const PointerInfo *Pointers;
	int PointerCount;
};
#endif