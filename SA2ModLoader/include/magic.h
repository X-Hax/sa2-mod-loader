#pragma once

#include <d3d9.h>
#include <dinput.h>
#include <stdint.h>
#include <XInput.h>
#include <map>
#include <vector>
#include <list>
#include "MemAccess.h"

namespace Magic
{
	namespace Base
	{
		template <typename T>
		class Singleton
		{
		private:
			static T* const m_pSubject;
		public:
			T* GetSubject()
			{
				if(!m_pSubject)
				{
					m_pSubject = new T();
				}
				return m_pSubject;
			}
		};
	}

	namespace MATH
	{
		template <typename T>
		struct Vector4
		{
			T x;
			T y;
			T z;
			T w;
		};

		template <typename T>
		struct Vector3
		{
			T x;
			T y;
			T z;
		};

		template <typename T>
		struct Matrix4
		{
			union
			{
				T m[4][4];
				T m2[16];
			};
		};
	}

	namespace INPUT
	{
		enum KEYBTN_CODE : uint32_t // Basically stripped DIK enum
		{
			KEYBTN_NULL = 0xFFFFFFFF,
			KEYBTN_ESCAPE = 0x0,
			KEYBTN_1 = 0x1,
			KEYBTN_2 = 0x2,
			KEYBTN_3 = 0x3,
			KEYBTN_4 = 0x4,
			KEYBTN_5 = 0x5,
			KEYBTN_6 = 0x6,
			KEYBTN_7 = 0x7,
			KEYBTN_8 = 0x8,
			KEYBTN_9 = 0x9,
			KEYBTN_0 = 0xA,
			KEYBTN_MINUS = 0xB,
			KEYBTN_EQUALS = 0xC,
			KEYBTN_BACK = 0xD,
			KEYBTN_TAB = 0xE,
			KEYBTN_Q = 0xF,
			KEYBTN_W = 0x10,
			KEYBTN_E = 0x11,
			KEYBTN_R = 0x12,
			KEYBTN_T = 0x13,
			KEYBTN_Y = 0x14,
			KEYBTN_U = 0x15,
			KEYBTN_I = 0x16,
			KEYBTN_O = 0x17,
			KEYBTN_P = 0x18,
			KEYBTN_LBRACKET = 0x19,
			KEYBTN_RBRACKET = 0x1A,
			KEYBTN_RETURN = 0x1B,
			KEYBTN_LCONTROL = 0x1C,
			KEYBTN_A = 0x1D,
			KEYBTN_S = 0x1E,
			KEYBTN_D = 0x1F,
			KEYBTN_F = 0x20,
			KEYBTN_G = 0x21,
			KEYBTN_H = 0x22,
			KEYBTN_J = 0x23,
			KEYBTN_K = 0x24,
			KEYBTN_L = 0x25,
			KEYBTN_SEMICOLON = 0x26,
			KEYBTN_APOSTROPHE = 0x27,
			KEYBTN_GRAVE = 0x28,
			KEYBTN_LSHIFT = 0x29,
			KEYBTN_BACKSLASH = 0x2A,
			KEYBTN_Z = 0x2B,
			KEYBTN_X = 0x2C,
			KEYBTN_C = 0x2D,
			KEYBTN_V = 0x2E,
			KEYBTN_B = 0x2F,
			KEYBTN_N = 0x30,
			KEYBTN_M = 0x31,
			KEYBTN_COMMA = 0x32,
			KEYBTN_PERIOD = 0x33,
			KEYBTN_SLASH = 0x34,
			KEYBTN_RSHIFT = 0x35,
			KEYBTN_MULTIPLY = 0x36,
			KEYBTN_LMENU = 0x37,
			KEYBTN_SPACE = 0x38,
			KEYBTN_CAPITAL = 0x39,
			KEYBTN_F1 = 0x3A,
			KEYBTN_F2 = 0x3B,
			KEYBTN_F3 = 0x3C,
			KEYBTN_F4 = 0x3D,
			KEYBTN_F5 = 0x3E,
			KEYBTN_F6 = 0x3F,
			KEYBTN_F7 = 0x40,
			KEYBTN_F8 = 0x41,
			KEYBTN_F9 = 0x42,
			KEYBTN_F10 = 0x43,
			KEYBTN_NUMLOCK = 0x44,
			KEYBTN_SCROLL = 0x45,
			KEYBTN_NUMPAD7 = 0x46,
			KEYBTN_NUMPAD8 = 0x47,
			KEYBTN_NUMPAD9 = 0x48,
			KEYBTN_SUBTRACT = 0x49,
			KEYBTN_NUMPAD4 = 0x4A,
			KEYBTN_NUMPAD5 = 0x4B,
			KEYBTN_NUMPAD6 = 0x4C,
			KEYBTN_ADD = 0x4D,
			KEYBTN_NUMPAD1 = 0x4E,
			KEYBTN_NUMPAD2 = 0x4F,
			KEYBTN_NUMPAD3 = 0x50,
			KEYBTN_NUMPAD0 = 0x51,
			KEYBTN_DECIMAL = 0x52,
			KEYBTN_OEM_102 = 0x53,
			KEYBTN_F11 = 0x54,
			KEYBTN_F12 = 0x55,
			KEYBTN_F13 = 0x56,
			KEYBTN_F14 = 0x57,
			KEYBTN_F15 = 0x58,
			KEYBTN_KANA = 0x59,
			KEYBTN_ABNT_C1 = 0x5A,
			KEYBTN_CONVERT = 0x5B,
			KEYBTN_NOCONVERT = 0x5C,
			KEYBTN_YEN = 0x5D,
			KEYBTN_ABNT_C2 = 0x5E,
			KEYBTN_NUMPADEQUALS = 0x5F,
			KEYBTN_PREVTRACK = 0x60,
			KEYBTN_AT = 0x61,
			KEYBTN_COLON = 0x62,
			KEYBTN_UNDERLINE = 0x63,
			KEYBTN_KANJI = 0x64,
			KEYBTN_STOP = 0x65,
			KEYBTN_AX = 0x66,
			KEYBTN_UNLABELED = 0x67,
			KEYBTN_NEXTTRACK = 0x68,
			KEYBTN_NUMPADENTER = 0x69,
			KEYBTN_RCONTROL = 0x6A,
			KEYBTN_MUTE = 0x6B,
			KEYBTN_CALCULATOR = 0x6C,
			KEYBTN_PLAYPAUSE = 0x6D,
			KEYBTN_MEDIASTOP = 0x6E,
			KEYBTN_VOLUMEDOWN = 0x6F,
			KEYBTN_VOLUMEUP = 0x70,
			KEYBTN_WEBHOME = 0x71,
			KEYBTN_NUMPADCOMMA = 0x72,
			KEYBTN_DIVIDE = 0x73,
			KEYBTN_SYSRQ = 0x74,
			KEYBTN_RMENU = 0x75,
			KEYBTN_PAUSE = 0x76,
			KEYBTN_HOME = 0x77,
			KEYBTN_UP = 0x78,
			KEYBTN_PRIOR = 0x79,
			KEYBTN_LEFT = 0x7A,
			KEYBTN_RIGHT = 0x7B,
			KEYBTN_END = 0x7C,
			KEYBTN_DOWN = 0x7D,
			KEYBTN_NEXT = 0x7E,
			KEYBTN_INSERT = 0x7F,
			KEYBTN_DELETE = 0x80,
			KEYBTN_LWIN = 0x81,
			KEYBTN_RWIN = 0x82,
			KEYBTN_APPS = 0x83,
			KEYBTN_POWER = 0x84,
			KEYBTN_SLEEP = 0x85,
			KEYBTN_WAKE = 0x86,
			KEYBTN_WEBSEARCH = 0x87,
			KEYBTN_WEBFAVORITES = 0x88,
			KEYBTN_WEBREFRESH = 0x89,
			KEYBTN_WEBSTOP = 0x8A,
			KEYBTN_WEBFORWARD = 0x8B,
			KEYBTN_WEBBACK = 0x8C,
			KEYBTN_MYCOMPUTER = 0x8D,
			KEYBTN_MAIL = 0x8E,
			KEYBTN_MEDIASELECT = 0x8F,
			KEYBTN_MAX = 0x90,
		};

