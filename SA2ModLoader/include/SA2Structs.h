#pragma once

#include <stdint.h>
#include "ninja.h"
#include "SA2Enums.h"

typedef Uint32 NJD_SPRITE;
typedef Uint32 NJD_FLAG;
typedef Uint32 NJD_CONTROL_3D;
typedef DWORD _DWORD;
typedef Uint32 NJD_COLOR_TARGET;
typedef Uint32 NJD_COLOR_BLENDING;
typedef Uint32 NJD_TEX_SHADING;
typedef Uint32 NJD_DRAW;
typedef Uint32 NJD_TEXATTR;

// SA2 Structs

struct EntityData1;
struct taskwk;
struct ObjectListEntry;
struct COL;
struct ModelIndex;
struct SETEntry;
struct _OBJ_EDITENTRY;
struct SETObjectData;
struct OBJ_CONDITION;
struct CollisionInfo;
struct colliwk;
struct struct_0;
struct AnimationIndex;
struct AnimationInfo;
struct CollisionData;
struct CCL_INFO;
struct CharObj2Base;
struct playerwk;
struct EntityData2;
struct motionwk;
struct LaunchConfig_vtable;
struct ChaoDataBase;
struct ChaoUnknownE;
struct ChaoDebugData1;
struct UnknownData2;
struct ChaoData1;
struct ObjUnknownA;
struct ObjUnknownB;
struct ObjectMaster;
struct task;
struct LoopHead;
struct CameraInfo;
struct CameraParam;

using ObjectFuncPtr = void(__cdecl*)(ObjectMaster*);
using TaskFuncPtr = void(__cdecl*)(task*);
using CameraFuncPtr = void(__cdecl*)(CameraInfo*, CameraParam*);
using ModelFuncPtr = void(__cdecl*)(NJS_OBJECT*);

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

union __declspec(align(2)) taskwkPtr
{
	void* Undefined;
	taskwk* twp;
	ChaoData1* Chao;
	ChaoDebugData1* ChaoDebug;
};

union Data2Ptr
{
	void *Undefined;
	ObjUnknownB *UnknownB;
	EntityData2 *Entity;
	CharObj2Base *Character;
	UnknownData2 *Unknown_Chao;
};

