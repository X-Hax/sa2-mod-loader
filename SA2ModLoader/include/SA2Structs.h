#pragma once

#include <stdint.h>
#include "ninja.h"
#include "SA2Enums.h"

// SA2 Structs

typedef NJS_VECTOR Vector3;

struct EntityData1;
struct ObjectListEntry;
struct COL;
struct ModelIndex;
struct SETEntry;
struct SETObjectData;
struct CollisionInfo;
struct struct_0;
struct AnimationIndex;
struct AnimationInfo;
struct CollisionData;
struct CharObj2Base;
struct EntityData2;
struct LaunchConfig_vtable;
struct ChaoDataBase;
struct ChaoUnknownE;
struct ChaoDebugData1;
struct UnknownData2;
struct ChaoData1;
struct ObjUnknownA;
struct ObjUnknownB;
struct ObjectMaster;

typedef void(__cdecl *ObjectFuncPtr)(ObjectMaster *);

// All structs should be packed.
#pragma pack(push, 1)

struct AllocatedMem
{
	int Cookie;
	void *Data;
};

union __declspec(align(2)) Data1Ptr
{
	void *Undefined;
	EntityData1 *Entity;
	ChaoData1 *Chao;
	ChaoDebugData1 *ChaoDebug;
};

union Data2Ptr
{
	void *Undefined;
	ObjUnknownB *UnknownB;
	EntityData2 *Entity;
	CharObj2Base *Character;
	UnknownData2 *Unknown_Chao;
};

struct ObjectMaster
{
	ObjectMaster *PrevObject;
	ObjectMaster *NextObject;
	ObjectMaster *Parent;
	ObjectMaster *Child;
	ObjectFuncPtr MainSub;
	ObjectFuncPtr DisplaySub;
	ObjectFuncPtr DeleteSub;
	ObjectFuncPtr field_1C;
	ObjectFuncPtr field_20;
	ObjectFuncPtr SomethingSub;
	ObjectFuncPtr field_28;
	void *field_2C;
	SETObjectData *SETData;
	Data1Ptr Data1;
	UnknownData2 *EntityData2;
	ObjUnknownA *UnknownA_ptr;
	Data2Ptr Data2;
	char *Name;
	char *NameAgain;
	void *field_4C;
};

struct SETObjectData
{
	uint8_t LoadCount;
	char field_1;
	__int16 Flags;
	ObjectMaster *Object;
	SETEntry *SETEntry;
	float field_C;
};

struct Rotation
{
	int x;
	int y;
	int z;
};

struct EntityData1
{
	char Action;
	char NextAction;
	char field_2;
	char Index;
	__int16 Status;
	__int16 field_6;
	Rotation Rotation;
	NJS_VECTOR Position;
	NJS_VECTOR Scale;
	CollisionInfo *Collision;
};


struct MotionTableData
{
	__int16 flag;
	__int16 gap2;
	int AnimID;
	int TransitionToID;
	float frameIncreaseSpeed_;
	float someSpeedThing;
	float dword14;
	__int16 SomeFlagThingInEntry2;
	__int16 field_1A;
	float frame;
	float StartFrame2;
	float EndFrame2;
	float PlaySpeed2;
	NJS_MOTION *LastNJS_Motion;
	__int16 SomeFlagThingInEntry;
	__int16 field_32;
	float StartFrame;
	float StartFrame_;
	float EndFrame;
	float PlaySpeed;
	NJS_MOTION *NJS_MOTION;
	void *PointerToAnimations;
};

struct ChaoUnknownD
{
	__int16 field_0;
	__int16 field_2;
	__int16 field_4;
	__int16 field_6;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
};

struct ChaoSomeUnknownA
{
	__int16 index;
	__int16 index2;
	__int16 ID;
	__int16 someFlag;
	int saveData;
	int field_C;
	NJS_VECTOR locationVector;
	int field_1C;
	int field_20;
	int field_24;
	float field_28;
	float field_2C;
	float playerDistance;
	__int16 setSomeIndex;
	__int16 field_36;
	__int16 field_38;
	__int16 field_3A;
	ObjectMaster *pointerToOwner;
	ChaoSomeUnknownA *heldBy;
	ChaoSomeUnknownA *field_44;
};

struct ChaoGlobalObjectDefinition
{
	__int16 field_0;
	__int16 field_2;
	__int16 field_4;
	__int16 field_6;
	float Distance;
	int field_C;
	int field_10;
	int field_14;
	ChaoSomeUnknownA *UnknownA;
};

