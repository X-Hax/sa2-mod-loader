#pragma once

#include <d3d9.h>
#include <dinput.h>
#include <stdint.h>
#include <XInput.h>
#include <map>
#include <vector>
#include <list>
#include "MemAccess.h"

namespace cppold
{
	namespace std
	{
		struct _Container_base0 {};

		template <class _Elem, class _Alloc = ::std::allocator<_Elem>>
		struct _String_val : _Container_base0
		{
			union _Bxty
			{
				_Elem _Buf[16];
				_Elem* _Ptr;
				_Elem _Alias[16];
			} _Bx;
			unsigned int _Mysize;
			unsigned int _Myres;
			_Alloc _Alval;
		};

		template <class _Elem, class _Traits = ::std::char_traits<_Elem>, class _Alloc = ::std::allocator<_Elem>>
		struct basic_string : _String_val<_Elem, _Alloc>
		{
		};

		typedef basic_string<char> string;
		typedef basic_string<wchar_t> wstring;
	}
}

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

		enum RESOURCE_POOL : __int32
		{
			RES_POOL_VIDEO,
			RES_POOL_COMBINED,
			RES_POOL_MEMORY,
			RES_POOL_MAX
		};

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
			std::string m_Name;
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
			std::string m_Name;
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
				void(__thiscall* Destructor)(Resource* _this, int a2);
				int(__thiscall* GetSize)(Resource* _this);
				bool(__thiscall* IsLoaded)(Resource* _this);
			}* __vftable;
			int m_ResType;
			int m_ResPool;
			void* m_pUserData;
			unsigned int m_ResMask;
			cppold::std::string m_Name;
			cppold::std::string m_Path;
		};

		struct VertexBuffer : Resource
		{
			enum PrimitiveType : __int32
			{
				PT_POINT_LIST,
				PT_LINE_LIST,
				PT_LINE_STRIP,
				PT_TRIANGLE_LIST,
				PT_TRIANGLE_STRIP,
				PT_TRIANGLE_FAN,
				PT_MAX
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
				INDEX_SIZE_16,
				INDEX_SIZE_32
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
			VertexShader* pVertexShader, m_pVS;
			PixelShader* m_pPS;
		};

		struct Material : Resource
		{
			int m_MaterialState;
			int m_AlphaRef;
			ShaderParamData m_ShaderParamData[2];
			VertexShader* pVertexShader, m_pVertexShader;
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
			VertexBuffer* m_pVertexBuf;
			IndexBuffer* m_pIndexBuffer;
			Material* m_pMaterial;
			MATH::Matrix4<float> m_LocalTrans;
		};

		struct Texture : Resource
		{
			enum EType : __int32
			{
				TEX_1D,
				TEX_2D,
				TEX_CUBE,
				TEX_VOLUME,
				TYPE_MAX
			};

			enum EFlag : __int32
			{
				F_LINEAR,
				F_LITTLE_ENDIAN
			};


			enum EUsage : __int32
			{
				U_NORMAL,
				U_BACK_BUFFER,
				U_FRONT_BUFFER,
				U_RENDER_TARGET,
				U_DEPTH_STENCIL,
				U_MAX
			};

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
			std::list<Magic::RenderCore::Resource*> m_pResources[11];
		};

		struct RenderDevice
		{
			enum RenderState : __int32
			{
				RS_FILL_MODE,
				RS_SHADING_MODE,
				RS_CULLING_MODE,
				RS_Z_ENABLE,
				RS_Z_TEST_FUNC,
				RS_Z_WRITE_ENABLE,
				RS_ALPHA_TEST_ENABLE,
				RS_ALPHA_BLENDING_ENABLE,
				RS_ALPHA_BLENDING_SRC_FACTOR,
				RS_ALPHA_BLENDING_DST_FACTOR,
				RS_ALPHA_TEST_FUNC,
				RS_ALPHA_REF,
				RS_STENCIL_TEST_ENABLE,
				RS_STENCIL_TEST_FAIL,
				RS_STENCIL_TEST_ZFAIL,
				RS_STENCIL_TEST_PASS,
				RS_STENCIL_TEST_FUNC,
				RS_STENCIL_REF,
				RS_STENCIL_MASK,
				RS_STENCIL_WRITE_MASK,
				RS_COLORWRITEENABLE,
				RS_MULTISAMPLEANTIALIAS,
				RS_POINTSIZE,
				RS_MAX
			};

			enum ShadingMode : __int32
			{
				SM_FLAT,
				SM_GOURAUD,
				SM_MAX
			};

			enum CullingMode : __int32
			{
				CULL_NONE,
				CULL_CW,
				CULL_CCW,
				CULL_MAX
			};

			enum FillMode : __int32
			{
				FILLMODE_POINT,
				FILLMODE_WIREFRAME,
				FILLMODE_SOLID,
				FILLMODE_MAX
			};

			enum CmpFunc : __int32
			{
				CMP_NEVER,
				CMP_LESS,
				CMP_EQUAL,
				CMP_LESSEQUAL,
				CMP_GREATER,
				CMP_NOTEQUAL,
				CMP_GREATEREQUAL,
				CMP_ALWAYS,
				CMP_MAX
			};

			enum StencilOperator : __int32
			{
				STENCILOP_KEEP,
				STENCILOP_ZERO,
				STENCILOP_REPLACE,
				STENCILOP_INCRSAT,
				STENCILOP_DECRSAT,
				STENCILOP_INVERT,
				STENCILOP_INCR,
				STENCILOP_DECR,
				STENCILOP_MAX
			};

			enum ImageFileFormat : __int32
			{
				IFF_BMP,
				IFF_JPG,
				IFF_TGA,
				IFF_PNG,
				IFF_DDS,
				IFF_PPM,
				IFF_DIB,
				IFF_HDR,
				IFF_PFM,
				IFF_MAX
			};

			enum LockMode : __int32
			{
				LOCK_DEFAULT,
				LOCK_DISCARD,
				LOCK_READONLY,
				LOCK_NOOVERWRITE,
				LOCK_MAX
			};

			enum : __int32
			{
				SAMPLER_COUNT_MAX = 0x8
			};

			enum BlendMode : __int32
			{
				BM_ZERO,
				BM_ONE,
				BM_SRCCOLOR,
				BM_INVSRCCOLOR,
				BM_SRCALPHA,
				BM_INVSRCALPHA,
				BM_DESTALPHA,
				BM_INVDESTALPHA,
				BM_DESTCOLOR,
				BM_INVDESTCOLOR,
				BM_MAX
			};

			enum CubeMapFace : __int32
			{
				CUBEMAP_FACE_POSITIVE_X,
				CUBEMAP_FACE_NEGATIVE_X,
				CUBEMAP_FACE_POSITIVE_Y,
				CUBEMAP_FACE_NEGATIVE_Y,
				CUBEMAP_FACE_POSITIVE_Z,
				CUBEMAP_FACE_NEGATIVE_Z,
				CUBEMAP_FACE_MAX
			};

			enum ColorWriteMask : __int32
			{
				COLOR_MASK_R = 0x1,
				COLOR_MASK_G = 0x2,
				COLOR_MASK_B = 0x4,
				COLOR_MASK_A = 0x8,
				COLOR_MASK_ALL = COLOR_MASK_R | COLOR_MASK_G | COLOR_MASK_B | COLOR_MASK_A
			};

			enum RenderTargetType : __int32
			{
				RENDER_TARGET_0,
				RENDER_TARGET_1,
				RENDER_TARGET_2,
				RENDER_TARGET_3,
				RENDER_TARGET_MAX
			};

			enum FrameBufferType : uint32_t
			{
				FBT_COLOR = 0x1,
				FBT_DEPTH = 0x2,
				FBT_STENCIL = 0x4,
				FBT_MAX = 0xFFFFFFFF
			};

			enum MultiSampleType : __int32
			{
				MULTISAMPLE_NONE,
				MULTISAMPLE_2X,
				MULTISAMPLE_4X,
				MULTISAMPLE_MAX
			};

			struct vtbl
			{
				void(__thiscall* Destructor)(RenderDevice* _this, char flag);
				void(__thiscall* InitializeThing)(RenderDevice* _this);
				void(__thiscall* FreeThing)(RenderDevice* _this);
				void(__thiscall* GetRenderAPIName)(RenderDevice* _this, LPCSTR); // returns "directx_9"
				RenderDevice* (__thiscall* GetRenderAPI)(RenderDevice* _this);
				void(__thiscall* GetTextureCaps)(RenderDevice* _this, LPVOID unknown_struct);
				int(__thiscall* GetVideoMemorySize)(RenderDevice* _this);
				int(__thiscall* GetShaderModel)(RenderDevice* _this);
				void(__thiscall* GetRenderDeviceInitInfo)(RenderDevice* _this, RenderDeviceInitInfo_t* pInfo);
				void(__thiscall* ResetRenderDeviceInitInfo)(RenderDevice* _this, RenderDeviceInitInfo_t* pInfo, OnDeviceLostFunc** lostFunc, OnDeviceResetFunc** resetFunc);
				void(__thiscall* BeginFrame)(RenderDevice* _this);
				void(__thiscall* EndFrame)(RenderDevice* _this);
				void(__thiscall* BeginRenderToTargets)(RenderDevice* _this, MultiRenderTargets_t* MRT);
				void(__thiscall* EndRenderToTargets)(RenderDevice* _this, bool flag);
				void(__thiscall* Present)(RenderDevice* _this);
				void(__thiscall* ClearFrameBuffer)(RenderDevice* _this, unsigned int type, unsigned int Color, float depth, unsigned int stencil);
				void(__thiscall* DrawPrimitive1)(RenderDevice* _this, InputLayout*, VertexBuffer*, int, int, IndexBuffer* pIndexBuf, int, int);
				void(__thiscall* DrawPrimitive2)(RenderDevice* _this, InputLayout*, Mesh*);
				void(__thiscall* DrawPrimitive3)(RenderDevice* _this, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10);
				void(__thiscall* DrawPrimitiveInstance1)(RenderDevice* _this, InputLayout* pInputLayout, VertexBuffer*, int, int, IndexBuffer* pIndexBuf, int, int, VertexBuffer*, int, int, int);
				void(__thiscall* DrawPrimitiveInstance2)(RenderDevice* _this, InputLayout* pInputLayout, Mesh*, VertexBuffer* pVertexBuf);
				void(__thiscall* DrawPrimitiveDirectly)(RenderDevice* _this, InputLayout* pInputLayout, VertexBuffer::PrimitiveType primitiveType, int primitiveCount, void* pVertexData, int vertexSize, int);
				void(__thiscall* GetRenderState)(RenderDevice* _this, RenderState state, DWORD* value);
				void(__thiscall* SetRenderState)(RenderDevice* _this, RenderState state, DWORD value);
				void(__thiscall* GetRenderStateRaw)(RenderDevice* _this, RenderState state, DWORD* rawValue);
				void(__thiscall* SetRenderStateRaw)(RenderDevice* _this, RenderState state, DWORD rawValue);
				MATH::Vector4<float>* (__thiscall* GetViewport)(RenderDevice* _this, MATH::Vector4<float>* result);
				void(__thiscall* SetViewport)(RenderDevice* _this, const MATH::Vector4<float>* pRect);
				void(__thiscall* GetScissorRect)(RenderDevice* _this, const MATH::Vector4<float>* pRect);
				void(__thiscall* SetScissorRect)(RenderDevice* _this, const MATH::Vector4<float>* pRect);
				void(__thiscall* StoreDepthStencilSurface)(RenderDevice* _this, Texture* pTexture);
				void(__thiscall* LoadDepthStencilSurface)(RenderDevice* _this, Texture* pTexture);
				void(__thiscall* UnloadDepthStencilSurface)(RenderDevice* _this, Texture* pTexture);
				void(__thiscall* RestoreDepthStencilSurface)(RenderDevice* _this, Texture* pTexture);
				void(__thiscall* StoreRenderTarget)(RenderDevice* _this, int rtIndex, Texture* pTexture);
				void(__thiscall* LoadRenderTarget)(RenderDevice* _this, int rtIndex, Texture* pTexture);
				void(__thiscall* LoadCubeRenderTarget)(RenderDevice* _this, int rtIndex, Texture* pTexture, int iFace);
				LPVOID Unknown37;
				void(__thiscall* UnloadRenderTarget)(RenderDevice* _this, int rtIndex, Texture* pTexture);
				void(__thiscall* RestoreRenderTarget)(RenderDevice* _this, int rtIndex, Texture* pTexture);
				void(__thiscall* LockShader)(RenderDevice* _this, const ShaderGroup* spShaders);
				void(__thiscall* UnlockShader)(RenderDevice* _this);
				bool(__thiscall* IsShaderLocked)(RenderDevice* _this);
				VertexShader* (__thiscall* GetVertexShader)(RenderDevice* _this);
				void(__thiscall* SetVertexShader)(RenderDevice* _this, VertexShader* pVertexShader);
				void(__thiscall* UnsetVertexShader)(RenderDevice* _this);
				PixelShader* (__thiscall* GetPixelShader)(RenderDevice* _this);
				void(__thiscall* SetPixelShader)(RenderDevice* _this, PixelShader* pPixelShader);
				void(__thiscall* UnsetPixelShader)(RenderDevice* _this);
				ShaderGroup* (__thiscall* GetShader)(RenderDevice* _this, ShaderGroup* result);
				void(__thiscall* SetShader1)(RenderDevice* _this, const ShaderGroup* spShaders);
				void(__thiscall* SetShader2)(RenderDevice* _this, VertexShader* pVertexShader, PixelShader* pPixelShader);
				void(__thiscall* UnsetShader)(RenderDevice* _this);
				void(__thiscall* ShaderSetConstant1)(RenderDevice* _this, ShaderStage* pShaderStage, const char* name, Sampler*, void**, int*);
				void(__thiscall* ShaderSetConstant2)(RenderDevice* _this, ShaderStage* pShaderStage, const char* name, const MATH::Matrix4<float>* value, void**);
				void(__thiscall* ShaderSetConstant3)(RenderDevice* _this, ShaderStage* pShaderStage, const char* name, const MATH::Vector4<float>* value, void**);
				void(__thiscall* ShaderSetConstant4)(RenderDevice* _this, ShaderStage* pShaderStage, const char* name, float, void**);
				void(__thiscall* ShaderSetConstant5)(RenderDevice* _this, ShaderStage* pShaderStage, const char* name, int, void**);
				void(__thiscall* ShaderSetConstant6)(RenderDevice* _this, ShaderStage* pShaderStage, const char* name, bool, void**);
				void(__thiscall* ShaderSetConstant7)(RenderDevice* _this, int, int, Texture* pTexture, const SamplerState_t*);
				void(__thiscall* ShaderSetConstant8)(RenderDevice* _this, int, int, const Magic::MATH::Matrix4<float>*);
				void(__thiscall* ShaderSetConstant9)(RenderDevice* _this, int, int, const Magic::MATH::Vector4<float>*);
				void(__thiscall* ShaderSetConstant10)(RenderDevice* _this, int, int, const float*, int);
				void(__thiscall* ShaderSetConstant11)(RenderDevice* _this, int, int, const int*, int);
				void(__thiscall* ShaderSetConstant12)(RenderDevice* _this, int, int, const bool*, int);
				bool(__thiscall* CheckDeviceLost)(RenderDevice* _this, OnDeviceLostFunc* lostFunc, OnDeviceResetFunc* resetFunc);
				LPVOID Unknown66;
				LPVOID Unknown67;
				void(__thiscall* SaveBackBufferToFile)(RenderDevice* _this, const std::string* name, ImageFileFormat imageFileFormat);
				void(__thiscall* BuildMipmaps)(RenderDevice* _this, Texture* pTexture);
				void(__thiscall* SetVerticalBlankCallback)(RenderDevice* _this, void(__cdecl* pCallback)(void*));
				bool(__thiscall* LoadInputLayout)(RenderDevice* _this, InputLayout* pInputLayout, InputElementDesc_t* elems);
				void(__thiscall* UnloadInputLayout)(RenderDevice* _this, InputLayout* pInputLayout);
				bool(__thiscall* LoadVertexBuffer)(RenderDevice* _this, VertexBuffer*, void* pRawData, int indexNum, int indexSize, RESOURCE_POOL pool);
				void(__thiscall* UnloadVertexBuffer)(RenderDevice* _this, VertexBuffer* pVertexBuf);
				bool(__thiscall* LockVertexBuffer)(RenderDevice* _this, VertexBuffer*, void** ppLockedData, int offset, int size, int lockFlag);
				void(__thiscall* UnlockVertexBuffer)(RenderDevice* _this, VertexBuffer* pVertexBuf);
				bool(__thiscall* LoadIndexBuffer)(RenderDevice* _this, IndexBuffer* pIndexBuf, void* pRawData, int indexNum, int indexSize, RESOURCE_POOL pool);
				void(__thiscall* UnloadIndexBuffer)(RenderDevice* _this, IndexBuffer* pIndexBuf);
				bool(__thiscall* LockIndexBuffer)(RenderDevice* _this, IndexBuffer* pIndexBuf, void** ppLockedData, int offset, int size, int lockFlag);
				void(__thiscall* UnlockIndexBuffer)(RenderDevice* _this, IndexBuffer* pIndexBuf);
				bool(__thiscall* CreateTexture)(RenderDevice* _this, Texture* pTexture, int texType, int* width, int* height, int* depth, int* format, int* mipLevels, RESOURCE_POOL* pool, int* flag);
				bool(__thiscall* LoadTexture)(RenderDevice* _this, Texture* pTexture, int textureType, void* pRawData, int size, int* width, int* height, int* depth, int* format, int* mipLevels, int* flag);
				void(__thiscall* UnloadTexture)(RenderDevice* _this, Texture* pTexture);
				bool(__thiscall* LockTexture)(RenderDevice* _this, Texture* pTexture, int mipmapLevel, void** ppLockedData, int* pPitch, GRect_t*, int lockFlag);
				void(__thiscall* UnlockTexture)(RenderDevice* _this, Texture* pTexture);
				int(__thiscall* GetLockedTexelOffset)(RenderDevice* _this, Texture* pTexture, int x, int y, int pitch, int mipmapLevel);
				bool(__thiscall* LoadVertexShader)(RenderDevice* _this, VertexShader* pVertexShader, void* pRawData, int size, int flag);
				void(__thiscall* UnloadVertexShader)(RenderDevice* _this, VertexShader* pVertexShader);
				bool(__thiscall* LoadPixelShader)(RenderDevice* _this, PixelShader* pPixelShader, void* pRawData, int size, int flag);
				void(__thiscall* UnloadPixelShader)(RenderDevice* _this, PixelShader* pPixelShader);
				bool(__thiscall* CopyTextureFromBackBuffer)(RenderDevice* _this, Texture* pDstTexture);
				bool(__thiscall* CopyTextureFromDepthStencilSurface)(RenderDevice* _this, Texture* pDstTexture);
				bool(__thiscall* CopyTextureFromRenderTarget)(RenderDevice* _this, Texture* pDstTexture, Texture* pSrcTexture);
				bool(__thiscall* CopyTextureFromTexture)(RenderDevice* _this, Texture* pDstTexture, Texture* pSrcTexture);
				bool(__thiscall* CreateDepthStencilSurface)(RenderDevice* _this, Texture* pTexture, int width, int height, int format, int* pool, int* flag);
				bool(__thiscall* CreateRenderTarget)(RenderDevice* _this, Texture* pTexture, int width, int height, int format, int* pool, int* flag);
				void(__thiscall* SaveTextureToFile)(RenderDevice* _this, const std::string* name, Texture* pTexture, ImageFileFormat imageFileFormat);
				void(__thiscall* ResetRenderStates)(RenderDevice* _this);
			}* __vftable /*VFT*/;
			int m_ShaderLockCounter;
			Magic::RenderCore::ResourceProfiler* m_pResourceProfiler;
		};

		struct RenderTargetManager
		{
			RenderDevice* m_pRenderDevice;
			std::map<Magic::RenderCore::Texture*, LPDIRECT3DSURFACE9> m_RenderTargetMap;
			std::map<Magic::RenderCore::Texture*, LPDIRECT3DSURFACE9> m_DepthStencilMap;
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
			std::map<Magic::RenderCore::Texture*, LPDIRECT3DSURFACE9> m_pLockedSurfaceMap;
			ShaderGroup* m_pShaderGroup;
			RenderDevice_DX9::RenderResCache_t m_RenderResCache;
			bool m_IsUseResCache;
			bool m_IsLockTail;
			std::map<Magic::RenderCore::IndexBuffer*, LPDIRECT3DVERTEXBUFFER9> m_IndexStreamSourceMap;
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

ThiscallFunctionPointer(Magic::RenderCore::Texture*, InitTexture, (Magic::RenderCore::Texture* _this), 0x85F690);
