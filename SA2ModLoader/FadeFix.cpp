#include "stdafx.h"
#include "SA2ModLoader.h"
#include "FadeFix.h"

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

void InitScreenFadeFix()
{
	WriteJump((void*)ScreenFadePtr, ScreenFade_r);
}