struct ChaoObjectList
{
	__int16 Count;
	__int16 field_2;
	int IsSet;
	int field_8;
	int field_C;
	float Distance;
	__int16 Count2;
	__int16 field_16;
	ChaoGlobalObjectDefinition field_18[32];
};

struct ChaoCurrentActionInfo
{
	__int16 field_0;
	unsigned __int16 Index;
	__int16 field_4;
	__int16 field_6;
	int field_8;
	int Timer;
	int field_10;
	int field_14;
	int BehaviourTime;
};

struct ChaoBehaviourInfothing
{
	int field_0[8];
};

struct ChaoBehaviourInfo
{
	ChaoCurrentActionInfo CurrentActionInfo;
	__int16 CanThink_;
	__int16 field_1E;
	int field_20;
	int field_24;
	ChaoBehaviourInfothing field_28;
	ChaoBehaviourInfothing field_48;
	char field_68[8];
	char field_70[72];
	int field_B8;
	int field_BC;
	int field_C0;
	char field_288[420];
	int field_268;
	int field_430;
	float someFloat;
	NJS_VECTOR someKindaPosition;
	int LastBehaviour;
	int behaviourNumber;
	int currentBehaviourIndex;
	int BehaviourBase[15];
	int field_2C8;
	int BehaviourTimer[15];
};

struct ChaoToyChunk
{
	NJS_OBJECT *model;
	NJS_TEXLIST *texlist;
	float scale;
	int exists;
};

struct ChaoFacialData
{
	int eyeTimer;
	__int16 field_4;
	__int16 field_6;
	__int16 Eye;
	__int16 field_A;
	int mouthTimer;
	__int16 field_10;
	__int16 Mouth;
	NJS_VECTOR somekindaposition;
	float field_20;
	int field_24;
	NJS_CNK_MODEL *Eye1;
	NJS_CNK_MODEL *Eye2;
	int field_30;
	int blinkState;
	int blinkTimer;
	int dword3C;
	unsigned int unsigned40;
	int gap44;
	int field_48;
	int VertRotAlt;
	int VertRot;
	int field_54;
	int HorizRotAlt;
	int HorizRot;
};

struct EmotionBallData
{
	__int16 ballID;
	__int16 ballID2;
	int notsure;
	int timer;
	int sizeSine;
	int yPosSine;
	int color;
	__int16 torchTimer;
	__int16 timer1;
	int randomZRot;
	NJS_VECTOR HeadPosUnitTransPortion;
	NJS_VECTOR parentPosition;
	__int16 gap38;
	unsigned __int16 field_3A;
	__int16 field_3C;
	__int16 field_3E;
	float field_40;
	float sizeTimer;
	int gap48;
	NJS_VECTOR position;
	NJS_VECTOR velocity;
	NJS_VECTOR someSize;
	__int16 field_70;
	__int16 field_72;
	__int16 field_74;
	__int16 field_76;
	__int16 field_78;
	__int16 field_7A;
	__int16 field_7C;
	__int16 field_7E;
	__int16 field_80;
	__int16 field_82;
	int field_84;
	float float88;
	int field_8C;
	NJS_VECTOR float90;
	NJS_VECTOR float9C;
	NJS_VECTOR field_A8;
};

struct ChaoObjectListInfo
{
	float MinimumDistance;
	int a2;
	int MinimumRotation;
	float a4;
	float a5;
};

struct ChaoEvos
{
	NJS_OBJECT *child[40];
	NJS_OBJECT *normal[40];
	NJS_OBJECT *swim[40];
	NJS_OBJECT *fly[40];
	NJS_OBJECT *run[40];
	NJS_OBJECT *power[40];
};

