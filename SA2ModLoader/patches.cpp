#include "stdafx.h"
#include "SA2ModLoader.h"
#include "UsercallFunctionHandler.h"
#include "patches.h"
#include "config.h"

#pragma region Car Fix
static const void* const loc_5E3D90 = (void*)0x5E3D90;
static inline BOOL sub_5E3D90(LoopHead* path, int a2, float* a3, float a4, NJS_POINT3* out_pos)
{
	int value;
	__asm
	{
		push[out_pos]
		push[a4]
		push[a3]
		push[a2]
		mov eax, [path]
		call loc_5E3D90
		mov value, eax
		add esp, 16
	}
	return value;
}

// Add additional safety checks when calling sub_5E3D90 to fix NaN car positions
BOOL CalcCarPath_r(int a1, LoopHead* path, float* a3, float* a4, float a5, NJS_POINT3* out_pos, Angle3* out_ang)
{
	NJS_POINT3 out1, out2;

	if (sub_5E3D90(path, a1, a3, a5, &out1) ||
		sub_5E3D90(path, a1, a3, a4[15], &out1) ||
		sub_5E3D90(path, a1, a3, a4[16], &out2)) {
		return TRUE;
	}

	NJS_POINT3 vec;
	vec.x = out1.x - out2.x;
	vec.y = out1.y - out2.y;
	vec.z = out1.z - out2.z;

	float dist = vec.x * vec.x + vec.z * vec.z;

	if (dist <= 0.0f)
		dist = 0.0f;

	out_ang->x = NJM_RAD_ANG(atan2f(-vec.y, sqrtf(dist)));
	out_ang->y = NJM_RAD_ANG(atan2f(vec.x, vec.z));
	out_ang->z = 0;

	vec.x = vec.x * 0.5f;
	vec.y = vec.y * 0.5f;
	vec.z = vec.z * 0.5f;

	out_pos->x = out2.x + vec.x;
	out_pos->y = out2.y + vec.y;
	out_pos->z = out2.z + vec.z;

	return FALSE;
}
#pragma endregion

#pragma region ScreenFade
int ScreenFade_ri(int targetAlpha)
{
	signed int increment = 6; // ecx

	if (*(char*)0x174AFCF != 2 || CurrentObjectSub)
		increment = 5;
	if (ScreenFadeARGB.argb.a == targetAlpha)
		return 1;
	else if (ScreenFadeARGB.argb.a > targetAlpha)
	{
		if (ScreenFadeARGB.argb.a - increment <= targetAlpha)
			ScreenFadeARGB.argb.a = targetAlpha;
		else
			ScreenFadeARGB.argb.a -= increment;
	}
	else // (ScreenFadeARGB.argb.a < targetAlpha)
	{
		if (ScreenFadeARGB.argb.a + increment >= targetAlpha)
			ScreenFadeARGB.argb.a = targetAlpha;
		else
			ScreenFadeARGB.argb.a += increment;
	}
	return 0;
}

static void __declspec(naked) ScreenFade_r()
{
	__asm
	{
		push edx // targetAlpha

		// Call your __cdecl function here:
		call ScreenFade_ri

		pop edx // targetAlpha
		retn
	}
}
#pragma endregion

#pragma region Particles
struct PosUVColorVert {
	NJS_VECTOR Pos;
	Uint32 color;
	Float u, v;
};

struct PosUVNormalColorVert {
	NJS_VECTOR Pos, Normal;
	Uint32 color;
	Float u, v;
};

VoidFunc(GXEnd, 0x0041C070);

DataPointer(int, GX_vtxcount, 0x01933F04);
DataPointer(void*, VertexDeclarationInfoInstance, 0x0174F7E8);
DataPointer(void*, PosUVColor, 0x0174F7FC);
DataPointer(void*, PosNormalUVColor, 0x174F814);
DataPointer(char*, VertexBuffer, 0x01933EF8);
DataPointer(int, VertexBufferStart, 0x01933F08);
DataPointer(int, VertexBufferOffset, 0x01933F0C);

