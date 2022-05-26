#pragma once

#include <d3d9.h>
#include <stdint.h>
#include <map>
#include "MemAccess.h"

namespace Magic
{
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

		struct VideoDevice_vtbl
		{
			void(__cdecl * Destructor)(VideoDevice* _this);
			int(__cdecl* GetDisplayWidth)(VideoDevice* _this);
			int(__cdecl* GetDisplayHeight)(VideoDevice* _this);
			int(__cdecl* GetDisplayFormat)(VideoDevice* _this);
			bool(__cdecl* IsWideScreen)(VideoDevice* _this);
			float(__cdecl* GetRefreshRate)(VideoDevice* _this);
		};

		struct VideoDevice
		{
			VideoDevice_vtbl* __vftable;
		};

		struct DX9_VideoDevice_Base : VideoDevice
		{
			LPDIRECT3D9 m_pD3D;
		};

		struct DX9_VideoDevice_PC : DX9_VideoDevice_Base
		{
			D3DDISPLAYMODE m_VideoMode;
		};

		struct DX9_DeviceCreator_Base_vtbl
		{
			bool(__cdecl* CreateD3DDevice)(DX9_DeviceCreator_Base* _this, const DeviceConfig_t*);
			void(__cdecl* FreeD3DDevice)(DX9_DeviceCreator_Base* _this, char flag);
		};

		struct DX9_DeviceCreator_Base
		{
			DX9_DeviceCreator_Base_vtbl* __vftable;
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

		struct Sampler_vtbl
		{
			void(__thiscall* Destructor)(Sampler* _this);
		};

		struct Sampler : SamplerState_t
		{
			Magic::RenderCore::Sampler_vtbl* __vftable /*VFT*/;
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

		struct Resource_vtbl
		{
			void(__thiscall* Destructor)(Resource* _this);
			int(__thiscall* GetSize)(Resource* _this);
			bool(__thiscall* IsLoaded)(Resource* _this);
		};

		struct Resource
		{
			Resource_vtbl* __vftable;
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

		struct InputLayout_vtbl
		{
			void(__thiscall* Destructor)(InputLayout* _this);
		};

		struct InputLayout
		{
			InputLayout_vtbl* __vftable;
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

		struct RenderDevice_vtbl
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
			MATH::Vector4<float>*(__thiscall* GetViewport)(RenderDevice* _this, MATH::Vector4<float>* result);
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
		};

		struct ResourceProfiler_vtbl
		{
			void(__cdecl * Destructor)(ResourceProfiler* _this);
		};

		struct ResourceProfiler
		{
			ResourceProfiler_vtbl* __vftable /*VFT*/;
			std::list<Magic::RenderCore::Resource*, std::allocator<Magic::RenderCore::Resource*> > m_pResources[11];
		};

		struct RenderDevice
		{
			Magic::RenderCore::RenderDevice_vtbl* __vftable /*VFT*/;
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