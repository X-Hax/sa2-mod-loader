#pragma once
#include "../ninja.h"

// SA2 Structs

typedef NJS_VECTOR Vector3;
struct ObjectMaster;

#pragma pack(push, 1)
struct Rotation
{
	int x, y, z;
};

struct CollisionData
{
	__int16 field_0;
	__int16 field_2;
	int field_4;
	char gap_8[12];
	float XScale;
	float YScale;
	float ZScale;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
};

struct CollisionInfo
{
	__int16 char0;
	__int16 field_2;
	WORD word4;
	WORD Count;
	float f8;
	CollisionData *CollisionArray;
	BYTE f10[140];
	ObjectMaster *Object;
	__int16 field_A0;
	__int16 field_A2;
	int field_A4;
};

struct UserConfigData
{
	BYTE f0[4];
	DWORD FullScreen;
	DWORD Width;
	DWORD Height;
	DWORD RefreshRate;
	DWORD Language;
	DWORD Display;
	DWORD Res;
	DWORD Analytics;
	DWORD dword24;
};

struct CharObj1
{
	char Action;
	char NextAction;
	char field_2;
	char field_3;
	unsigned short Status;
	char field_6;
	char field_7;
	Rotation Rotation;
	NJS_VECTOR Position;
	NJS_VECTOR Scale;
	CollisionInfo *field_2C;
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

struct CharObj2
{
	char PlayerNum;
	char CharID;
	char Costume;
	char CharID2;
	char ActionWindowItems[8];
	char ActionWindowItemCount;
	char field_D[3];
	__int16 Powerups;
	int field_12;
	__int16 UnderwaterTime;
	int field_18[3];
	int Upgrades;
	int field_28[8];
	float MechHP;
	int field_4C[6];
	NJS_VECTOR Speed;
	int field_70[19];
	NJS_MOTION **Animation;
	PhysicsData PhysData;
	int field_144[12];
	CharAnimInfo AnimInfo;
	int field_1A0[7];
};

typedef CharObj2 CharObj2Base;

// Sonic's CharObj2
// Shared with: Shadow, Amy, MetalSonic
struct SonicCharObj2 : CharObj2
{
	char field_1B8[432];
	short SpindashTimer;
	char filler[42];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Knuckles's CharObj2
// Shared with: Rouge
struct KnucklesCharObj2 : CharObj2
{
	char field_1B8[568];
	NJS_TEXLIST *TextureList;
	NJS_TEXLIST *EffectTextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_400[32];
};

// Mechless Eggman's CharObj2
// Not Shared
struct EggmanCharObj2 : CharObj2
{
	char field_1B8[424];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Mech Eggman's CharObj2
// Shared with: Tails Mech
struct MechEggmanCharObj2 : CharObj2
{
	char field_1B8[652];
	NJS_TEXLIST *CommonTextureList;
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Mechless Tails's CharObj2
// Not Shared
struct TailsCharObj2 : CharObj2
{
	char field_1B8[504];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_3BC[36];
};

// Super Sonic's CharObj2
// Shared with: Super Shadow
struct SuperSonicCharObj2 : CharObj2
{
	char field_1B8[440];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct SETEntry
{
	__int16 ID;
	__int16 XRot;
	__int16 YRot;
	__int16 ZRot;
	NJS_VECTOR Position;
	NJS_VECTOR Scale;
};

typedef void(__cdecl *ObjectFuncPtr)(ObjectMaster *);

struct SETObjectData
{
	char LoadCount;
	char field_1;
	__int16 Flags;
	ObjectMaster *Object;
	SETEntry *SETEntry;
	float field_C;
};

struct ObjectMaster
{
	ObjectMaster* PrevObject;
	ObjectMaster* NextObject;
	ObjectMaster* Parent;
	ObjectMaster* Child;
	ObjectFuncPtr MainSub;
	ObjectFuncPtr DisplaySub;
	ObjectFuncPtr DeleteSub;
	void* field_1C;
	void* field_20;
	void* field_24;
	void* field_28;
	void* field_2C;
	SETObjectData* field_30;
	CharObj1* Data1;
	void* field_38;
	void* field_3C;
	CharObj2Base* Data2;
	char* Name;
	int field_48;
	void* field_4C;
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
	NJS_VECTOR Center;
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
	NJS_TEXLIST *TextureList;
};

struct StartPosition
{
	__int16 Level;
	__int16 Rotation1P;
	__int16 RotationP1;
	__int16 RotationP2;
	NJS_VECTOR Position1P;
	NJS_VECTOR PositionP1;
	NJS_VECTOR PositionP2;
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
	void(__cdecl *Init)();
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
	void(__cdecl *Prolog)();
	void(__cdecl *Epilog)();
} ChaoSegmentData, RELFileInfo;

struct MemManFuncs
{
	void *(__cdecl *Allocate)(int size, char *file, int line);
	void(__cdecl *Deallocate)(void *data, char *file, int line);
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
	NJS_TEXLIST *NJS_TEXLIST;
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
	NJS_VECTOR Mission2Position;
	NJS_VECTOR Mission3Position;
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