		enum JOYBTN_CODE : uint32_t
		{
			JOYBTN_NULL = 0xFFFFFFFF,
			JOYBTN_SINGLE_BEGIN = 0x0,
			JOYBTN_0 = 0x0,
			JOYBTN_A = 0x0,
			JOYBTN_CROSS = 0x0,
			JOYBTN_1 = 0x1,
			JOYBTN_B = 0x1,
			JOYBTN_CIRCLE = 0x1,
			JOYBTN_2 = 0x2,
			JOYBTN_X = 0x2,
			JOYBTN_SQUARE = 0x2,
			JOYBTN_3 = 0x3,
			JOYBTN_Y = 0x3,
			JOYBTN_TRIANGLE = 0x3,
			JOYBTN_4 = 0x4,
			JOYBTN_LSHOULDER1 = 0x4,
			JOYBTN_L1 = 0x4,
			JOYBTN_5 = 0x5,
			JOYBTN_RSHOULDER1 = 0x5,
			JOYBTN_R1 = 0x5,
			JOYBTN_6 = 0x6,
			JOYBTN_BACK = 0x6,
			JOYBTN_7 = 0x7,
			JOYBTN_START = 0x7,
			JOYBTN_8 = 0x8,
			JOYBTN_LTHUMB_PUSH = 0x8,
			JOYBTN_L3 = 0x8,
			JOYBTN_9 = 0x9,
			JOYBTN_RTHUMB_PUSH = 0x9,
			JOYBTN_R3 = 0x9,
			JOYBTN_10 = 0xA,
			JOYBTN_SELECT = 0xA,
			JOYBTN_11 = 0xB,
			JOYBTN_BIGBUTTON = 0xB,
			JOYBTN_12 = 0xC,
			JOYBTN_13 = 0xD,
			JOYBTN_14 = 0xE,
			JOYBTN_15 = 0xF,
			JOYBTN_16 = 0x10,
			JOYBTN_17 = 0x11,
			JOYBTN_18 = 0x12,
			JOYBTN_19 = 0x13,
			JOYBTN_20 = 0x14,
			JOYBTN_21 = 0x15,
			JOYBTN_22 = 0x16,
			JOYBTN_23 = 0x17,
			JOYBTN_24 = 0x18,
			JOYBTN_25 = 0x19,
			JOYBTN_26 = 0x1A,
			JOYBTN_27 = 0x1B,
			JOYBTN_28 = 0x1C,
			JOYBTN_29 = 0x1D,
			JOYBTN_30 = 0x1E,
			JOYBTN_31 = 0x1F,
			JOYBTN_UP = 0x20,
			JOYBTN_DOWN = 0x21,
			JOYBTN_LEFT = 0x22,
			JOYBTN_RIGHT = 0x23,
			JOYBTN_SINGLE_END = 0x24,
			JOYBTN_INTERZONE_BEGIN = 0x24,
			JOYBTN_LTRIGGER = 0x24,
			JOYBTN_L2 = 0x24,
			JOYBTN_RTRIGGER = 0x25,
			JOYBTN_R2 = 0x25,
			JOYBTN_LTHUMBX = 0x26,
			JOYBTN_LTHUMBY = 0x27,
			JOYBTN_RTHUMBX = 0x28,
			JOYBTN_RTHUMBY = 0x29,
			JOYBTN_SENSOR_X = 0x2A,
			JOYBTN_SENSOR_Y = 0x2B,
			JOYBTN_SENSOR_Z = 0x2C,
			JOYBTN_SENSOR_G = 0x2D,
			JOYBTN_INTERZONE_END = 0x2E,
			JOYBTN_POS_BEGIN = 0x2E,
			JOYBTN_LTHUMB_STICK = 0x2E,
			JOYBTN_RTHUMB_STICK = 0x2F,
			JOYBTN_POS_END = 0x30,
			JOYBTN_MAX = 0x30,
		};