union motionwkPtr
{
	void* Undefined;
	ObjUnknownB* UnknownB;
	motionwk* Entity;
	playerwk* Character;
	UnknownData2* Unknown_Chao;
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
	ObjectFuncPtr DisplaySub_Delayed1;
	ObjectFuncPtr DisplaySub_Delayed2;
	ObjectFuncPtr DisplaySub_Delayed3;
	ObjectFuncPtr DisplaySub_Delayed4;
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

struct task
{
	task* last;
	task* next;
	task* ptp;
	task* ctp;
	TaskFuncPtr exec;
	TaskFuncPtr disp;
	TaskFuncPtr dest;
	TaskFuncPtr dispDelayed1;
	TaskFuncPtr dispDelayed2;
	TaskFuncPtr dispDelayed3;
	TaskFuncPtr dispDelayed4;
	void* field_2C;
	OBJ_CONDITION* ocp;
	taskwkPtr Data1;
	UnknownData2* EntityData2;
	ObjUnknownA* UnknownA_ptr;
	motionwkPtr anywk;
	char* Name;
	char* NameAgain;
	void* thp;
};

struct SETObjectData
{
	uint8_t LoadCount;
	char field_1;
	__int16 Flags;
	ObjectMaster *Object;
	SETEntry *SETEntry;
	float Distance;
};

struct OBJ_CONDITION
{
	char scCount;
	char scUserFlag;
	__int16 ssCondition;
	task* pTask;
	_OBJ_EDITENTRY* pObjEditEntry;
	float fRangeOut;
};

struct Rotation
{
	Angle x, y, z;
};

using Angle3 = Rotation;

struct EntityData1
{
	char Action;
	char NextAction;
	char field_2;
	char Index;
	__int16 Status;
	__int16 Timer;
	Rotation Rotation;
	NJS_VECTOR Position;
	NJS_VECTOR Scale;
	CollisionInfo *Collision;
};

struct taskwk
{
	char mode;
	char smode;
	char id;
	char btimer;
	__int16 flag;
	unsigned __int16 wtimer;
	Angle3 ang;
	NJS_POINT3 pos;
	NJS_POINT3 scl;
	colliwk* cwp;
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

// Movement information (motionwk internally)
struct EntityData2
{
	NJS_POINT3 Velocity;
	NJS_POINT3 Acceleration;
	Rotation Forward;
	Rotation SpeedAngle;
	float Radius;
	float Height;
	float Weight;
};

struct motionwk
{
	NJS_POINT3 spd;
	NJS_POINT3 acc;
	Angle3 ang_aim;
	Angle3 ang_spd;
	float rad;
	float height;
	float weight;
};

struct PhysicsData
{
	int HangTime;
	float FloorGrip;
	float SpeedCapH;
	float SpeedCapV;
	float SpeedMaxH;
	float PushSpeedMax;
	float JumpSpeed;
	float NoControlSpeed;
	float SlideSpeed;
	float JogSpeed;
	float RunSpeed;
	float RushSpeed;
	float KnockbackSpeed;
	float DashSpeed;
	float JumpAdd;
	float RunAccel;
	float AirAccel;
	float RunDecel;
	float RunBrake;
	float AirBrake;
	float AirDecel;
	float AirResist;
	float AirResistV;
	float AirResistH;
	float GroundFriction;
	float GroundFrictionH;
	float FrictionCap;
	float RatBound;
	float Radius;
	float Height;
	float Weight;
	float CameraY;
	float CenterHeight;
};

// Physics struct from SADX symbols
struct player_parameter
{
	int jump2_timer;
	float pos_error;
	float lim_h_spd;
	float lim_v_spd;
	float max_x_spd;
	float max_psh_spd;
	float jmp_y_spd;
	float nocon_speed;
	float slide_speed;
	float jog_speed;
	float run_speed;
	float rush_speed;
	float crash_speed;
	float dash_speed;
	float jmp_addit;
	float run_accel;
	float air_accel;
	float slow_down;
	float run_break;
	float air_break;
	float air_resist_air;
	float air_resist;
	float air_resist_y;
	float air_resist_z;
	float grd_frict;
	float grd_frict_z;
	float lim_frict;
	float rat_bound;
	float rad;
	float height;
	float weight;
	float eyes_height;
	float center_height;
};

struct CharAnimInfo
{
	__int16 mtnmode;
	__int16 Next;
	__int16 Current;
	__int16 Animation3;
	__int16 field_8;
	__int16 acttimer;
	__int16 field_C;
	char field_E;
	char field_F;
	float nframe;
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
	NJS_MOTION *Motion;
};

struct mtnjvwk
{
	__int16 mtnmode;
	unsigned __int16 reqaction;
	unsigned __int16 field_19;
	unsigned __int16 action;
	unsigned __int16 lastaction;
	unsigned __int16 nextaction;
	unsigned __int16 acttimer;
	__int16 flag;
	float nframe;
	float start_frame;
	float* spdp;
	float* workp;
	char field_20;
	char field_21;
	char field_22;
	char field_23;
	AnimationInfo* plactptr;
	NJS_MOTION* actwkptr;
};

struct shadowwk
{
	int angx;
	int angz;
	unsigned int Attr_top;
	unsigned int Attr_bottom;
	float y_top;
	float y_bottom;
	unsigned int _Attr_top;
	unsigned int _Attr_bottom;
};

// Vertical surface information for shadows and ripples
struct CharSurfaceInfo
{
	Angle AngX;
	Angle AngZ;
	SurfaceFlags TopSurface;
	SurfaceFlags BottomSurface;
	Float TopSurfaceDist;
	Float BottomSurfaceDist;
	SurfaceFlags PrevTopSurface;
	SurfaceFlags PrevBottomSurface;
};

// Contains input (first 4 variables) and output information for the dynamic collision system.
struct csts
{
	float radius;
	NJS_POINT3 pos;
	NJS_POINT3 spd;
	NJS_POINT3 tnorm;
	unsigned __int16 find_count;
	unsigned __int16 selected_nmb;
	float yt;
	float yb;
	int angx;
	int angz;
	NJS_POINT3 normal;
	NJS_POINT3 normal2;
	NJS_POINT3 onpoly;
	NJS_POINT3 pshbk;
	NJS_POINT3 anaspdh;
	NJS_POINT3 anaspdv;
};

// Player-specific data, common base for all characters.
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
	__int16 field_12;
	__int16 DisableControlTimer;
	__int16 UnderwaterTime;
	__int16 IdleTime;
	__int16 ConfuseTime;
	char gap1C[8];
	int Upgrades;
	float field_28;
	Angle TiltAngle;
	float PathDist;
	float Up;
	char field_38[8];
	float SomeMultiplier;
	int field_44;
	float MechHP;
	NJS_POINT3 Eff;
	NJS_POINT3 Acceleration;
	NJS_VECTOR Speed;
	NJS_POINT3 WallNormal;
	NJS_POINT3 FloorNormal;
	SurfaceFlags CurrentSurfaceFlags;
	SurfaceFlags PreviousSurfaceFlags;
	csts* DynColInfo;
	ObjectMaster* HeldObject;
	char gap98[4];
	ObjectMaster* HoldTarget;
	ObjectMaster* CurrentDyncolTask;
	int field_A4;
	char gapA8[16];
	LoopHead* PathData;
	NJS_MOTION** Animation;
	PhysicsData PhysData;
	NJS_VECTOR SomeVectors[4];
	CharAnimInfo AnimInfo;
	CharSurfaceInfo SurfaceInfo;
};

struct playerwk
{
	char PlayerNum;
	char CharID;
	char Costume;
	char CharID2;
	char ActionWindowItems[8];
	char ActionWindowItemCount;
	char field_D[3];
	__int16 Powerups;
	__int16 field_12;
	__int16 DisableControlTimer;
	__int16 UnderwaterTime;
	__int16 IdleTime;
	__int16 ConfuseTime;
	char gap1C[8];
	int Upgrades;
	float field_28;
	Angle TiltAngle;
	float PathDist;
	float Up;
	char field_38[8];
	float SomeMultiplier;
	int field_44;
	float MechHP;
	NJS_POINT3 eff;
	NJS_POINT3 acc;
	NJS_VECTOR spd;
	NJS_POINT3 wall_normal;
	NJS_POINT3 floor_normal;
	SurfaceFlags CurrentSurfaceFlags;
	SurfaceFlags PreviousSurfaceFlags;
	csts* cstsp; //dyncol info
	task* htp;
	char gap98[4];
	task* HoldTarget;
	task* CurrentDyncolTask;
	int field_A4;
	char gapA8[16];
	LoopHead* PathData;
	NJS_MOTION** Animation;
	player_parameter p;
	NJS_VECTOR SomeVectors[4];
	mtnjvwk mj;
	shadowwk shadow;
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

struct _OBJ_EDITENTRY
{
	unsigned __int16 usID;
	__int16 rotx;
	__int16 roty;
	__int16 rotz;
	float xpos;
	float ypos;
	float zpos;
	float xscl;
	float yscl;
	float zscl;
};

struct CollisionHitInfo
{
	__int8 my_num;
	__int8 hit_num;
	unsigned __int16 flag;
	EntityData1* hit_entity;
};

struct c_colli_hit_info
{
	char my_num;
	char hit_num;
	unsigned __int16 flag;
	taskwk* hit_twp;
};

struct CollisionInfo
{
	unsigned __int16 Id;
	__int16 HitCount;
	unsigned __int16 Flag;
	unsigned __int16 Count;
	float Range;
	CollisionData* CollisionArray;
	CollisionHitInfo CollisionHits[16]; // the first 16 entities that collide with this
	NJS_POINT3 Normal;
	ObjectMaster* Object;
	__int16 my_num;
	__int16 hit_num;
	CollisionInfo* CollidingObject; // the first colliding object
};

struct colliwk
{
	unsigned __int16 id;
	__int16 nbHit;
	unsigned __int16 flag;
	unsigned __int16 nbInfo;
	float colli_range;
	CCL_INFO* info;
	c_colli_hit_info hit_info[16];
	NJS_POINT3 normal;
	task* mytask;
	__int16 my_num;
	__int16 hit_num;
	colliwk* hit_cwp;
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

struct ChaoEmotions
{
	__int16 field_124;
	__int16 Category1Timer;
	__int16 SicknessTimer;
	__int16 Category2Timer;
	char Joy;
	char Anger;
	char UrgeToCry;
	char Fear;
	char Surprise;
	char Dizziness;
	char Relax;
	char Total;
	__int16 Sleepiness;
	__int16 Tiredness;
	__int16 Hunger;
	__int16 MateDesire;
	__int16 Boredom;
	__int16 Lonely;
	__int16 Tire;
	__int16 Stress;
	__int16 Nourish;
	__int16 Conditn;
	__int16 Energy;
	char Normal_Curiosity;
	char Kindness;
	char CryBaby_Energetic;
	char Naive_Normal;
	char Solitude;
	char Vitality;
	char Glutton;
	char Regain;
	char Skillful;
	char Charm;
	char Chatty;
	char Normal_Carefree;
	char Fickle;
	char FavoriteFruit;
	char field_34;
	char field_35;
	char CoughLevel;
	char ColdLevel;
	char RashLevel;
	char RunnyNoseLevel;
	char HiccupsLevel;
	char StomachAcheLevel;
};

struct  ChaoDataBase
{
	char gap_0[18];
	char Name[7];
	char field_19;
	char GBATexture;
	char field_1B[5];
	char SwimFraction;
	char FlyFraction;
	char RunFraction;
	char PowerFraction;
	char StaminaFraction;
	char LuckyFraction;
	char IntelligenceFraction;
	char UnknownFraction;
	char StatGrades[8];
	char StatLevels[8];
	__int16 StatPoints[8];
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int someGBAthingstart;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
	int someGBAthingend;
	ChaoType Type;
	char Garden;
	__int16 Happiness;
	__int16 InKindergarten;
	__int16 ClockRollovers;
	__int16 AdultClockRollovers;
	__int16 Lifespan;
	__int16 Lifespan2;
	__int16 Reincarnations;
	int field_90;
	int field_94;
	int Seed;
	int field_9C;
	int field_A0;
	int TimescaleTimer;
	float PowerRun;
	float FlySwim;
	float Alignment;
	int field_B4;
	int field_B8;
	float LastLifeAlignment;
	float EvolutionProgress;
	char gap_C4[13];
	char EyeType;
	char MouthType;
	char BallType;
	char gap_D4[1];
	char Headgear;
	char HideFeet;
	char Medal;
	unsigned char Color;
	char MonotoneHighlights;
	char Texture;
	char Shiny;
	char EggColor;
	SADXBodyType BodyType;
	char BodyTypeAnimal;
	char field_DF[41];
	__int16 DoctorMedal;
	char field_10A[14];
	int SA2AnimalBehavior;
	SA2BAnimal SA2BArmType;
	SA2BAnimal SA2BEarType;
	SA2BAnimal SA2BForeheadType;
	SA2BAnimal SA2BHornType;
	SA2BAnimal SA2BLegType;
	SA2BAnimal SA2BTailType;
	SA2BAnimal SA2BWingType;
	SA2BAnimal SA2BFaceType;
	ChaoEmotions Emotion;
	char ClassRoomFlags[4];
	__int16 SA2BToys;
	__int16 field_166;
	__int16 field_168;
	__int16 field_16A;
	ChaoCharacterBond SA2BCharacterBonds[6];
	char field_190[680];
	ChaoDNA DNA;
};

struct ChaoData2
{
	char gap0[4];
	float float4;
	char gap8[212];
	float WaterHeight;
};

struct AnimationInfo
{
	__int16 AnimNum;
	__int16 ModelNum;
	__int16 mtnmode;
	__int16 NextAnimation;
	float TransitionSpeed;
	float AnimationSpeed;
};

struct CollisionData
{
	char kind; // an identifier for colliding entities
	char form;
	char push;
	char damage;
	unsigned int attr;
	NJS_VECTOR center;
	Float param1;
	Float param2;
	Float param3;
	Float param4;
	Rotation rotation;
};

struct CCL_INFO
{
	char kind;
	char form;
	char push;
	char damage;
	unsigned int attr;
	NJS_POINT3 center;
	float a;
	float b;
	float c;
	float d;
	int angx;
	int angy;
	int angz;
};

struct JiggleInfo
{
	char type;
	char field_1;
	__int16 field_2;
	float speed;
	int field_8;
	int field_C;
	int field_10;
	NJS_OBJECT* SourceModelPtr;
	NJS_OBJECT* SourceModelCopy;
	NJS_OBJECT* Model;
	NJS_OBJECT* OtherModel;
	NJS_VECTOR field_24;
	char gap30[4];
	float field_34[33];
	int field_B8;
	int field_BC;
	int field_C0;
	float field_C4;
	float field_C8;
	float field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	float field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	int field_108;
	int field_10C;
	int field_110;
	int field_114;
	int field_118;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	int field_12C;
	int field_130;
	int field_134;
	int field_138;
	int field_13C;
};

struct SonicCharObj2
{
	CharObj2Base base;
	char gap1BC[40];
	NJS_POINT3 righthand_pos;
	NJS_POINT3 lefthand_pos;
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 head0_pos;
	NJS_POINT3 head1_pos;
	NJS_POINT3 head2_pos;
	char gap224[48];
	NJS_POINT3 righthand_vec0;
	NJS_POINT3 lefthand_vec0;
	NJS_POINT3 righthand_vec1;
	NJS_POINT3 lefthand_vec1;
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 head0_vec;
	NJS_POINT3 head1_vec;
	NJS_POINT3 head2_vec;
	char gap290[133];
	char SomersaultNextAction;
	char gap35E[8];
	__int16 SomersaultTime;
	__int16 SpindashCounter;
	char field_36A[6];
	int FlameRingPulse;
	int BounceBraceletPulse;
	NJS_VECTOR SpineJigglePos;
	NJS_VECTOR LightDashPos;
	JiggleInfo* SpineJiggle;
	NJS_TEXLIST* TextureList;
	ModelIndex* ModelList;
	AnimationIndex* MotionList;
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
	char LoadFlags;
	char List;
	__int16 ObjectFlags;
	float Distance;
	ObjectFuncPtr Function;
	const char *Name;
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
	__int16 VisibleModelCount;
	__int16 field_4;
	__int16 AnimCount;
	__int16 Attrs;
	__int16 IsTextureLoaded;
	float ClippingDistance;
	COL* COLList;
	void* AnimData;
	const char* TextureName;
	NJS_TEXLIST* TextureList;
};

struct COL
{
	NJS_VECTOR Center;
	float Radius;
	NJS_OBJECT *Model;
	int field_14;
	int Chunks;
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
	void(__cdecl* Free)();
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

struct Mission4TimeLimits
{
	char Level;
	MinSec TimeLimit;
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

struct MLTSoundEntry
{
	__int8 Bank;
	__int8 ID;
	__int8 SecondaryBank;
	__int8 DefaultFlags;
	__int16 Unknown;
	__int16 DefaultDistance;
};

struct MLTSoundList
{
	int Size;
	MLTSoundEntry* List;
};

struct CharacterSoundBank
{
	int Character;
	const char *Name;
	MLTSoundList* SoundList;
};

struct CharacterVoiceBank
{
	char Mode;
	char Character;
	short Padding;
	const char* Filename_JP;
	const char* Filename_EN;
	MLTSoundList* SoundList;
};

struct DeathZone
{
	uint32_t Flags;
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
	char field_1BC[40];
	NJS_POINT3 righthand_vec;
	NJS_POINT3 lefthand_vec;
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 head_vec0;
	NJS_POINT3 head_vec1;
	NJS_POINT3 bodycenter_vec;
	char field_23C[48];
	NJS_POINT3 righthand_pos0;
	NJS_POINT3 lefthand_pos0;
	NJS_POINT3 righthand_pos1;
	NJS_POINT3 lefthand_pos1;
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 head0_pos;
	NJS_POINT3 head1_pos;
	NJS_POINT3 bodycenter_pos;
	char field_2D8[214];
	__int16 WingAction;
	int WingAnimID;
	int WingAnimID2;
	float WingFloat;
	char field_3BC[4];
	NJS_VECTOR SomeVector;
	NJS_VECTOR TorsoJigglePos;
	NJS_VECTOR HeadJigglePos;
	JiggleInfo* HeadJiggle;
	JiggleInfo* LowerTorsoJiggle;
	JiggleInfo* UpperTorsoJiggle;
	NJS_TEXLIST *TextureList;
	NJS_TEXLIST *EffectTextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
	NJS_MOTION *WingMotion;
	char field_404[12];
	NJS_MOTION *WingShapeMotion;
	char field_414[12];
};

struct EggmanCharObj2
{
	CharObj2Base base;
	char field_1BC[40];
	NJS_POINT3 righthand_vec;
	NJS_POINT3 lefthand_vec;
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 head_vec0;
	NJS_POINT3 head_vec1;
	NJS_POINT3 bodycenter_vec;
	char field_23C[48];
	NJS_POINT3 righthand_pos0;
	NJS_POINT3 lefthand_pos0;
	NJS_POINT3 righthand_pos1;
	NJS_POINT3 lefthand_pos1;
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 head0_pos;
	NJS_POINT3 head1_pos;
	NJS_POINT3 bodycenter_pos;
	char field_2D8[136];
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

struct MechEggmanCharObj2
{
	CharObj2Base base;
	char field_1BC[40];
	NJS_POINT3 meleeweapon_vec;
	char field_1F4[12];
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 mechtorsotop0_vec;
	char field_224[12];
	NJS_POINT3 mechtorsobottom_vec;
	NJS_POINT3 blasterbarrel_vec;
	NJS_POINT3 shot_vec;
	NJS_POINT3 lockonlaser_vec;
	NJS_POINT3 mechtorsotop1_vec;
	NJS_POINT3 meleeweapon_pos;
	char field_278[36];
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 mechtorsotop0_pos;
	char field_2C0[12];
	NJS_POINT3 mechtorsobottom_pos;
	NJS_POINT3 blasterbarrel_pos;
	NJS_POINT3 shot_pos;
	NJS_POINT3 lockonlaser_pos;
	NJS_POINT3 mechtorsotop1_pos;
	char field_308[84];
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
	float rockettimer;
	float field_378;
	char field_37C[4];
	int field_380;
	char field_384[12];
	float field_390;
	float field_394;
	float field_398;
	int field_39C;
	NJS_POINT3 rockets_pos;
	char field_3AC[60];
	NJS_POINT3 rockets_vec;
	NJS_POINT3 leftcalf_vec;
	NJS_POINT3 rightcalf_vec;
	NJS_POINT3 leftcalf_pos;
	NJS_POINT3 rightcalf_pos;
	Angle BlasterAimAngle;
	Angle MechTorsoTopAimAngle;
	int LaserBlasterPulse;
	int CannonUpgradeRecoil;
	int BlasterBarrelRecoil;
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
	char field_1BC[40];
	NJS_POINT3 righthand_pos;
	NJS_POINT3 lefthand_pos;
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 head0_pos;
	NJS_POINT3 head1_pos;
	NJS_POINT3 head2_pos;
	NJS_POINT3 tailnode_pos;
	char field_2BC[36];
	NJS_POINT3 righthand_vec0;
	NJS_POINT3 lefthand_vec0;
	NJS_POINT3 righthand_vec1;
	NJS_POINT3 lefthand_vec1;
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 head0_vec;
	NJS_POINT3 head1_vec;
	NJS_POINT3 head2_vec;
	NJS_POINT3 tailnode_vec;
	char field_3BC[204];
	NJS_TEXLIST* TextureList;
	ModelIndex* ModelList;
	AnimationIndex* MotionList;
	char field_4BC[36];
};

struct SuperSonicCharObj2
{
	CharObj2Base base;
	char field_1BC[40];
	NJS_POINT3 righthand_vec;
	NJS_POINT3 lefthand_vec;
	NJS_POINT3 rightfoot_vec;
	NJS_POINT3 leftfoot_vec;
	NJS_POINT3 head_vec0;
	NJS_POINT3 head_vec1;
	NJS_POINT3 bodycenter_vec;
	char field_23C[48];
	NJS_POINT3 righthand_pos0;
	NJS_POINT3 lefthand_pos0;
	NJS_POINT3 righthand_pos1;
	NJS_POINT3 lefthand_pos1;
	NJS_POINT3 rightfoot_pos;
	NJS_POINT3 leftfoot_pos;
	NJS_POINT3 head0_pos;
	NJS_POINT3 head1_pos;
	NJS_POINT3 bodycenter_pos;
	char field_2D8[136];
	NJS_VECTOR SpineJigglePos;
	JiggleInfo *SpineJiggle;
	NJS_TEXLIST *TextureList;
	ModelIndex *ModelList;
	AnimationIndex *MotionList;
};

// Raycast hit information
struct xssunit
{
	Sint32 findflag;
	Sint32 objatt;
	Angle angx;
	Angle angz;
	Float onpos;
	NJS_POINT3 normal;
};

// Raycast header for vertical search
struct zxsdwstr
{
	NJS_POINT3 pos;
	xssunit lower;
	xssunit upper;
};

// Raycast header for 6 directions (GetShadowPosXYZ, GetShadowPosXYZonWater)
struct xyyzzxsdwstr
{
	NJS_POINT3 pos;
	xssunit hit[6];
};

// Animation set information for enemies
struct EnemyActionTable
{
	NJS_ACTION* actptr;
	Sint16 mtnmode;
	Sint16 next;
	Float frame;
	Float racio;
};

// Struct for the enemy animation system, used in EnemySetMotion
struct EnemyMotionData
{
	Float nframe;
	int unk02;
	Float start_frame;
	int unk04;
	Sint8 mtnmode;
	Sint8 flag;
	Sint8 action;                // Current animation
	Sint8 reqaction;
	Sint8 lastaction;
	Sint8 nextaction;
	Sint16 unk16;
	EnemyActionTable* plactptr;  // Animation table
	NJS_OBJECT* objp;            // Model to use for drawing
	NJS_ACTION* actwkptr;        // Use this when mtnmode is 2, otherwise use plactptr[action].actptr to retrieve the action
	NJS_ACTION* prev_actptr;
	Float prev_nframe;
	Float prev_start_frame;
	int unk30;
};

// Main enemy data, allocated with EnemyInitialize
struct EnemyData
{
	NJS_POINT3 velo; // Movement vector, can be altered by enemy collision subs like EnemyCheckWall
	NJS_POINT3 acc;
	int unk18;
	int unk1C;
	int unk20;
	int unk24;
	int unk28;
	int unk2C;
	int unk30;
	int unk34;
	int unk38;
	int unk3C;
	int unk40;
	Sint8 pnum; // To store player ID when needed
	Sint8 itemid; // Switch or emerald ID
	Sint8 unk46[2];
	__int16 unk48[2];
	Sint16 flag; // see E_FLAG enum
	Sint16 unk4E;
	int unk50;
	Float unk54;
	NJS_POINT3 home; // Start position, automatically set by EnemyInitialize
	NJS_POINT3 aim; // Custom target position
	NJS_POINT3 unk70;
	NJS_POINT3 pre; // Previsious position
	NJS_POINT3 force;
	NJS_POINT3 unk94;
	NJS_POINT3 norm; // Floor normal
	int unkAC;
	int unkBO;
	int unkB4;
	int unkB8;
	NJS_POINT3 colli_center; // Center offset
	Float colli_top; // Height
	Float colli_radius; // Radius
	Float colli_bottom; // Y offset
	Float cliff_height;
	Float bound_side; // Wall "bounce"
	Float bound_floor; // Floor "bounce"
	Float bound_friction; // Ground friction
	Float bound_ceiling; // Ceiling "bounce"
	Angle bound_add_angle; // Speed to turn at when enemy hits a wall
	Float shadow_scl; // For DC shadows
	Float shadow_scl_ratio; // For DC shadows
	xyyzzxsdwstr shadow; // Contains raycasting information
	Float buyoscale; // Distortion effect scalor
	Angle angx_spd; // X angle speed
	Angle angy_spd; // Y angle speed (used by EnemyTurnTo functions)
	Angle angz_spd; // Z angle speed
	Angle aim_angle; // Destination angle (used by EnemyTurnTo functions)
	Angle view_angle; // Field of view of enemy (used by EnemySearchPlayer)
	Float view_range; // View distance (used by EnemySearchPlayer)
	Float hear_range; // View distance unaffected by field of view (used by EnemySearchPlayer)
	EnemyMotionData motiondata; // For EnemySetMotion, pobj and plactptr necessary
};

struct TextureAnimData
{
	int Type;
	int SpeedControls;
	int SpeedDivider;
	float* field_C;
	short* UVData;
	short* field_14;
	short* field_18;
	NJS_CNK_MODEL* field_1C;
	short field_20;
	short field_22;
};

struct ModelTextureAnimationArray1
{
	NJS_OBJECT* Model;
	TextureAnimData* texanim;
	int field_8;
};

struct ModelTextureAnimationArray2
{
	int Type;
	NJS_OBJECT* Model;
	TextureAnimData* texanim;
	int field_C;
	int field_10;
};

struct ModelTextureAnimationArray3
{
	NJS_OBJECT* Model;
	TextureAnimData* texanim;
	int field_8;
	int field_C;
};

struct CutsceneTexAnimIDs
{
	int TexID;
	int	TexLoopNumber;
};

struct UVEditEntry
{
	void    *UVAddress;
	NJS_TEX	UV;
};

struct EventTexAnim
{
	int			 TexID;
	void		 *PolyMaterial;
	int			 UVEditCount;
	UVEditEntry	 *UVEdits;
};

struct CutsceneTexAnimMain
{
	NJS_OBJECT		*Model;
	int				AnimCount;
	EventTexAnim	*Data;
};

struct CutsceneTexAnim
{
	CutsceneTexAnimMain	*Data;
	CutsceneTexAnimIDs	*TexID;
	int					DataCount;
};

struct UpgradeListData
{
	NJS_OBJECT	*RootModel;
	NJS_OBJECT	*Upgrade1Attach;
	NJS_OBJECT  *Upgrade1Model;
	NJS_OBJECT	*Upgrade2Attach;
	NJS_OBJECT	*Upgrade2Model;
};

struct UpgradeList
{
	UpgradeListData Data[18];
};

struct UpgradeOverrideList
{
	NJS_OBJECT	*Upgrade1[31];
	NJS_OBJECT	*Upgrade2[31];
	NJS_OBJECT	*NormalModel[31];
};

struct BlurModels
{
	NJS_OBJECT	*models[64];
};

struct ReflectPlane
{
	NJS_POINT3 points[4];
};

struct ReflectionData
{
	int				DataSets;
	int				Transparency[32];
	ReflectPlane	*ReflectionPlane;
};

struct BigCameoData
{
	NJS_OBJECT	*Model;
	NJS_MOTION	**AnimationArray;
	int			AnimationCount;
	int			Unk;
};

struct CutsceneEntityData
{
	NJS_OBJECT		*Model;
	NJS_MOTION		*Animation;
	NJS_MOTION		*ShapeMotion;
	NJS_OBJECT		*GCModel;
	NJS_OBJECT		*ShadowModel;
	TextureAnimData *Texanim;
	NJS_POINT3		Position;
	int				Flags;
	int				DisplayLayer;
};

struct CutsceneAssetArray
{
	CutsceneEntityData  *entity;
	int					entityCount;
	NJS_MOTION			*cameraArray;
	int					cameraArrayCount;
	NJS_MOTION			*particleMotionArray;
	int					particleMotionCount;
	BigCameoData		*bigCameo;
	Uint32				frameCount;
};

struct EventFileHeader
{
	CutsceneAssetArray	*assets;
	NJS_TEXLIST			*texlist;
	int					sceneCount;
	void				*texDimensions;
	ReflectionData		*reflectionData;
	BlurModels			*blurModels;
	UpgradeOverrideList *upgradeOverrideList;
	NJS_OBJECT			**tailsTails;
	UpgradeList			*upgradeList;
	CutsceneTexAnim		*texanimData;
	int					shadowControl;
};
struct EventParticlePulse
{
	int scale;
	int speed;
};
struct EventParticleAnimControl
{
	EventParticlePulse* data;
	int count;
};

struct EventSubtitleData
{
	int FrameStart;
	int VisibleTime;
};

struct EventAudioData
{
	int FrameStart;
	char SFXInit;
	char CreditsControl;
	__int16 VoiceID;
	char MusicName[16];
	char JingleName[16];
	char field_28[32];
};

struct EventScreenEffectData
{
	int FrameStart;
	char Type;
	char field_5[3];
	NJS_COLOR Color;
	char Fade;
	char field_D;
	__int16 TexID;
	int VisibleTime;
	__int16 PosX;
	__int16 PosY;
	float Width;
	float Height;
	char field_20[32];
};

struct EventParticleData
{
	int FrameStart;
	char Type;
	char MotionID;
	__int16 field_6;
	float Setting1;
	float Setting2;
	float Setting3;
	float Setting4;
	char field_18[32];
};

struct EventLightingData
{
	int FrameStart;
	int Type;
	NJS_VECTOR Direction;
	NJS_VECTOR Color;
	float Intensity;
	NJS_VECTOR AmbientColor;
	char field_30[20];
};

struct EventBlurData
{
	int FrameStart;
	int Duration;
	char ModelIDs[6];
	__int16 field_E;
	int Instances;
	char field_14[44];
};

struct EventParticleGeneratorData
{
	NJS_VECTOR Position;
	NJS_VECTOR field_C;
	__int16 field_18[4];
	int FrameStart;
	NJS_VECTOR Spread;
	int Count;
	int field_34;
	int Type;
	int field_3C;
};

struct EventVideoData
{
	int FrameStart;
	__int16 PosX;
	__int16 PosY;
	float Depth;
	char OverlayType;
	char OverlayTexID;
	__int16 field_E;
	char VideoName[48];
};

struct EventEffectData
{
	EventSubtitleData subtitles[256];
	EventAudioData audio[512];
	EventScreenEffectData screeneffects[64];
	EventParticleData particles[2048];
	EventLightingData lighting[1024];
	EventBlurData blur[64];
	EventParticleGeneratorData particlegen[64];
	EventVideoData video[64];
};

struct MiniEventParts
{
	NJS_OBJECT* Part;
	NJS_MOTION* PartAnimation;
	NJS_MOTION* PartShapeMotion;
};

struct MiniEventAssets
{
	NJS_MOTION* BodyAnimation;
	MiniEventParts Data[4];
};

struct MiniEventFile
{
	int Flags;
	NJS_MOTION *Camera;
	MiniEventAssets* Assets[8];
};

struct MiniEventEffects
{
	int FrameStart;
	char FadeType;
	char SFXEntry1;
	char SFXEntry2;
	char field_7;
	__int16 VoiceID;
	char MusicData[16];
	char JingleData[16];
	__int16 field_2A;
	int RumbleControl;
	char field_30[28];
};

struct MiniEventEffectFile
{
	EventSubtitleData subtitles[32];
	MiniEventEffects effects[64];
	NJS_VECTOR field_1400;
	float field_140C;
	NJS_VECTOR field_1410;
};

struct TailsPlainFile
{
	NJS_TEXLIST* Texlist;
	NJS_OBJECT* Model;
	NJS_MOTION* Motion;
	NJS_MOTION* Camera;
};

struct CutsceneVoices
{
	Uint32 InternalID;
	int	   VoiceFileID;
};

struct SummaryData
{
	int StringID;
	int VoiceID;
	int JPNFrameStart;
	int ENGFrameStart;
	int FRNFrameStart;
	int SPAFrameStart;
	int GERFrameStart;
	int ITAFrameStart;
};

struct SummaryDataArray
{
	int StoryType;
	int StorySequenceID;
	int BackgroundID;
	SummaryData* Data;
	int DataCount;
};

struct MenuVoices
{
	int Default;
	int Shadow;
	int Rouge;
	int Eggman;
	int Maria;
	int Secretary;
	int Omochao;
	int Amy;
	int Tails;
	int Knuckles;
	int Sonic;
};

struct ActionWindowTextArray
{
	int Action;
	int TextID;
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

struct PDS_KEYBOARDINFO
{
	unsigned __int8 lang;
	unsigned __int8 type;
	unsigned __int8 led;
	unsigned __int8 led_ctrl;
};

struct PDS_KEYBOARD
{
	unsigned __int8 ctrl;
	unsigned __int8 led;
	unsigned __int8 key[6];
	PDS_KEYBOARDINFO* info;
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

struct LightData
{
	NJS_VECTOR direction;
	float intensity;
	float ambient;
	NJS_VECTOR color;
};

struct LightGCData
{
	NJS_VECTOR direction;
	NJS_VECTOR color;
	NJS_VECTOR ambient;
	int flags;
	int field_28;
	int field_2C;
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

struct RewardScreen
{
	int* stringIDs;
	int stringCount;
	int congratulationsCheck;
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

struct AL_GBAManagerExecutor_Thing
{
	int thing;
	int field_4;
	__int16 field_8;
	__int16 field_A;
	int mode;
	int field_10;
	int field_14;
	int hasItem[11];
	int hasItem___[11];
	int chaoDataPointer;
	int eggType;
	int rings;
	char fruitType[8];
	ChaoData chaoData1;
	ChaoData chaoData2;
};

struct AL_GBAManagerExecutor_Data
{
	int dword0;
	int gap4;
	int dword8;
	ObjectMaster* field_C;
	int insertedChao;
	ChaoData* ChaoDataPointer;
	AL_GBAManagerExecutor_Thing field_18[4];
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

struct BossTitleLetterData
{
	int GBIX;
	__int16 Width;
	__int16 Height;
	__int16 Delay;
	__int16 TexWidth;
	__int16 TexHeight;
	__int16 field_E;
};

struct BossTitleData
{
	BossTitleLetterData* Letters;
	__int16 LetterCount;
	__int16 field_6;
	int anonymous_2;
	int anonymous_3;
	int anonymous_4;
	int DisplayTime;
	NJS_TEXLIST* TexList;
	float X;
	float Y;
	float Scale;
	int anonymous_9;
};

struct CharaObjectData
{
	NJS_OBJECT* MainModel;
	NJS_MOTION* Animation1;
	NJS_MOTION* Animation2;
	NJS_MOTION* Animation3;
	NJS_OBJECT* AccessoryModel;
	NJS_OBJECT* AccessoryAttachNode;
	NJS_OBJECT* SuperModel;
	NJS_MOTION* SuperAnimation1;
	NJS_MOTION* SuperAnimation2;
	NJS_MOTION* SuperAnimation3;
	int anonymous_9;
	int Rating;
	int DescriptionID;
	int TextBackTexture;
	float anonymous_13;
};

struct VersusIntroData
{
	int IntroAnimID;
	NJS_MOTION* IntroCamera;
	float field_8;
	int NeutralIntroVoice;
	int WinningIntroVoice;
	int LosingIntroVoice;
};

struct IdleVoiceArray
{
	char Character;
	char Level;
	__int16 Voice1;
	__int16 Voice2;
};

struct KartSpecialInfo
{
	int ID;
	NJS_OBJECT* Model;
	NJS_OBJECT* LowModel;
	NJS_TEXLIST* TexList;
	int Unknown1;
	int Unknown2;
	int Unknown3;
};

struct KartMenu
{
	int CharacterID;
	int PortraitID;
	NJS_OBJECT* KartModel;
	char SPD;
	char ACL;
	char BRK;
	char GRP;
};

struct KartSFXAndShadows
{
	int EngineSFX;
	int BrakeSFX;
	int FinishVoice;
	int FirstVoice;
	int LastVoice;
	NJS_OBJECT* ShadowModel;
};

struct CartStreetLightPos
{
	NJS_VECTOR Position;
	int YRotation;
};

struct CartCourseModelInfo
{
	NJS_OBJECT* Model;
	NJS_OBJECT* Collision;
	NJS_VECTOR EndPoint;
	int YRotation;
	char field_18[76];
	CartStreetLightPos* StreetLights;
	int StreetLightCount;
};

struct KartCourse
{
	char* Course;
	int Length;
};

struct KartPhysics
{
	float Acceleration;
	float BrakePower;
	float Deceleration;
	float SpeedCap;
	float Weight;
	float Unk1;
	float DriftHandling;
	float DriftSpeedThreshold;
	float Unk2;
	float TopSpeed;
};

struct KartCPUStats
{
	float speed;
	float variance;
	float aggression;
};

struct KartDownloadData
{
	KartPhysics downloadPhysics;
	KartCPUStats CPUDownloadStats;
	KartCPUStats CPUNormalStats;
	int field_40;
	char MusicTrack;
};

struct ChaoMotionTableEntry
{
	NJS_MOTION* Motion;
	unsigned int Flag1;
	int TransitionID;
	unsigned int Flag2;
	float StartFrame;
	float EndFrame;
	float PlaySpeed;
};

struct _OBJ_CAMERAMODE
{
	const char* name;
	CameraFuncPtr fnCamera;
	int mode;
};

struct _OBJ_CAMERAADJUST
{
	const char* name;
	CameraFuncPtr fnAdjust;
};

// Information for motion cameras, see SetMotionCamera
struct CameraMotion
{
	float speed;
	Angle angy;
	NJS_MOTION* mtn;
	NJS_POINT3* pos;
	int flag;
	void* endcallback;
};

// Data for the camera collision system
struct CameraColliInfo
{
	NJS_POINT3 current_pos;
	NJS_POINT3 next_pos;
	int flag_mask;
	float float_1C;
	NJS_POINT3 output_pos;
	char gap2C[220];
	NJS_POINT3 dir;
	char gap114[36];
	int field_138;
};

// Drawing view information
struct CameraViewInfo
{
	NJS_POINT3 pos;
	NJS_POINT3 dir;
	Angle roll;
	Angle fov;
	float unknown;
};

// Camera location information
struct CameraLocation
{
	NJS_POINT3 pos;
	Angle3 ang;
	NJS_POINT3 dir;
	NJS_POINT3 tgt;
	NJS_POINT3 diff; // difference with previous position
	float spd;
};

// Free space for camera subtasks
struct CameraWork
{
	char gap[256];
};

// Camera subtask information
struct CameraParam
{
	int mode;
	int field_4;
	unsigned int ulTimer;
	CameraFuncPtr pObjCameraMode;
	int field_10;
	CameraWork work;
};

// Main camera task information
struct CameraInfo
{
	Angle fov_current;
	Angle fov_target;
	Angle fov_spd;
	BOOL analog_control;
	BOOL Player_stop_flag;
	int field_14;
	CameraColliInfo colliwk;
	NJS_POINT3 shake_offset;
	int shake_mode;
	int shake_timer;
	float shake_magnitude;
	CameraViewInfo view;
	int field_190;
	CameraLocation location;
	CameraLocation oldLocation;
	CameraLocation oldLocation_; // this one is set before it runs collisions
	int currentCameraSlot; // current slot for cameraModes & cameraAdjusts
	CameraParam cameraModes[16];
	CameraParam cameraAdjusts[16];
};

// Additional object data for path controllers.
struct PathControl
{
	Uint8 Action;
	Uint8 Player;
	Uint16 Unknown;
	Uint16 CounterA;
	Uint16 CounterB;
	Sint32 padding[3];
	NJS_VECTOR minimum;
	NJS_VECTOR maximum;
	NJS_VECTOR direction;
	LoopHead* loophead;
};

struct ef_message {
	uint8_t Hide;
	unsigned char field_1;
	unsigned short XPos;
	unsigned short YPos;
	unsigned char field_6;
	unsigned char field_7;
	float XScale;
	float YScale;
	float shadow_offset;
	unsigned short field_14;
	unsigned short field_16;
	unsigned short XScale_q;
	unsigned short YScale_q;
	unsigned short field_1C;
	unsigned short field_1E;
	short field_20;
	short field_22;
	short field_24;
	unsigned short BackgroundXPos;
	unsigned short BackgroundYPos;
	unsigned short BackgroundYScale;
	unsigned char field_2C;
	unsigned char field_2D;
	unsigned char field_2E;
	unsigned char field_2F;
	NJS_COLOR TextColor;
	NJS_COLOR ShadowColor;
	NJS_COLOR BackgroundColor;
	unsigned int* field_3C;
	unsigned char field_40;
	unsigned char field_41;
	unsigned char field_42;
	unsigned char field_43;
	NJS_TEXLIST TexList;
	NJS_TEXNAME TexName;
};

struct ef_message_b {
	ef_message Message;
	unsigned char field_58;
	unsigned char field_59;
	unsigned char field_5A;
	unsigned char field_5B;
	unsigned char field_5C;
	unsigned char field_5D;
	unsigned char field_5E;
	unsigned char field_5F;
	int MessageTimer;
	unsigned char field_64;
	unsigned char field_65;
	unsigned char field_66;
	unsigned char field_67;
	int AutoAdvanceText;
	char* OriginalString;
	char* RemainingString;
	unsigned char field_74;
	unsigned char field_75;
	unsigned char field_76;
	unsigned char field_77;
	unsigned char field_78;
	unsigned char field_79;
	unsigned char field_7A;
	unsigned char field_7B;
	unsigned char field_7C;
	unsigned char field_7D;
	unsigned char field_7E;
	unsigned char field_7F;
	int State_q;
	unsigned char field_84;
	unsigned char field_85;
	unsigned char field_86;
	unsigned char field_87;
};

struct DemoInput {
	Buttons ButtonsP1;
	Buttons ButtonsP2;
	Buttons ButtonsPressedP1;
	Buttons ButtonsPressedP2;
	short TransformedJoyXP1;
	short TransformedJoyXP2;
	short TransformedJoyYP1;
	short TransformedJoyYP2;
	uint8_t StickPushedP1;
	uint8_t StickPushedP2;
	unsigned char field_1A;
	unsigned char field_1B;
	int JoyAngleP1;
	float JoyMagnitudeP1;
	int JoyAngleP2;
	float JoyMagnitudeP2;
};

struct Number {
	unsigned int Mode;
	signed int MaxNumber;
	signed int Number;
	NJS_VECTOR Pos;
	float Scale;
	NJS_COLOR Color;
};

struct PolygonPoint {
	NJS_POINT3 pos;
	NJS_COLOR color;
};

struct RenderInfo
{
	char gap0[8];
	void* TextureData;
	Uint32 attrflags;             /* Ninja attributes from NJS_MATERIAL */
	int unknown2;
	int texparbuf;
	int unknown3;
	int unknown4;
	NJS_TEXLIST* CurrentTexlist;
	int unknown5;
	int CurrentTexid;
};

struct DynColInfo
{
	SurfaceFlags  Attribute;
	NJS_OBJECT*   Object;
	ObjectMaster* Task;
};

// Information output from GetPathStatus.
struct PathInfo
{
	int angx;
	int angz;
	int angax;
	int angaz;
	float onpathpos; // distance from start (input)
	NJS_POINT3 pos;
	float xpos;
	float ypos;
	float zpos;
	NJS_POINT3 normal;
	NJS_POINT3 normala;
	NJS_POINT3 front;
};

// Information output from CalcPathTbl.
struct pathtbl
{
	__int16 angx;
	__int16 angz;
	float length;
	float xpos;
	float ypos;
	float zpos;
	__int16 angx_;
	__int16 angz_;
};

// Linked list for dyncol polygon information.
struct CL_PolyInfo
{
	CL_PolyInfo* next;
	CL_PolyInfo* before;
	float cx;
	float cy;
	float cz;
	float ndlenmax2;
	NJS_POINT3 vrt[3];
	unsigned int polatflg;
};

// Linked list for dyncol tasks.
struct CL_ObjInfo
{
	CL_ObjInfo* next;
	CL_ObjInfo* before;
	NJS_OBJECT* objptr;
	int objatt;
	CL_PolyInfo* pri_start;
	CL_PolyInfo* pri_end;
	int pri_nmb;
	CL_PolyInfo** pni_start;
	CL_PolyInfo** pni_end;
	int pni_nmb;
	float zelen;
	float xelen;
	float cx;
	float cy;
	float cz;
	float dummy;
};

// Links a color to a bitfield of chunks.
struct ChunkMapColor
{
	unsigned int color;
	unsigned int chunks;
};

struct SoundEntry
{
	__int8 Unknown1;
	__int8 Unknown2;
	__int8 Bank;
	__int8 Panning;
	__int8 Volume3D;
	__int8 Volume;
	__int8 MaxVolume;
	__int8 Unknown3;
	SoundFlags Flags;
	__int16 DistanceShort;
	__int16 PlayTime;
	__int16 PlayTime2;
	ObjectMaster* Entity;
	int SoundID;
	NJS_VECTOR* PositionPtr;
	NJS_VECTOR Position;
	float Unknow4;
	float DistanceFloat;
};

struct SoundSystem
{
	int StartIndex;
	int OverflowIndex;
	int Unknown1;
	int Unknown2;
	SoundEntry SoundEntries[43];
	void* SomePointers[8];
	MLTSoundList MLTSoundLists[8];
};

struct HomingAttackTarget
{
	EntityData1* entity;
	float distance;
};

struct Light
{
	NJS_VECTOR direction;
	float intensity;
	float ambient;
	NJS_VECTOR color;
};

struct LightGC
{
	NJS_VECTOR direction;
	NJS_VECTOR lightColor;
	NJS_VECTOR ambientReg;
	int flags;
	int unused;
	int unk;
};

struct CHAO_ID
{
	int gid[2];
	int id[2];
	int num;
};

struct AL_BODY_INFO
{
	float HPos; // Run : Power Align
	float VPos; // Swim : Fly Align
	float APos; // Dark : Hero Align
	float AimHPos; // Last Run : Power
	float AimVPos; // Last Swim : Fly
	float AimAPos; // Last Alignment
	float growth; // Magitude
	float EyePos;
	float EyeScl;
	float EyeRatio;
	Sint8 EyeColor;
	Sint8 DefaultEyeNum;
	eAL_MOUTH_NUM DefaultMouthNum;
	eCHAO_ICON HonbuNum; // Icon type
	Sint8 HonbuColorNum; // Inert
	eCHAO_MASK ObakeHead; // Hat / Mask
	eBODY_PARTS ObakeBody; // Hide Feet
	eMEDAL_PARTS MedalNum;
	eAL_COLOR ColorNum;
	bool NonTex; // isMonotone
	eCHAO_JEWEL JewelNum;
	Sint8 MultiNum; // Shiny
	eCHAO_EGGS EggColor;
	eCHAO_BODYTYPE FormNum;
	Sint8 FormSubNum; // Animal
	Sint8 UNDEF0;
};

struct AL_TIME
{
	Sint8 minute;
	Sint8 second;
	Sint8 frame;
};

struct AL_RACE_PERSONAL_INFO
{
	AL_TIME PersonalRecord[10];
	Sint8 nbWin[10];
	short MedalFlag;
};

struct AL_KARATE_PERSONAL_INFO
{
	Sint8 rank;
	Sint8 level;
	Sint8 tournament;
	Sint8 UNDEF0;
	short nbBattle;
	short nbWin;
	short nbLose;
	short nbDraw;
};

struct AL_PARTS
{
	int MinimalFlag;
	eCHAO_MINIMALS MinimalParts[8];
};

struct AL_EMOTION
{
	short Flag;
	short MoodTimer;
	short IllTimer;
	short Timer;
	Sint8 Mood[8];
	short State[11];
	Sint8 Personality[13];
	Sint8 Taste;
	Sint8 Tv;
	Sint8 Music;
	Sint8 IllState[6];
};

struct AL_KNOWLEDGE_PLAYER
{
	Uint8 like;
	Sint8 fear;
	short distance;
	short meet;
};

struct AL_KNOWLEDGE_CHAO
{
	CHAO_ID id;
	Uint8 like;
	Sint8 fear;
	short distance;
	short meet;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
};

struct AL_KNOWLEDGE_OTHER
{
	Uint8 like;
};

struct AL_KNOWLEDGE_BTL
{
	Sint8 ArtFlag;
	Sint8 DanceFlag;
	Sint8 SongFlag;
	Sint8 MusicFlag;
	short SToyFlag;
	short LToyFlag;
	int KwTimer;
	AL_KNOWLEDGE_PLAYER player[6];
	AL_KNOWLEDGE_CHAO chao[20];
	AL_KNOWLEDGE_OTHER bhv[120];
};

struct AL_GENE
{
	bool IsAnalyzed;
	eCHAO_EGGS EggColor;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	CHAO_ID MotherID;
	CHAO_ID FatherID;
	char MotherName[8];
	char FatherName[8];
	char MGroundMotherName[8];
	char MGroundFatherName[8];
	char FGroundMotherName[8];
	char FGroundFatherName[8];
	Sint8 Abl[16];
	Sint8 LifeTime[2];
	Sint8 HPos[2];
	Sint8 VPos[2];
	Sint8 APos[2];
	Sint8 Personality[2][13];
	Sint8 Taste[2];
	Sint8 Tv[2];
	Sint8 Music[2];
	eAL_COLOR Color[2];
	bool NonTex[2]; // Monotone
	eCHAO_JEWEL Jewel[2];
	Sint8 Multi[2]; // Shiny
	Sint8 EyePos[2];
	Sint8 EyeScl[2];
	Sint8 EyeRatio[2];
	Sint8 EyeColor[2];
};

struct AL_RACE_PERSONAL_INFO_DX
{
	AL_TIME PersonalRecord[5];
};

struct AL_KNOWLEDGE_DX
{
	AL_KNOWLEDGE_PLAYER player[6];
};

struct CHAO_PARAM_GC
{
	Sint8 GBAChao;
	Sint8 GBAEgg;
	Sint8 GBABerry[8];
	Sint8 padding0;
	Sint8 padding1;
	int GBARing; // Rings moving from GBA -> GCN
	Sint8 BootMethed;
	Sint8 Birthplace;
	char name[7];
	Sint8 GBAType;
	Sint8 GBASkin;
	Sint8 GBAMood;
	Sint8 GBABelly;
	Sint8 GBASleepy;
	Sint8 GBALonelyness;
	Sint8 padding2;
	Sint8 Exp[8]; // Progress 0 - 100
	Sint8 Abl[8]; // Grade E - S
	Sint8 Lev[8]; // Level 0 - 99
	short Skill[8]; // Total Points
	short GBAPallete[16];
	Sint8 rmsg[16];
	int runaway;
	Sint8 dummy[4];
	eCHAO_TYPE type;  // Chao Type
	eCHAO_GARDENS place; // Chao Area
	signed short like; // Happiness
	Sint8 ClassNum;
	Sint8 UNDEF0;
	short age; // total clock rollovers
	short old; // adult clock rollovers
	short life; // Liftime1
	short LifeMax; // Liftime2
	short nbSucceed; // reincarnations
	CHAO_ID ChaoID;
	int LifeTimer;
	AL_BODY_INFO body;
	AL_RACE_PERSONAL_INFO race;
	AL_KARATE_PERSONAL_INFO karate;
	Sint8 UNDEF1;
	Sint8 UNDEF2;
	AL_PARTS PartsBTL;
	AL_EMOTION emotion;
	AL_KNOWLEDGE_BTL knowledge;
	AL_GENE gene;
};

struct CHAO_SAVE_INFO
{
	CHAO_PARAM_GC param;
	int IsInitializedDX;
	AL_PARTS partsDX;
	AL_RACE_PERSONAL_INFO_DX raceDX;
	Sint8 UNDEF0;
	AL_KNOWLEDGE_DX knowledgeDX;
	Sint8 UNDEF1[736];
};

struct TMP_PARAM
{
	Sint8 Exp[8];
	Sint8 Abl[8];
	Sint8 Lev[8];
	short Skills[8];
};

struct MOTION_INFO
{
	short mode;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	float frame;
	float start;
	float end;
	float spd;
	void* pMotion;
};

struct MOTION_CTRL
{
	short flag;
	short posture;
	int curr_num;
	int next_num;
	float multi_spd;
	float link_spd;
	float ratio;
	MOTION_INFO minfo[2];
	int table;
};

struct MOTION_TABLE
{
	void* pMotion;
	short mode;
	short posture;
	int next;
	int link_step;
	float start;
	float end;
	float spd;
};

struct KW_BHV_ENTRY
{
	int bhv;
};

struct AL_BEHAVIOR
{
	short Flag;
	short Mode;
	short SubMode;
	short MoveMode;
	int InterruptFlag;
	int Timer;
	int SubTimer;
	int LimitTimer;
	int BehaviorTimer;
	short Intention;
	short IntentionMode;
	short IntentionSubMode;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	int IntentionTimer[18];
	int IntervalTimer[128];
	int FreeWork;
	float MoveRadius;
	NJS_POINT3 BasePos;
	void* PrevFunc;
	int nbBhvFuncEntry;
	int CurrBhvFuncNum;
	void* BhvFuncList[16];
	int ReserveTimerList[16];
	int CurrKwBhvNum;
	KW_BHV_ENTRY KwBhvList[4];
	int dummy[15]; // 16 in SADX
};

struct AL_SHAPE
{
	void* pObject;
	void* CurrObjectList[40];
	ChaoEvos* pObjectList;
	ChaoEvos* pObjectListH;
	ChaoEvos* pObjectListD;
	NJS_POINT3 BodyPos;
	NJS_POINT3 HeadPos;
	NJS_POINT3 LeftHandPos;
	NJS_POINT3 RightHandPos;
	NJS_POINT3 LeftFootPos;
	NJS_POINT3 RightFootPos;
	NJS_POINT3 MouthPos;
	NJS_POINT3 MouthVec;
	NJS_POINT3 LeftEyePos;
	NJS_POINT3 LeftEyeVec;
	NJS_POINT3 RightEyePos;
	NJS_POINT3 RightEyeVec;
	NJS_OBJECT* pLeftHandItemObject;
	NJS_TEXLIST* pLeftHandItemTexlist;
	float LeftHandItemScale;
	float LeftHandItemActiveFlag;
	NJS_OBJECT* pRightHandItemObject;
	NJS_TEXLIST* pRightHandItemTexlist;
	float RightHandItemScale;
	float RightHandItemActiveFlag;
	int palette;
	short Flag;
	short ColorNum;
	short EnvNum;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	int IconColor;
	float SclH;
	float SclV;
	float CamDist;
};

struct AL_FACE_CTRL
{
	int EyeTimer;
	short EyeColorNum;
	short EyeCurrNum;
	short EyeDefaultNum;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	int MouthTimer;
	short MouthCurrNum;
	short MouthDefaultNum;
	float EyePosX;
	float EyePosY;
	float EyeSclX;
	float EyeSclY;
	int Flag;
	NJS_CNK_MODEL* pEyeObject[2];
	NJS_CNK_MODEL* pMouthObject;
	int EyeLidBlinkMode;
	int EyeLidBlinkTimer;
	int EyeLidBlinkAng;
	int EyeLidExpressionMode;
	int EyeLidExpressionTimer;
	int EyeLidExpressionDefaultCloseAng;
	int EyeLidExpressionCurrCloseAng;
	int EyeLidExpressionAimCloseAng;
	int EyeLidExpressionDefaultSlopeAng;
	int EyeLidExpressionCurrSlopeAng;
	int EyeLidExpressionAimSlopeAng;
};

struct AL_ICON_INFO
{
	short Mode;
	short TexNum;
	short Timer;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	NJS_POINT3 Offset;
	NJS_POINT3 Pos;
	NJS_POINT3 Velo;
	NJS_POINT3 Scl;
	NJS_POINT3 SclSpd;
};

struct AL_ICON
{
	short CurrType;
	short NextType;
	int Timer;
	int NextTimer;
	int PuniPhase;
	int PosPhase;
	int Color;
	short TexAnimNum;
	short TexAnimTimer;
	int ang;
	NJS_POINT3 Up; // HeadPosUnitTransPortion
	NJS_POINT3 Pos; // parentPosition
	AL_ICON_INFO Upper;
	AL_ICON_INFO Lower;
};

struct AL_ZONE
{
	short Flag;
	short CurrNum;
	float Ratio;
	NJS_LINE Plane;
};

struct al_perception_link
{
	short info[4];
	float dist;
	int InSightFlag;
	int HearFlag;
	int SmellFlag;
	void* pEntry;
};

struct AL_PERCEPTION_INFO
{
	short nbPerception;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	int InSightFlag;
	int HeardFlag;
	int SmellFlag;
	float NearestDist;
	short NearestNum;
	Sint8 UNDEF2;
	Sint8 UNDEF3;
	al_perception_link list[32];
};

struct AL_PERCEPTION
{
	float SightRange;
	int SightAngle;
	int SightAngleHalf;
	float HearRange;
	float SmellRange;
	AL_PERCEPTION_INFO Player;
	AL_PERCEPTION_INFO Chao;
	AL_PERCEPTION_INFO Fruit;
	AL_PERCEPTION_INFO Tree;
	AL_PERCEPTION_INFO Toy;
	AL_PERCEPTION_INFO Sound;
};

struct CHAOWK // ChaoData1
{
	EntityData1 entity;
	int Timer;
	ObjectMaster* pMayu;
	ObjectMaster* pBookTask;
	int NestFlag;
	ObjectMaster* pAnyTask;
	ObjectMaster* pAimTask;
	int AimNum;
	int RememberNum;
	int pitch;
	float ClimbFirstPos;
	int IsParamCopy;
	CHAO_PARAM_GC* pParamGC;
	TMP_PARAM tmpParam;
	int Stamina;
	int AimStamina;
	int tp;
	int pre_ang[3];
	int ChaoFlag;
	short ColliFormat;
	Sint8 UNDEF0;
	Sint8 UNDEF1;
	float CurrZone;
	MOTION_CTRL MotionCtrl;
	MOTION_CTRL MiniMotionCtrl;
	MOTION_TABLE MiniMotionTable[4];
	AL_BEHAVIOR Behavior;
	AL_SHAPE Shape;
	AL_FACE_CTRL Face;
	AL_ICON Icon;
	AL_ZONE Zone;
	AL_PERCEPTION Perception;
	void* pWork;
};

/* Particle task (allocated) */
struct sp_task
{
	Angle ang;       /* Roll angle */
	Float scl;       /* Sprite scale */
	NJS_POINT3 pos;  /* Sprite position */
	NJS_COLOR color; /* Sprite color */
	Float frame;     /* The integral part is the texture id, the fractional part is basically a timer */
	Angle unk;       /* Angle sometimes set by particules, but seemingly unused in drawing function */
	NJS_POINT3 spd;  /* Velocity */
	Angle angspd;    /* Roll angle speed */
	sp_task* next;   /* Next task for current particle info (linked list) */
	Sint32 flag;     /* Task-specific flag bitfield, see SP_TASK_FLAG enum */
	Float offset;    /* Offset applied after roll */
	sp_task* link;   /* Reserved for linked list */
};

/* Particle information (static) */
struct sp_info
{
	Sint32 flag;                             /* Rendering flags, see SP_FLAG enum */
	NJS_TEXLIST* texlist;                    /* Texlist to use  */
	Sint32 texno;                            /* Start texture num */
	Sint32 animnum;                          /* Number of textures */
	Float animspd;                           /* Texture animation speed: task->no + animspd */
	Float hoffset;                           /* X, Y and Z offset */
	Float voffset;                           /* Y offset */
	Float sclspd;                            /* Scaling speed: task->spd + sclspd */
	Bool(__cdecl* exec)(sp_info*, sp_task*); /* Subtask function */
	Float unk24;
	const sp_task* cst_task;                 /* Default task parameters (optional) */
	Sint32 unk2C;
	sp_task* tasks;                          /* Tasks linked list */
	sp_info* link;                           /* Reserved for linked list */
};

typedef motionwk motionwk2;

struct PAKTexInf
{
	char filename[28];
	Uint32 globalIndex;
	Uint32 Type;
	Uint32 BitDepth;
	Uint32 PixelFormat;
	Uint32 nWidth;
	Uint32 nHeight;
	Uint32 TextureSize;
	Uint32 fSurfaceFlags;
};

struct CreditsEntry
{
    int Type;
    float A, R, G, B;
    const char* Text;
};

struct MenuBackgrounds
{
	char* filename;
	NJS_TEXLIST* texlist;
};

#pragma pack(pop)