void ParticleGXEnd()
{
	// If the vertex buffer does not contain normals
	if (VertexDeclarationInfoInstance == PosUVColor)
	{
		// Use vertex declaration with normals
		VertexDeclarationInfoInstance = PosNormalUVColor;

		// Remake the vertex buffer
		PosUVColorVert origvert[4];
		PosUVNormalColorVert* newvert = (PosUVNormalColorVert*)(VertexBuffer + VertexBufferStart);

		memcpy(origvert, VertexBuffer + VertexBufferStart, sizeof(PosUVColorVert) * GX_vtxcount);

		for (int i = 0; i < GX_vtxcount; i++) {
			newvert[i].Pos = origvert[i].Pos;
			newvert[i].Normal = { 0.0f, 1.0f, 0.0f };
			newvert[i].u = origvert[i].u;
			newvert[i].v = origvert[i].v;
			newvert[i].color = origvert[i].color;
		}

		// Set new vertex buffer offset
		VertexBufferOffset = VertexBufferStart + sizeof(PosNormalUVColor) * GX_vtxcount;

		// Send the new buffer
		GXEnd();

		// Revert vertex declaration changes
		VertexDeclarationInfoInstance = PosUVColor;
	}
	else
	{
		GXEnd();
	}
}
#pragma endregion

void SyncLoad(void (*a1)(void*), void* a2)
{
	a1(a2);
}

static void* syMalloc(int n)
{
	uint32_t* p_ = (uint32_t*) MemoryManager->Allocate(n + 4, nullptr, 0);

	*p_ = 0x12345678;

	return (void*) (p_ + 1);
}

DataPointer(void*, _nj_vertex_buf_    , 0x025EFE48);
DataPointer(int  , _nj_vertex_buf_num_, 0x025EFE4C);

void ApplyPatches(LoaderSettings* loaderSettings)
{
	// Expand chunk model vertex buffer from 8192 to 32768 verts
	if (loaderSettings->ExtendVertexBuffer || IsGamePatchEnabled("ExtendVertexBuffer"))
	{
		/*
		*	Ninja bases the vertex number assuming 64 bytes per-vertex. Battle only uses 32 bytes
		*	per-vertex, so we set the number to half as many as we actually want. In this case,
		*	16k to get 32k vertex entries of space.
		*/
		_nj_vertex_buf_		= syMalloc( 64 * 0x4000 ); // sizeof(NJS_VERTEX_BUF) * 16'384
		_nj_vertex_buf_num_	= 0x4000;
	}

	// Fix env map condition bug in chDrawCnk
	if (loaderSettings->EnvMapFix || IsGamePatchEnabled("EnvMapFix"))
		WriteData<6>((char*)0x0056DE7D, (char)0x90);

	if (loaderSettings->SyncLoad || IsGamePatchEnabled("SyncLoad"))
	{
		WriteJump((void*)0x428470, SyncLoad);
		WriteJump((void*)0x428740, SyncLoad);
	}

	// Fix screen flickering during victory pose. 
	if (loaderSettings->ScreenFadeFix || IsGamePatchEnabled("ScreenFadeFix"))
		WriteJump((void*)ScreenFadePtr, ScreenFade_r);

	// Fix City Escape car bug on intel iGPUs caused by NaN float position
	if (loaderSettings->CECarFix || IsGamePatchEnabled("CECarFix"))
		GenerateUsercallHook(CalcCarPath_r, rEAX, 0x5DE0B0, rECX, rEDX, rEAX, stack4, stack4, stack4, stack4);
	
	// Fix particle draw calls missing normals
	if (loaderSettings->ParticlesFix || IsGamePatchEnabled("ParticlesFix"))
	{
		WriteJump((void*)0x4915E9, ParticleGXEnd);
		WriteCall((void*)0x491B90, ParticleGXEnd);
		WriteCall((void*)0x4917EE, ParticleGXEnd);
	}
}