		enum MOUSEBTN_CODE : uint32_t
		{
			MOUSEBTN_LB = 0x0,
			MOUSEBTN_RB = 0x1,
			MOUSEBTN_MB = 0x2,
		};

		struct Keystroke
		{
			KEYBTN_CODE m_KeyCode;
			int m_Flags;
			int unknown3;
		};

		struct IPoint
		{
			float x;
			float y;
		};

		struct Controller
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Controller* _this);
				bool(__thiscall* IsMouse)(Controller* _this);
				bool(__thiscall* IsKeyboard)(Controller* _this);
				bool(__thiscall* IsJoystick)(Controller* _this);
				bool(__thiscall* IsValid)(Controller* _this);
			}* __vftable;
		};

		struct Joystick /* : Controller */
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Joystick* _this);
				bool(__thiscall* IsMouse)(Joystick* _this);
				bool(__thiscall* IsKeyboard)(Joystick* _this);
				bool(__thiscall* IsJoystick)(Joystick* _this);
				bool(__thiscall* IsValid)(Joystick* _this);
				void(__thiscall* Initialize)(Joystick* _this);
				void(__thiscall* Finalize)(Joystick* _this);
				void(__thiscall* Update)(Joystick* _this);
				bool(__thiscall* IsPress)(Joystick* _this, JOYBTN_CODE);
				bool(__thiscall* IsRelease)(Joystick* _this, JOYBTN_CODE);
				bool(__thiscall* IsLast)(Joystick* _this, JOYBTN_CODE);
				bool(__thiscall* IsUp)(Joystick* _this, JOYBTN_CODE);
				bool(__thiscall* IsDown)(Joystick* _this, JOYBTN_CODE);
				bool(__thiscall* Unknown13)(Joystick* _this, int, int);
				float(__thiscall* GetValue)(Joystick* _this, JOYBTN_CODE);
				IPoint*(__thiscall* GetPos)(Joystick* _this, IPoint* result, JOYBTN_CODE);
				bool(__thiscall* VibStart)(Joystick* _this, int left, int right);
				bool(__thiscall* VibStop)(Joystick* _this);
				bool(__thiscall* Unknown18)(Joystick* _this, int, int);
				bool(__thiscall* Unknown19)(Joystick* _this);
			}* __vftable;
			int unknown;
			int m_ControllerIndex;
			char initialized;
			XINPUT_STATE is[2];
			int index_prevpool;
			int index_currentpool;
			_XINPUT_VIBRATION vibration;
			char isVibrating;
		};

		struct Keyboard /* : Controller */
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Keyboard* _this);
				bool(__thiscall* IsMouse)(Keyboard* _this);
				bool(__thiscall* IsKeyboard)(Keyboard* _this);
				bool(__thiscall* IsJoystick)(Keyboard* _this);
				bool(__thiscall* IsValid)(Keyboard* _this);
				void(__thiscall* Initialize)(Keyboard* _this);
				void(__thiscall* Finalize)(Keyboard* _this);
				void(__thiscall* Update)(Keyboard* _this);
				bool(__thiscall* IsPress)(Keyboard* _this, KEYBTN_CODE);
				bool(__thiscall* IsRelease)(Keyboard* _this, KEYBTN_CODE);
				bool(__thiscall* IsLast)(Keyboard* _this, KEYBTN_CODE);
				bool(__thiscall* IsUp)(Keyboard* _this, KEYBTN_CODE);
				bool(__thiscall* IsDown)(Keyboard* _this, KEYBTN_CODE);
				bool(__thiscall* GetKeystroke)(Keyboard* _this, Keystroke*);
			}* __vftable;
			int unknown1;
			LPDIRECTINPUT8 api;
			int unknown3;
			LPDIRECTINPUTDEVICE device;
			Keystroke ks[32];
			BYTE a6[128];
			int ks_index;
			int ks_previndex;
			BYTE current_keys[256];
			BYTE last_keys[256];
			int index_prevpool;
			int index_currentpool;
			char initialized;
		};

		struct Mouse /* : Controller */
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Mouse* _this);
				bool(__thiscall* IsMouse)(Mouse* _this);
				bool(__thiscall* IsKeyboard)(Mouse* _this);
				bool(__thiscall* IsJoystick)(Mouse* _this);
				bool(__thiscall* IsValid)(Mouse* _this);
				void(__thiscall* Initialize)(Mouse* _this);
				void(__thiscall* Finalize)(Mouse* _this);
				void(__thiscall* Update)(Mouse* _this);
				bool(__thiscall* IsPress)(Mouse* _this, MOUSEBTN_CODE);
				bool(__thiscall* IsRelease)(Mouse* _this, MOUSEBTN_CODE);
				bool(__thiscall* IsLast)(Mouse* _this, MOUSEBTN_CODE);
				bool(__thiscall* IsUp)(Mouse* _this, MOUSEBTN_CODE);
				bool(__thiscall* IsDown)(Mouse* _this, MOUSEBTN_CODE);
				int(__thiscall* GetLX)(Mouse* _this);
				int(__thiscall* GetLY)(Mouse* _this);
				int(__thiscall* GetLZ)(Mouse* _this);
				void(__thiscall* GetPos)(Mouse* _this, int* x, int* y);
				IPoint*(__cdecl* GetNormalPos)(Mouse* _this, IPoint* result);

			}* __vftable;
		};

		struct InputImpl
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(InputImpl* _this);
				void(__thiscall* Initialize)(InputImpl* _this);
				void(__thiscall* Finalize)(InputImpl* _this);
				void(__thiscall* Update)(InputImpl* _this);
				Mouse* (__thiscall* GetMouse)(InputImpl* _this, int);
				Keyboard* (__thiscall* GetKeyboard)(InputImpl* _this, int);
				Joystick* (__thiscall* GetJoystick)(InputImpl* _this, int);
				Controller* (__thiscall* GetController)(InputImpl* _this, int);
				void(__thiscall* WndProc)(InputImpl* _this, void*, unsigned int, unsigned int, int);
				int(__thiscall* GetMouseNum)(InputImpl* _this);
				int(__thiscall* GetKeyboardNum)(InputImpl* _this);
				int(__thiscall* GetJoystickNum)(InputImpl* _this);
				void(__thiscall* Unknown)(InputImpl* _this);
			}* __vftable;
			LPDIRECTINPUT8 dinput;
			int unknown2;
			int unknown3;
			int unknown4;
			Keyboard* keyboard;
			int unknown6[6 * 4];
		};

		struct Input
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Input* _this);
				void(__thiscall* Initialize)(Input* _this);
				void(__thiscall* Finalize)(Input* _this);
				void(__thiscall* Update)(Input* _this);
				Mouse* (__thiscall* GetMouse)(Input* _this, int);
				Keyboard* (__thiscall* GetKeyboard)(Input* _this, int);
				Joystick* (__thiscall* GetJoystick)(Input* _this, int);
				Controller* (__thiscall* GetController)(Input* _this, int);
				void(__thiscall* WndProc)(Input* _this, void*, unsigned int, unsigned int, int);
				int(__thiscall* GetMouseNum)(Input* _this);
				int(__thiscall* GetKeyboardNum)(Input* _this);
				int(__thiscall* GetJoystickNum)(Input* _this);
				void(__thiscall* Unknown)(Input* _this);
			}* __vftable;
			InputImpl* m_pInputImpl;
		};
	}

	namespace RenderCore
	{
		struct Sampler;
		struct Resource;
		struct Material;
		struct Texture;
		struct InputLayout;
		struct RenderDevice;
		struct ResourceProfiler;
		struct DX9_DeviceCreator_Base;
		struct VideoDevice;
		struct DeviceConfig_t;

		struct VideoDevice
		{
			struct vtbl
			{
				void(__cdecl* Destructor)(VideoDevice* _this);
				int(__cdecl* GetDisplayWidth)(VideoDevice* _this);
				int(__cdecl* GetDisplayHeight)(VideoDevice* _this);
				int(__cdecl* GetDisplayFormat)(VideoDevice* _this);
				bool(__cdecl* IsWideScreen)(VideoDevice* _this);
				float(__cdecl* GetRefreshRate)(VideoDevice* _this);
			}* __vftable;
		};

		struct DX9_VideoDevice_Base : VideoDevice
		{
			LPDIRECT3D9 m_pD3D;
		};

		struct DX9_VideoDevice_PC : DX9_VideoDevice_Base
		{
			D3DDISPLAYMODE m_VideoMode;
		};

		struct DX9_DeviceCreator_Base
		{
			struct vtbl
			{
				bool(__cdecl* CreateD3DDevice)(DX9_DeviceCreator_Base* _this, const DeviceConfig_t*);
				void(__cdecl* FreeD3DDevice)(DX9_DeviceCreator_Base* _this, char flag);
			}* __vftable;
			LPDIRECT3D9 m_pD3D;
			LPDIRECT3DDEVICE9 m_pD3DDevice;
			D3DPRESENT_PARAMETERS m_D3DPP;
			int m_ShaderModel;
			int m_VideoMemorySize;
		};

		struct DeviceConfig_t
		{
			unsigned int m_BackBufferFormat;
			unsigned int m_DepthStencilFormat;
			unsigned int m_BackBufferWidth;
			unsigned int m_BackBufferHeight;
			HWND__* m_hWnd;
			bool m_IsFullScreen;
			bool m_IsVSyn;
			int m_RefreshRate;
			int m_Display;
			void* m_pUserData;
			int field_28;
		};

		struct RenderDeviceInitInfo_t
		{
			bool m_IsWindow;
			int m_BackBufferWidth;
			int m_BackBufferHeight;
			int m_BackBufferFormat;
			int m_DepthStencilFormat;
			int m_MultiSampleType;
			bool m_IsVSyn;
		};

		struct SamplerState_t
		{
			int m_MinFilter;
			int m_MagFilter;
			int m_MipFilter;
			int m_AddressU;
			int m_AddressV;
			int m_AddressW;
			int m_BorderColor;
		};

		template<typename T>
		struct ShaderParam
		{
			std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_Name;
			T m_Param;
			void* m_Handle;
			int m_SamplerIndex;
			bool m_IsUnused;
		};

		struct Sampler : SamplerState_t
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Sampler* _this);
			}* __vftable;
			std::basic_string<char, std::char_traits<char>, std::allocator<char> > m_Name;
			Texture* m_pTexture;
			int m_Index;
		};

		template<typename T>
		struct TrieShaderParam
		{
			void* root;
			T* m_Container;
			unsigned int m_Size;
		};

		template<typename T>
		struct ShaderParamPool
		{
			TrieShaderParam<ShaderParam<T>*> m_Params;
		};

		struct ShaderParamData
		{
			ShaderParamPool<bool> m_BoolParamPool;
			ShaderParamPool<int> m_IntParamPool;
			ShaderParamPool<float> m_FloatParamPool;
			ShaderParamPool<Magic::MATH::Vector4<float> > m_VectorParamPool;
			ShaderParamPool<Magic::MATH::Matrix4<float> > m_MatrixParamPool;
			ShaderParamPool<Magic::RenderCore::Sampler*> m_SamplerParamPool;
		};

		struct GSphere_t
		{
			MATH::Vector3<float> m_Center;
			float m_Radius;
		};

		struct GBoundingBox_t
		{
			MATH::Vector3<float> m_Min;
			MATH::Vector3<float> m_Max;
		};

		struct GRect_t
		{
			float m_Left;
			float m_Top;
			float m_Right;
			float m_Bottom;
		};

		struct Resource
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(Resource* _this);
				int(__thiscall* GetSize)(Resource* _this);
				bool(__thiscall* IsLoaded)(Resource* _this);
			}* __vftable;
			int m_ResType;
			int m_ResPool;
			void* m_pUserData;
			unsigned int m_ResMask;
			std::basic_string<char, std::char_traits<char>, std::allocator<char>> m_Name;
			std::basic_string<char, std::char_traits<char>, std::allocator<char>> m_Path;
		};

		struct VertexBuffer : Resource
		{
			enum PrimitiveType : __int32
			{
				PT_POINT_LIST = 0x0,
				PT_LINE_LIST = 0x1,
				PT_LINE_STRIP = 0x2,
				PT_TRIANGLE_LIST = 0x3,
				PT_TRIANGLE_STRIP = 0x4,
				PT_TRIANGLE_FAN = 0x5,
				PT_MAX = 0x6,
			};

			PrimitiveType m_PrimitiveType;
			int m_VertexNum;
			int m_VertexSize;
			bool m_IsLocked;
		};

		struct IndexBuffer : Resource
		{
			enum IndexSizeType : __int32
			{
				INDEX_SIZE_16 = 0x0,
				INDEX_SIZE_32 = 0x1,
			};

			int m_IndexNum;
			int m_IndexSize;
			bool m_IsLocked;
		};

		struct ShaderStage : Resource
		{
			int m_ShaderStageType;
			void* m_ConstTableHandle;
			int m_ShaderCodeSize;
		};

		struct PixelShader : ShaderStage {};
		struct VertexShader : ShaderStage {};

		struct ShaderGroup
		{
			VertexShader* m_pVS;
			PixelShader* m_pPS;
		};

		struct Material : Resource
		{
			int m_MaterialState;
			int m_AlphaRef;
			ShaderParamData m_ShaderParamData[2];
			VertexShader* m_pVertexShader;
			PixelShader* m_pPixelShader;
		};

		struct Mesh : Resource
		{
			int m_IndexBase;
			int m_IndexCount;
			int m_VertexBase;
			int m_VertexCount;
			GBoundingBox_t m_BoundingBox;
			GSphere_t m_BoundingSphere;
			VertexBuffer* m_pVertexBuffer;
			IndexBuffer* m_pIndexBuffer;
			Material* m_pMaterial;
			MATH::Matrix4<float> m_LocalTrans;
		};

		struct Texture : Resource
		{
			int m_TexType;
			int m_TexSize;
			int m_TexFlags;
			int m_Width;
			int m_Height;
			int m_Depth;
			int m_Format;
			int m_Usage;
			int m_MipmapLevelCount;
			bool m_IsLocked;
		};

		struct MultiRenderTargets_t
		{
			Texture* m_RenderTarget0;
			Texture* m_RenderTarget1;
			Texture* m_RenderTarget2;
			Texture* m_RenderTarget3;
			Texture* m_DepthStencil;
		};

		struct InputElementDesc_t
		{
			int m_StreamIndex;
			int m_Offset;
			int m_Format;
			int m_Method;
			int m_SemanticName;
			int m_SemanticIndex;
		};

		struct InputLayout
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(InputLayout* _this);
			}* __vftable;
			std::vector<InputElementDesc_t, std::allocator<InputElementDesc_t>> m_InputElements;
			void* m_pData;
			int m_StreamCount;
			int m_StreamSize[3];
			int m_UniqueID;
		};

		struct CallbackFunc // custom
		{
			void(__thiscall* Destructor)(CallbackFunc*, char flag);
			void(__fastcall* Function)(CallbackFunc*);
		};

		typedef CallbackFunc OnDeviceLostFunc;
		typedef CallbackFunc OnDeviceResetFunc;

		struct ResourceProfiler
		{
			struct vtbl
			{
				void(__cdecl* Destructor)(ResourceProfiler* _this);
			}* __vftable /*VFT*/;
			std::list<Magic::RenderCore::Resource*, std::allocator<Magic::RenderCore::Resource*> > m_pResources[11];
		};

		struct RenderDevice
		{
			struct vtbl
			{
				void(__thiscall* Destructor)(RenderDevice* _this, char flag);
				void(__thiscall* InitializeThing)(RenderDevice* _this);
				void(__thiscall* FreeThing)(RenderDevice* _this);
				void(__thiscall* GetRenderAPIName)(RenderDevice* _this, LPCSTR*); // returns "directx_9"
				RenderDevice* (__thiscall* GetRenderAPI)(RenderDevice* _this);
				void(__thiscall* GetTextureCaps)(RenderDevice* _this, LPVOID unknown_struct);
				int(__thiscall* GetVideoMemorySize)(RenderDevice* _this);
				int(__thiscall* GetShaderModel)(RenderDevice* _this);
				void(__thiscall* GetRenderDeviceInitInfo)(RenderDevice* _this, RenderDeviceInitInfo_t*);
				void(__thiscall* ResetRenderDeviceInitInfo)(RenderDevice* _this, RenderDeviceInitInfo_t*, OnDeviceLostFunc**, OnDeviceResetFunc**);
				void(__thiscall* BeginFrame)(RenderDevice* _this);
				void(__thiscall* EndFrame)(RenderDevice* _this);
				void(__thiscall* BeginRenderToTargets)(RenderDevice* _this, MultiRenderTargets_t*);
				void(__thiscall* EndRenderToTargets)(RenderDevice* _this, bool);
				void(__thiscall* Present)(RenderDevice* _this);
				void(__thiscall* ClearFrameBuffer)(RenderDevice* _this, unsigned int, D3DCOLOR* Color, float Z, unsigned int Stencil);
				void(__thiscall* DrawPrimitive1)(RenderDevice* _this, InputLayout*, VertexBuffer*, int, int, IndexBuffer*, int, int);
				void(__thiscall* DrawPrimitive2)(RenderDevice* _this, InputLayout*, Mesh*);
				void(__thiscall* DrawPrimitive3)(RenderDevice* _this, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);
				void(__thiscall* DrawPrimitiveInstance1)(RenderDevice* _this, InputLayout*, VertexBuffer*, int, int, IndexBuffer*, int, int, VertexBuffer*, int, int, int);
				void(__thiscall* DrawPrimitiveInstance2)(RenderDevice* _this, InputLayout*, Mesh*, VertexBuffer*);
				void(__thiscall* DrawPrimitiveDirectly)(RenderDevice* _this, InputLayout*, int, int, void*, int);
				void(__thiscall* GetRenderState)(RenderDevice* _this, unsigned int, DWORD* pValue);
				void(__thiscall* SetRenderState)(RenderDevice* _this, unsigned int, DWORD Value);
				void(__thiscall* GetRenderStateRaw)(RenderDevice* _this, unsigned int, DWORD* pValue);
				void(__thiscall* SetRenderStateRaw)(RenderDevice* _this, unsigned int, DWORD Value);
				MATH::Vector4<float>* (__thiscall* GetViewport)(RenderDevice* _this, MATH::Vector4<float>* result);
				void(__thiscall* SetViewport)(RenderDevice* _this, const MATH::Vector4<float>*);
				void(__thiscall* GetScissorRect)(RenderDevice* _this, const MATH::Vector4<float>*);
				void(__thiscall* SetScissorRect)(RenderDevice* _this, const MATH::Vector4<float>*);
				void(__thiscall* StoreDepthStencilSurface)(RenderDevice* _this, Texture*);
				void(__thiscall* LoadDepthStencilSurface)(RenderDevice* _this, Texture*);
				void(__thiscall* UnloadDepthStencilSurface)(RenderDevice* _this, Texture*);
				void(__thiscall* RestoreDepthStencilSurface)(RenderDevice* _this, Texture*);
				void(__thiscall* StoreRenderTarget)(RenderDevice* _this, int, Texture*);
				void(__thiscall* LoadRenderTarget)(RenderDevice* _this, int, Texture*);
				void(__thiscall* LoadCubeRenderTarget)(RenderDevice* _this, int, Texture*, int);
				LPVOID Unknown37;
				void(__thiscall* UnloadRenderTarget)(RenderDevice* _this, int, Texture*);
				void(__thiscall* RestoreRenderTarget)(RenderDevice* _this, int, Texture*);
				void(__thiscall* LockShader)(RenderDevice* _this, const ShaderGroup*);
				void(__thiscall* UnlockShader)(RenderDevice* _this);
				bool(__thiscall* IsShaderLocked)(RenderDevice* _this);
				VertexShader* (__thiscall* GetVertexShader)(RenderDevice* _this);
				void(__thiscall* SetVertexShader)(RenderDevice* _this, VertexShader*);
				void(__thiscall* UnsetVertexShader)(RenderDevice* _this);
				PixelShader* (__thiscall* GetPixelShader)(RenderDevice* _this);
				void(__thiscall* SetPixelShader)(RenderDevice* _this, PixelShader*);
				void(__thiscall* UnsetPixelShader)(RenderDevice* _this);
				ShaderGroup* (__thiscall* GetShader)(RenderDevice* _this, ShaderGroup* result);
				void(__thiscall* SetShader1)(RenderDevice* _this, const ShaderGroup*);
				void(__thiscall* SetShader2)(RenderDevice* _this, VertexShader*, PixelShader*);
				void(__thiscall* UnsetShader)(RenderDevice* _this);
				void(__thiscall* ShaderSetConstant1)(RenderDevice* _this, ShaderStage*, const char*, Sampler*, void**, int*);
				void(__thiscall* ShaderSetConstant2)(RenderDevice* _this, ShaderStage*, const char*, const MATH::Matrix4<float>*, void**);
				void(__thiscall* ShaderSetConstant3)(RenderDevice* _this, ShaderStage*, const char*, const MATH::Vector4<float>*, void**);
				void(__thiscall* ShaderSetConstant4)(RenderDevice* _this, ShaderStage*, const char*, float, void**);
				void(__thiscall* ShaderSetConstant5)(RenderDevice* _this, ShaderStage*, const char*, int, void**);
				void(__thiscall* ShaderSetConstant6)(RenderDevice* _this, ShaderStage*, const char*, bool, void**);
				void(__thiscall* ShaderSetConstant7)(RenderDevice* _this, int, int, Texture*, const SamplerState_t*);
				void(__thiscall* ShaderSetConstant8)(RenderDevice* _this, int, int, const Magic::MATH::Matrix4<float>*);
				void(__thiscall* ShaderSetConstant9)(RenderDevice* _this, int, int, const Magic::MATH::Vector4<float>*);
				void(__thiscall* ShaderSetConstant10)(RenderDevice* _this, int, int, const float*, int);
				void(__thiscall* ShaderSetConstant11)(RenderDevice* _this, int, int, const int*, int);
				void(__thiscall* ShaderSetConstant12)(RenderDevice* _this, int, int, const bool*, int);
				bool(__thiscall* CheckDeviceLost)(RenderDevice* _this, OnDeviceLostFunc*, OnDeviceResetFunc*);
				LPVOID Unknown66;
				LPVOID Unknown67;
				void(__thiscall* SaveBackBufferToFile)(RenderDevice* _this, const std::string*, int);
				void(__thiscall* BuildMipmaps)(RenderDevice* _this, Texture*);
				void(__thiscall* SetVerticalBlankCallback)(RenderDevice* _this, void(__cdecl*)(void*));
				bool(__thiscall* LoadInputLayout)(RenderDevice* _this, InputLayout*, InputElementDesc_t*);
				void(__thiscall* UnloadInputLayout)(RenderDevice* _this, InputLayout*);
				bool(__thiscall* LoadVertexBuffer)(RenderDevice* _this, VertexBuffer*, BYTE* pbData, BYTE* pbBuffer, UINT Length, D3DPOOL);
				void(__thiscall* UnloadVertexBuffer)(RenderDevice* _this, VertexBuffer*);
				bool(__thiscall* LockVertexBuffer)(RenderDevice* _this, VertexBuffer*, BYTE** ppbData, UINT OffsetToLock, UINT SizeToLock, DWORD Flags);
				void(__thiscall* UnlockVertexBuffer)(RenderDevice* _this, VertexBuffer*);
				bool(__thiscall* LoadIndexBuffer)(RenderDevice* _this, IndexBuffer*, BYTE* pbData, BYTE* pbBuffer, UINT Length, D3DPOOL);
				void(__thiscall* UnloadIndexBuffer)(RenderDevice* _this, IndexBuffer*);
				bool(__thiscall* LockIndexBuffer)(RenderDevice* _this, IndexBuffer*, BYTE** ppbData, UINT OffsetToLock, UINT SizeToLock, DWORD Flags);
				void(__thiscall* UnlockIndexBuffer)(RenderDevice* _this, IndexBuffer*);
				bool(__thiscall* CreateTexture)(RenderDevice* _this, Texture*, int Type, UINT* pWdith, UINT* pHeight, UINT* pDepth, D3DFORMAT* pFormat, UINT* pMipLevel, D3DPOOL* pPool, int*);
				bool(__thiscall* LoadTexture)(RenderDevice* _this, Texture*, int, void*, int, int*, int*, int*, int*, int*, int*);
				void(__thiscall* UnloadTexture)(RenderDevice* _this, Texture*);
				bool(__thiscall* LockTexture)(RenderDevice* _this, Texture*, int, void**, int*, GRect_t*, int);
				void(__thiscall* UnlockTexture)(RenderDevice* _this, Texture*);
				int(__thiscall* GetLockedTexelOffset)(RenderDevice* _this, Texture*, int, int, int, int);
				bool(__thiscall* LoadVertexShader)(RenderDevice* _this, VertexShader*, DWORD* pFunction, int, int);
				void(__thiscall* UnloadVertexShader)(RenderDevice* _this, VertexShader*);
				bool(__thiscall* LoadPixelShader)(RenderDevice* _this, PixelShader*, DWORD* pFunction, int, int);
				void(__thiscall* UnloadPixelShader)(RenderDevice* _this, PixelShader*);
				bool(__thiscall* CopyTextureFromBackBuffer)(RenderDevice* _this, Texture*);
				bool(__thiscall* CopyTextureFromDepthStencilSurface)(RenderDevice* _this, Texture*);
				bool(__thiscall* CopyTextureFromRenderTarget)(RenderDevice* _this, Texture*, Texture*);
				bool(__thiscall* CopyTextureFromTexture)(RenderDevice* _this, Texture*, Texture*);
				bool(__thiscall* CreateDepthStencilSurface)(RenderDevice* _this, Texture*, int, int, int, int*, int*);
				bool(__thiscall* CreateRenderTarget)(RenderDevice* _this, Texture*, int, int, int, int*, int*);
				void(__thiscall* SaveTextureToFile)(RenderDevice* _this, const std::string*, Texture*, int);
				void(__thiscall* ResetRenderStates)(RenderDevice* _this);
			}* __vftable /*VFT*/;
			int m_ShaderLockCounter;
			Magic::RenderCore::ResourceProfiler* m_pResourceProfiler;
		};

		struct RenderTargetManager
		{
			RenderDevice* m_pRenderDevice;
			std::map<Magic::RenderCore::Texture*, LPDIRECT3DSURFACE9, std::less<Magic::RenderCore::Texture*>, std::allocator<std::pair<Magic::RenderCore::Texture* const, LPDIRECT3DSURFACE9>>> m_RenderTargetMap;
			std::map<Magic::RenderCore::Texture*, LPDIRECT3DSURFACE9, std::less<Magic::RenderCore::Texture*>, std::allocator<std::pair<Magic::RenderCore::Texture* const, LPDIRECT3DSURFACE9>>> m_DepthStencilMap;
			Magic::RenderCore::MultiRenderTargets_t m_CurrentMRT;
			Magic::RenderCore::MultiRenderTargets_t m_LastMRT;
		};

		struct RenderDevice_DX9 : RenderDevice
		{
			struct RenderResCache_t
			{
				LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;
				LPDIRECT3DVERTEXBUFFER9 m_VertexBufferArray[4];
				LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
				LPDIRECT3DTEXTURE9 m_VSTextureArray[4];
				LPDIRECT3DTEXTURE9 m_PSTextureArray[16];
				LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
				LPDIRECT3DPIXELSHADER9 m_pPixelShader;
			};

			_DWORD field_C;
			int m_VideoMemorySize;
			int m_ShaderModel;
			RenderDeviceInitInfo_t m_InitInfo;
			DX9_DeviceCreator_Base* m_pDeviceCreator;
			LPDIRECT3DDEVICE9 m_pD3DDevice;
			std::map<Magic::RenderCore::Texture*, LPDIRECT3DSURFACE9, std::less<Magic::RenderCore::Texture*>, std::allocator<std::pair<Magic::RenderCore::Texture* const, LPDIRECT3DSURFACE9>>> m_pLockedSurfaceMap;
			ShaderGroup* m_pShaderGroup;
			RenderDevice_DX9::RenderResCache_t m_RenderResCache;
			bool m_IsUseResCache;
			bool m_IsLockTail;
			std::map<Magic::RenderCore::IndexBuffer*, LPDIRECT3DVERTEXBUFFER9, std::less<Magic::RenderCore::IndexBuffer*>, std::allocator<std::pair<Magic::RenderCore::IndexBuffer* const, LPDIRECT3DVERTEXBUFFER9>>> m_IndexStreamSourceMap;
			Texture* m_pBackBufferScreenShotTex;
			RenderTargetManager* m_pRenderTargetManager;
		};

		ThiscallFunctionPointer(void, Present, (RenderDevice_DX9* _this), 0x867AE0);
		ThiscallFunctionPointer(void, EndScene, (RenderDevice_DX9* _this), 0x867B00);
		ThiscallFunctionPointer(void, BeginScene, (RenderDevice_DX9* _this), 0x867B10);
		ThiscallFunctionPointer(void, ClearFrameBuffer, (RenderDevice_DX9* _this, unsigned int, D3DCOLOR* Color, float Z, unsigned int Stencil), 0x867B20);

		DataPointer(RenderDevice_DX9*, _g_pRenderDevice, 0x1A55764); // Pointer to the RenderDevice for internal usage
	}
}