struct ChaoData1
{
	EntityData1 entity;
	int gap_30;
	int field_34;
	ObjectMaster *ObjectMaster_ptr1;
	int field_38;
	ObjectMaster *ObjectMaster_ptr2;
	int field_44;
	float field_54;
	int field_4C;
	int field_50;
	int field_60;
	int field_58;
	ChaoDataBase *ChaoDataBase_ptr;
	char field_70[40];
	int field_88;
	int field_8C;
	char field_90[16];
	int Flags;
	__int16 field_B4;
	__int16 field_A6;
	float waypointID;
	MotionTableData MotionTable;
	MotionTableData BodyTypeNone_MotionTable;
	char gap144[112];
	ChaoBehaviourInfo ChaoBehaviourInfo;
	int field_4BC[21];
	int PointerToStructWithCnkObject;
	float ChaoNodes[40];
	ChaoEvos *NormalModels;
	ChaoEvos *HeroModels;
	ChaoEvos *DarkModels;
	NJS_VECTOR BaseTranslationPos;
	NJS_VECTOR HeadTranslationPos;
	NJS_VECTOR LeftHandTranslationPos;
	NJS_VECTOR RightHandTranslationPos;
	NJS_VECTOR LeftLegTranslationPos;
	NJS_VECTOR RightLegTranslationPos;
	NJS_VECTOR NoseTranslationPos;
	NJS_VECTOR NoseUnitTransPortion;
	NJS_VECTOR LeftEyeTranslationPos;
	NJS_VECTOR LeftEyeUnitTransPortion;
	NJS_VECTOR RightEyeTranslationPos;
	NJS_VECTOR RightEyeUnitTransPortion;
	ChaoToyChunk LeftHandToyChunk;
	ChaoToyChunk RightHandToyChunk;
	int field_670;
	int field_674;
	int field_678;
	int field_67C;
	int field_680;
	int field_684;
	int field_688;
	ChaoFacialData ChaoFacialData;
	EmotionBallData EmotionBallData;
	NJS_VECTOR field_7A4;
	float waypointLerpFactor;
	NJS_VECTOR field_7B4;
	NJS_VECTOR field_7C0;
	ChaoObjectListInfo ObjectListInfo;
	ChaoObjectList PlayerObjects;
	ChaoObjectList ChaoObjects;
	ChaoObjectList FruitObjects;
	ChaoObjectList TreeObjects;
	ChaoObjectList ToyObjects;
	char ObjectListEnd;
	char field_19D9[927];
};

struct ChaoDebugData1
{
	char CurrentMenu;
	char MenuSelection;
	char field_2[18];
	__int16 ChaoID;
	char field_16[10];
};

struct UnknownData2_B
{
	int field_0;
	char gap_4[12];
	float field_10;
	NJS_VECTOR field_14;
};

struct UnknownData2
{
	NJS_VECTOR vector_0;
	char gap_C[4];
	float field_10;
	char gap_14[8];
	int field_1C;
	char gap_20[8];
	int field_28;
	int field_2C;
	float field_30;
	int field_34;
	char gap_38[4];
	int field_3C;
	__int16 field_40;
	__int16 Index;
	char gap_44[4];
	float field_48;
	char gap_4C[20];
	NJS_VECTOR some_vector;
	char gap_6C[4];
	float field_70;
	char gap_74[28];
	float field_90;
	float field_94;
	float field_98;
	float field_9C;
	float field_A0;
	float field_A4;
	float field_A8;
	float field_AC;
	char gap_B0[4];
	float field_B4;
	float field_B8;
	float field_BC;
	float field_C0;
	float field_C4;
	float field_C8;
	float field_CC;
	float field_D0;
	char gap_D4[4];
	float field_D8;
	float field_DC;
	NJS_VECTOR field_E0;
	UnknownData2_B WeirdStructures[12];
};

struct ObjUnknownA
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	float field_24;
	float field_28;
	float field_2C;
	int field_30;
	int field_34;
};

struct ObjUnknownB
{
	int Time;
	int Index;
	int Mode;
	int field_C;
};

