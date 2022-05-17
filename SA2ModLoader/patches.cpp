#include "stdafx.h"
#include "SA2ModLoader.h"
#include "UsercallFunctionHandler.h"
#include "patches.h"

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

void ApplyPatches()
{
	// Expand chunk model vertex buffer from 8192 to 32768 verts
	*(void**)0x25EFE48 = calloc(1, 0x100004);

	// Fix env map condition bug in chDrawCnk
	WriteData<6>((char*)0x0056DE7D, (char)0x90);

	// Fix the screen fade
	WriteJump((void*)ScreenFadePtr, ScreenFade_r);

	// Fix City Escape car bug on intel iGPUs caused by NaN float position
	GenerateUsercallHook(CalcCarPath_r, rEAX, 0x5DE0B0, rECX, rEDX, rEAX, stack4, stack4, stack4, stack4);
}