struct SOCController
{
	enum ControllerButton : __int32
	{
		BTN_START = 0x0,
		BTN_UP = 0x1,
		BTN_DOWN = 0x2,
		BTN_LEFT = 0x3,
		BTN_RIGHT = 0x4,
		BTN_A = 0x5,
		BTN_B = 0x6,
		BTN_X = 0x7,
		BTN_Y = 0x8,
		BTN_LS = 0x9,
		BTN_RS = 0xA,
		AXIS_LX = 0xB,
		AXIS_LY = 0xC,
		AXIS_RX = 0xD,
		AXIS_RY = 0xE,
	};

	struct vtbl
	{
		void(__thiscall* Destructor)(SOCController* _this);
		void(__thiscall* Update)(SOCController* _this);
		bool(__thiscall* IsPress)(SOCController* _this, SOCController::ControllerButton);
		bool(__thiscall* IsDown)(SOCController* _this, SOCController::ControllerButton);
		float(__thiscall* GetValue)(SOCController* _this, SOCController::ControllerButton);
		bool(__thiscall* VibStart)(SOCController* _this, int, int);
		bool(__thiscall* VibStop)(SOCController* _this);
		bool(__thiscall* Vib)(SOCController* _this, int, int, long double);
		bool(__thiscall* IsValid)(SOCController* _this);
	}* __vftable;

