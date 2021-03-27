#ifndef SA2MODLOADER_H
#define SA2MODLOADER_H

#if !defined(_M_IX86) && !defined(__i386__)
#error Mods must be built targeting 32-bit x86, change your settings.
#endif

#include "MemAccess.h"
#include "SA2ModInfo.h"
#include "SA2Enums.h"
#include "SA2Structs.h"
#include "SA2Variables.h"
#include "SA2Functions.h"

static inline void ResizeTextureList(NJS_TEXLIST* texlist, Uint32 count)
{
	texlist->textures = new NJS_TEXNAME[count]{};
	texlist->nbTexture = count;
}

static inline void ResizeTextureList(NJS_TEXLIST* texlist, NJS_TEXNAME* textures, Uint32 count)
{
	texlist->textures = textures;
	texlist->nbTexture = count;
}

template <Uint32 N>
static inline void ResizeTextureList(NJS_TEXLIST* texlist, NJS_TEXNAME(&textures)[N])
{
	ResizeTextureList(texlist, textures, N);
}

static inline void njPushMatrix(NJS_MATRIX_PTR m)
{
	if (_nj_current_matrix_ptr_ + 12 <= *(float**)0x2670588)
	{
		if (!m)
			m = _nj_current_matrix_ptr_;
		float *v5 = _nj_current_matrix_ptr_ + 12;
		memcpy(v5, m, 0x30u);
		_nj_current_matrix_ptr_ = v5;
	}
}

static inline void njPopMatrix(int n)
{
	for (int i = 0; i < n; i++)
		if (_nj_current_matrix_ptr_ - 12 >= *(float**)0x267053C)
			_nj_current_matrix_ptr_ -= 12;
		else
			break;
}
#endif