struct EntityData2
{
	CharObj2Base *CharacterData;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	float field_34;
	int field_38;
	float field_3C;
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
	float MinSpeedForSomethingButAlsoKnockbackSpeed;
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

struct CharAnimInfo
{
	__int16 field_0;
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
	float field_18;
	char field_1C;
	char field_1D;
	char field_1E;
	char field_1F;
	char field_20;
	char field_21;
	char field_22;
	char field_23;
	AnimationInfo *Animations;
	NJS_MOTION *motion;
};

struct CharObj2Base
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
	__int16 IdleTime;
	char gap_1A[10];
	int Upgrades;
	float field_28;
	char field_2C[28];
	float MechHP;
	int field_4C[6];
	NJS_VECTOR Speed;
	char gap_70[36];
	ObjectMaster *HeldObject;
	char gap_98[4];
	ObjectMaster *HoldTarget;
	char gap_A0[28];
	NJS_MOTION **Animation;
	PhysicsData PhysData;
	int field_144[12];
	CharAnimInfo AnimInfo;
	int field_1A0[7];
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

struct CollisionInfo
{
	__int16 char0;
	__int16 field_2;
	uint16_t word4;
	uint16_t Count;
	float field_8;
	CollisionData *CollisionArray;
	uint8_t f10[140];
	ObjectMaster *Object;
	__int16 field_A0;
	__int16 field_A2;
	int field_A4;
};

struct ChaoCharacterBond
{
	char a;
	char b;
	int unknown;
};

struct ChaoDNA
{
	char ResetTrigger;
	char field_1[91];
	char SwimStatGrade1;
	char SwimStatGrade2;
	char FlyStatGrade1;
	char FlyStatGrade2;
	char RunStatGrade1;
	char RunStatGrade2;
	char PowerStatGrade1;
	char PowerStatGrade2;
	char StaminaStatGrade1;
	char StaminaStatGrade2;
	char LuckStatGrade1;
	char LuckStatGrade2;
	char IntelligenceStatGrade1;
	char IntelligenceStatGrade2;
	char UnknownStatGrade1;
	char UnknownStatGrade2;
	char field_4A4[34];
	char FavoriteFruit1;
	char FavoriteFruit2;
	char field_4C8[4];
	char Color1;
	char Color2;
	char MonotoneFlag1;
	char MonotoneFlag2;
	char Texture1;
	char Texture2;
	char ShinyFlag1;
	char ShinyFlag2;
	char EggColor1;
	char EggColor2;
	char gap_4D6[6];
};

struct __declspec(align(1)) ChaoDataBase
{
	char gap_0[18];
	char Name[7];
	char gap_19[7];
	char SwimFraction;
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char LuckyFraction;
	char IntelligenceFraction;
	char UnknownFraction;
	char SwimGrade;
	char FlyGrade;
	char RunGrade;
	char PowerGrade;
	char StaminaGrade;
	char LuckyGrade;
	char IntelligenceGrade;
	char UnknownGrade;
	char SwimLevel;
	char FlyLevel;
	char RunLevel;
	char PowerLevel;
	char StaminaLevel;
	char LuckLevel;
	char IntelligenceLevel;
	char UnknownLevel;
	__int16 SwimStat;
	__int16 FlyStat;
	__int16 RunStat;
	__int16 PowerStat;
	__int16 StaminaStat;
	__int16 LuckStat;
	char IntelligenceStat;
	char UnknownStat;
	char field_46[58];
	ChaoType Type;
	char Garden;
	__int16 Happiness;
	__int16 field_84;
	__int16 ClockRollovers;
	__int16 field_88;
	__int16 Lifespan;
	__int16 Lifespan2;
	__int16 Reincarnations;
	char field_90[24];
	float PowerRun;
	float FlySwim;
	float Alignment;
	char gap_B4[12];
	float EvolutionProgress;
	char gap_C4[13];
	char EyeType;
	char MouthType;
	char BallType;
	char gap_D4[1];
	char Headgear;
	char HideFeet;
	char Medal;
	char Color;
	char MonotoneHighlights;
	char Texture;
	char Shiny;
	char EggColor;
	SADXBodyType BodyType;
	char BodyTypeAnimal;
	char field_DF[57];
	int SA2AnimalBehavior;
	SA2BAnimal SA2BArmType;
	SA2BAnimal SA2BEarType;
	SA2BAnimal SA2BForeheadType;
	SA2BAnimal SA2BHornType;
	SA2BAnimal SA2BLegType;
	SA2BAnimal SA2BTailType;
	SA2BAnimal SA2BWingType;
	SA2BAnimal SA2BFaceType;
	char field_124[8];
	char Joy;
	char field_12D;
	char UrgeToCry;
	char Fear;
	char field_130;
	char Dizziness;
	char field_132[2];
	__int16 Sleepiness;
	__int16 Tiredness;
	__int16 Hunger;
	__int16 MateDesire;
	__int16 Boredom;
	char field_13E[10];
	__int16 Energy;
	char Normal_Curiosity;
	char field_14B;
	char CryBaby_Energetic;
	char Naive_Normal;
	char field_14E[2];
	char Normal_BigEater;
	char field_151[4];
	char Normal_Carefree;
	char field_156;
	char FavoriteFruit;
	char field_158[2];
	char CoughLevel;
	char ColdLevel;
	char RashLevel;
	char RunnyNoseLevel;
	char HiccupsLevel;
	char StomachAcheLevel;
	char field_160[4];
	__int16 SA2BToys;
	char field_166[6];
	ChaoCharacterBond SA2BCharacterBonds[6];
	char field_190[680];
	ChaoDNA DNA;
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

struct CollisionData
{
	__int16 field_0;
	__int16 field_2;
	int field_4;
	NJS_VECTOR some_vector;
	float anonymous_1;
	float anonymous_2;
	float anonymous_3;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
};

struct SonicCharObj2
{
	CharObj2Base base;
	char field_1BC[428];
	__int16 SpindashCounter;
	char field_36A[42];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct ModelIndex
{
	int Index;
	NJS_OBJECT *Model;
};

struct AnimationIndex
{
	uint16_t Index;
	uint16_t Count;
	NJS_MOTION *Animation;
};

struct ObjectListHead
{
	int Count;
	ObjectListEntry *List;
};

struct ObjectListEntry
{
	LoadObj LoadFlags;
	char List;
	__int16 ObjectFlags;
	float DistanceMaybe;
	ObjectFuncPtr Function;
	char *Name;
};

struct LevelItemData
{
	int Character;
	void *Name;
	int Mask;
	int Index;
	int field_10;
	void *anonymous_1;
	void *anonymous_2;
	void *anonymous_3;
	int field_20;
};

struct Time
{
	char Minutes;
	char Seconds;
	char Centiseconds;
};

struct SaveLevelScore
{
	__int16 Rings;
	__int16 field_2;
	int Score;
	Time Time;
	char field_B;
};

struct SaveLevelInfo
{
	char Ranks[6];
	__int16 PlayCounts[5];
	SaveLevelScore Scores[15];
};

struct SaveKartTime
{
	Time Time;
	char Character;
};

struct SaveKartInfo
{
	SaveKartTime Times[3];
	char Emblem;
};

struct SaveBossInfo
{
	Time Time1;
	char field_3[9];
	Time Time2;
	char field_F[9];
	Time Time3;
	char field_1B[145];
	char Emblem;
};

struct SaveData
{
	int Checksum;
	int dword_1DEC604;
	char anonymous_0;
	char anonymous_1;
	char anonymous_2;
	char anonymous_3;
	char anonymous_4;
	char anonymous_5;
	char EmblemCount;
	char anonymous_6;
	char LastCharacter;
	char LastLevel;
	char anonymous_9;
	char anonymous_10;
	char gap_14[8];
	__int16 anonymous_11;
	__int16 anonymous_12;
	__int16 anonymous_13;
	__int16 anonymous_14;
	int dword_1DEC624;
	int dword_1DEC628;
	int PlayTime;
	int TotalRings;
	int dword_1DEC634;
	SaveLevelInfo Levels[62];
	SaveKartInfo KartRace[3];
	char anonymous_80;
	char anonymous_81;
	char gap_2fd9[23];
	SaveBossInfo HeroBossAttack;
	char gap_309d[23];
	SaveBossInfo DarkBossAttack;
	char gap_3161[23];
	SaveBossInfo AllBossAttack;
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

struct COL
{
	NJS_VECTOR Center;
	float Radius;
	NJS_OBJECT *Model;
	int field_14;
	int field_18;
	int Flags;
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
	ObjectFuncPtr subprgmanager;
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

struct ChaoSegmentData
{
	char *Name;
	void(__cdecl *Prolog)();
	void(__cdecl *Epilog)();
};

struct RELFileInfo
{
	char *Name;
	void(__cdecl *Prolog)();
	void(__cdecl *Epilog)();
};

struct MemManFuncs
{
	AllocatedMem *(__cdecl *Allocate)(int size, char *file, int line);
	void(__cdecl *Deallocate)(AllocatedMem *, char *, int);
	void *AllocateArray;
	void *Deallocate2;
};

struct ChaoData
{
	ChaoDataBase data;
	char field_4DC[68];
	char field_520[736];
};

struct struct_a1
{
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	uint8_t byte4;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
	uint8_t byte9;
	uint8_t byteA;
	uint32_t dwordB;
	uint32_t dwordF;
	uint8_t f13[1];
	uint16_t word14;
	uint8_t f16[14];
	char char24;
	char gap_25[14];
	char field_33;
};

struct TexPackInfo
{
	const char *TexName;
	NJS_TEXLIST *TexList;
};

struct LevelRankScores
{
	__int16 Level;
	__int16 DRank;
	__int16 CRank;
	__int16 BRank;
	__int16 ARank;
};

struct struct_this
{
	uint8_t f0[2];
	uint8_t Rank;
	char Perfect;
	char gap_4[4];
	uint32_t TimeBonus;
	uint32_t Score;
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

struct BlackMarketItem
{
	ChaoItemCategory Category;
	Sint8 Type;
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

struct AnalogThing
{
	Angle direction;
	float magnitude;
};

struct LoopPoint
{
	__int16 XRot;
	__int16 YRot;
	float Distance;
	NJS_VECTOR Position;
};

struct LoopHead
{
	__int16 anonymous_0;
	__int16 Count;
	float TotalDistance;
	LoopPoint *Points;
	ObjectFuncPtr Object;
};

struct EmeManThing
{
	char byte0;
	char byte1;
	char byte2;
	char byte3;
	NJS_VECTOR v;
};

struct EmeManObj2
{
	uint8_t Action;
	uint8_t byte1;
	uint8_t byte2;
	uint8_t HintCount;
	uint8_t Status;
	uint8_t byte5;
	uint8_t byte6;
	uint8_t byte7;
	uint8_t byte8;
	uint8_t byte9;
	uint8_t byteA;
	uint8_t byteB;
	uint8_t byteC;
	uint8_t byteD;
	uint8_t fE[2];
	NJS_VECTOR dword10;
	uint8_t byte1C;
	uint8_t byte1D;
	uint8_t f1E[2];
	uint32_t dword20;
	uint32_t dword24;
	uint32_t dword28;
	EmeManThing byte2C[3];
	EmeManThing *ptr_a;
	EmeManThing *ptr_b;
	EmeManThing *ptr_c;
	EmeManThing *ptr_d;
	uint32_t dword6C;
	NJS_TEXLIST *TexList;
};

struct KnucklesCharObj2
{
	CharObj2Base base;
	char field_1BC[564];
	NJS_TEXLIST *TextureList;
	NJS_TEXLIST *EffectTextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_400[32];
};

struct EggmanCharObj2
{
	CharObj2Base base;
	char field_1BC[420];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct MechEggmanCharObj2
{
	CharObj2Base base;
	char field_1BC[128];
	float field_23C;
	char field_240[4];
	float field_244;
	int field_248;
	char field_24C[8];
	float field_254;
	float field_258;
	float field_25C;
	char field_260[120];
	float field_2D8;
	float field_2DC;
	float field_2E0;
	float field_2E4;
	float field_2E8;
	float field_2EC;
	float field_2F0;
	float field_2F4;
	float field_2F8;
	char field_2FC[96];
	char field_35C;
	char field_35D;
	char field_35E;
	char field_35F;
	char field_360;
	char field_361[3];
	__int16 field_364;
	__int16 field_366;
	__int16 field_368;
	__int16 field_36A;
	__int16 field_36C;
	__int16 field_36E;
	char field_370[4];
	float field_374;
	char field_378[8];
	int field_380;
	char field_384[12];
	float field_390;
	float field_394;
	float field_398;
	int field_39C;
	char field_3A0[132];
	int field_424;
	int field_428;
	int field_42C;
	int field_430;
	int field_434;
	float field_438;
	float field_43C;
	float field_440;
	NJS_TEXLIST *CommonTextureList;
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct TailsCharObj2
{
	CharObj2Base base;
	char field_1BC[500];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	char field_3BC[36];
};

struct SuperSonicCharObj2
{
	CharObj2Base base;
	char field_1BC[436];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct EnemyField38
{
	float f0;
	char field_4[24];
	int field_1C;
	char field_20[36];
	uint8_t byte44;
	uint8_t byte45;
	char field_46;
	char field_47;
	__int16 field_48;
	__int16 field_4A;
	uint16_t word4C;
	uint8_t f4E[6];
	float float54;
	float dword58;
	float dword5C;
	float dword60;
	char f64[52];
	float field_98;
	char field_9C[36];
	float floatC0;
	uint8_t fC4[4];
	float floatC8;
	float floatCC;
	float floatD0;
	float floatD4;
	float floatD8;
	float floatDC;
	float floatE0;
	float floatE4;
	uint32_t dwordE8;
	float fEC;
	float field_F0;
	char field_F4[208];
	int field_1C4;
	uint32_t dword1C8;
	uint8_t f1CC[8];
	uint32_t dword1D4;
	float float1D8;
	float float1DC;
	int field_1E0;
	char field_1E4[15];
	char field_1F3;
	char field_1F4[4];
	int field_1F8;
	int field_1FC;
	char field_200[16];
};

struct EventFileHeader
{
	void *field_0;
	NJS_TEXLIST *field_4;
	int field_8;
	void *field_C;
	void *field_10;
	void *field_14;
	void *field_18;
	NJS_OBJECT **field_1C;
	void *field_20;
	void *field_24;
	int field_28;
};

struct ControlShadowCharObj2
{
	uint8_t byte0;
	uint8_t byte1;
	uint16_t word2;
	uint16_t word4;
	uint8_t f6[2];
};

struct UserConfigData
{
	int Fullscreen;
	int Width;
	int Height;
	int RefreshRate;
	int Language;
	int Display;
	int Res;
	int Analytics;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int Rumble;
};

struct LaunchConfig
{
	LaunchConfig_vtable *vtable;
	UserConfigData data;
	char gap_38[451];
	char field_1FB;
};

struct LaunchConfig_vtable
{
	int sub_4025E0;
};

struct struct_v1
{
	uint32_t dword0;
	uint8_t f4[4];
	uint32_t dword8;
	uint8_t byteC;
	uint8_t fD[35];
	uint32_t dword30;
	uint32_t dword34;
	uint32_t dword38;
};

struct PDS_PERIPHERALINFO
{
	Uint32 type;
	Uint32 reserved[3];
	Uint8 is_root;
	Uint8 area_code;
	Uint8 connector_dir[2];
	char product_name[32];
	char license[64];
	Uint16 stdby_pow;
	Uint16 max_pow;
};

struct PDS_PERIPHERAL
{
	Uint32 id;
	Uint32 support;
	Uint32 on;
	Uint32 off;
	Uint32 press;
	Uint32 release;
	Uint16 r;
	Uint16 l;
	Sint16 x1;
	Sint16 y1;
	Sint16 x2;
	Sint16 y2;
	char *name;
	void *extend;
	Uint32 old;
	PDS_PERIPHERALINFO *info;
};

struct ButtonMapping
{
	int XInput;
	int Dreamcast;
};

struct XInputMapping
{
	int dword_8AE160;
	int dword_8AE164;
};

struct ItemBoxItem
{
	int Texture;
	void(__cdecl *Code)(ObjectMaster *, int);
};

struct s_effect2_2
{
	int field_0;
	int field_4;
	int field_8;
	char gap_C[6143];
	char field_180B;
};

struct s_effect2
{
	s_effect2_2 field_0[8];
};

struct PDS_BASE
{
	int off;
	int press;
	int release;
	__int16 R;
	__int16 L;
	__int16 analog_l_x;
	__int16 analog_l_y;
	__int16 analog_r_x;
	__int16 analog_r_y;
	int name;
	int extend;
	int old;
	PDS_PERIPHERALINFO *info;
	int field_28;
	int field_2C;
	int field_30;
};

struct FogData
{
	int flags;
	int color;
	float far_;
	float near_;
	float fogtable[128];
};

struct SaveFileStruct
{
	char field_0[842];
};

struct struc_33
{
	int field_0;
	int field_4;
	int field_8;
	char gap_C[328];
	float field_154;
	float field_158;
	float field_15C;
	int field_160;
	char gap_164[9072];
	int field_24D4;
};

struct struc_36
{
	char gap_0[20];
	int field_14;
	int field_18;
	int field_1C;
	float field_20;
	float field_24;
	char gap_28[832];
	int field_368;
	char gap_36C[636];
	int field_5E8;
	int field_5EC;
	int field_5F0;
	int field_5F4;
	int field_5F8;
	int field_5FC;
	int field_600;
	int field_604;
	char gap_608[24];
	int field_620;
	int field_624;
	int field_628;
	int field_62C;
	int field_630;
	int field_634;
	int field_638;
	int field_63C;
	char gap_640[4];
	int field_644;
	int field_648;
	int field_64C;
	int field_650;
	int field_654;
	int field_658;
	int field_65C;
	int field_660;
	int field_664;
	int field_668;
	int field_66C;
	int field_670;
	int field_674;
	int field_678;
	char gap_67C[27];
	char field_697;
};

struct SOCController__vtbl
{
	int sub_425440;
	int sub_4262A0;
	int sub_425F40;
	int sub_426010;
	int sub_4260F0;
	int sub_426150;
	int sub_4261D0;
	int sub_426230;
	int sub_426320;
};

struct SOCController
{
	SOCController__vtbl *vtable;
	char gap_4[4];
	int field_8;
	int field_12;
	char field_16;
	char gap_11[7];
	__int64 field_24;
	char gap_20[8];
	int field_40;
	int field_2C;
};

struct SOCControllerAll__vtbl
{
	int sub_425440;
	int nullsub_1;
	int sub_426340;
	int sub_4263B0;
	int sub_425410;
	int sub_425420;
	int sub_708E50;
	int sub_425430;
	int anonymous_0;
};

struct SOCControllerAll
{
	SOCControllerAll__vtbl *vtable;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
};

struct DebugFunction
{
	void(__cdecl *Function)(void *_this);
	int Enabled;
	const char *Name;
};

struct ChaoUnknownC
{
	char gap_0[2];
	__int16 field_2;
	char gap_4[8];
	int field_C;
	int field_10;
	char gap_14[16];
	char field_24[72];
	char gap_6C[516];
	float field_270;
	NJS_VECTOR vector;
	char gap_280[384];
	char field_400;
};

struct MotionTableEntry
{
	__int16 field_0;
	__int16 field_2;
	int field_4;
	int field_8;
	float field_C;
	float field_10;
	float field_14;
	__int16 field_18;
	__int16 field_1A;
	float field_1C;
	float field_20;
	float field_24;
	float field_28;
	int field_2C;
	int field_30;
	float field_34;
};

struct ALO_ChaosDriveExecutor_Data1
{
	EntityData1 entity;
	char field_30;
	char gap_31[3];
	int field_34;
	char gap_38[4];
	int field_3C;
	int field_40;
	char gap_44[4];
	int field_48;
	int field_4C;
	int field_50;
};

struct AL_MinimalExecutor_Data1
{
	EntityData1 entity;
	char gap_30[4];
	int field_34;
	char gap_38[76];
	int field_84;
	char gap_88[24];
	int field_A0;
	char gap_A4[24];
	int field_BC;
	char gap_C0[24];
	int field_D8;
	char gap_DC[27];
	char field_F7;
};

struct ALO_EntranceElevatorExecutor_Data2
{
	int field_0;
	int field_4;
	NJS_VECTOR Position;
	int field_14;
	int field_18;
	int field_1C;
	NJS_VECTOR field_20;
	int field_2C;
	int field_30;
	int field_34;
};

struct CharSelectThing
{
	char Costume;
	char CostumeUnlocked;
	char gap_2;
	char Visible;
	char gap_4[16];
};

struct LastBoss2Data2
{
	char field_0;
	char field_1;
	char field_2;
	char field_3;
	char field_4;
	char field_5;
	char field_6;
	char field_7;
	__int16 field_8;
	char field_A;
	char field_B;
	__int16 field_C;
	char field_E;
	char field_F;
};

struct MenuData
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
};

struct DispWinnerAndContinue_Data
{
	DispAction Action;
	char Player;
	char Selection;
	char field_3;
	int Timer;
	int field_8;
	int field_C;
	int field_10;
};

struct al_confirmload_data2
{
	uint32_t state;
	uint32_t dword4;
	uint32_t dword8;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	char *data_ptr;
	uint32_t dword1C;
	uint32_t dword20;
};

struct AL_GBAManagerExecutor_Data
{
	int field_0;
	int field_4;
	int field_8;
	ObjectMaster *ObjectPtr;
	char field_10[4228];
	char gap_1094[12691];
	char field_4227;
};

struct ChaoEggData
{
	uint32_t dword0;
	char gap_4[4];
	uint32_t dword8;
	uint32_t dwordC;
	uint8_t gap10[4];
	uint32_t dword14;
	float float18;
	float float1C;
	float field_20;
	uint32_t dword24;
	int field_28;
};

struct ProbablyShaderRelated
{
	int field_0;
	int field_4;
	int HorizontalResolution;
	int field_C;
	int field_10;
	int field_14;
	int RefreshRate;
	int Display;
	int field_20;
	int field_24;
	int field_28;
};

struct __declspec(align(8)) struct_a2
{
	void *vtbl;
	int field_4;
	int field_8;
	int field_C;
	uint32_t dword10;
	uint32_t dword14;
	uint8_t byte18;
	char gap_19[3];
	uint32_t DisplayHeight;
	uint32_t RefreshRate;
	uint32_t dword24;
	uint32_t dword28;
	uint32_t dword2C;
	uint8_t byte30;
	char gap_31[3];
	uint32_t dword34;
	uint32_t dword38;
	uint8_t gap3C[32];
	uint32_t dword5C;
	uint8_t gap60[788];
	uint8_t byte374;
	char gap_375[3];
	uint32_t dword378;
	uint32_t dword37C;
};

struct StoryEntry
{
	int8_t Type;
	int8_t Character;
	int16_t Level;
	int16_t Events[4];
};

#pragma pack(pop)