	int field_4;
	Magic::INPUT::Input* m_pInput;
	int m_ControllerIndex;
	int field_10;
	int field_14;
	double m_VibStartTime;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
};

struct SOCInput
{
	void* thing;
	int m_ActiveIndex;
	SOCController* m_pController[8];
	Magic::INPUT::Input* m_pInput;
	PDS_PERIPHERAL* m_Peri;
	int m_PeriNum;
	PDS_KEYBOARD* m_Keyboard;
	int m_KeyboardNum;
};

struct SOCRender
{
	char gap_0[20];
	int field_14;
	int field_18;
	int FogMode;
	float FogStart;
	float FogEnd;
	NJS_ARGB FogColor;
	char gap_28[752];
	NJS_MATRIX Matrix1;
	NJS_MATRIX Matrix2;
	NJS_MATRIX Matrix3;
	NJS_MATRIX TextureMatrices[4];
	char gap_36C[256];
	int field_5E8;
	int field_5EC;
	int field_5F0;
	int field_5F4;
	int field_5F8;
	int field_5FC;
	int field_600;
	int field_604;
	D3DVIEWPORT9 m_ViewPort;
	Magic::RenderCore::SamplerState_t m_SamplerState;
	int field_63C;
	char gap_640[4];
	BYTE ReloadShaders;
	LPVOID pShaderData;
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
	Magic::RenderCore::RenderDeviceInitInfo_t m_InitInfo;
};

DataPointer(Magic::RenderCore::DX9_VideoDevice_PC*, g_pVideoDevice, 0x1A556EC);
DataPointer(Magic::RenderCore::RenderDevice_DX9*, g_pRenderDevice, 0x1A557C0); // Pointer to the RenderDevice for external usage
DataPointer(SOCRender*, g_pSOCRender, 0x1A557BC);
DataPointer(Magic::RenderCore::OnDeviceResetFunc*, DeviceResetFunc, 0x174F81C);
DataPointer(Magic::RenderCore::OnDeviceLostFunc*, DeviceLostFunc, 0x174F820);

StdcallFunctionPointer(SOCController*, GetJoystick, (SOCInput*, int pnum), 0x4256A0);
DataPointer(SOCInput*, g_pSOCInput, 0x1AF19E4); // Singleton