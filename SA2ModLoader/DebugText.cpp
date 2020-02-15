#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"
#include <stdio.h>
#include <stdarg.h>
#include "SA2ModLoader.h"
#include <string>
#include <deque>
#include <iostream>
#include "DebugText.h"

const int SetShadersPtr = 0x0041B1F0;
void SetShaders(int result)
{
	__asm
	{
		mov eax, result
		call SetShadersPtr
	}
}


VoidFunc(sub_42C170, 0x42C170);
VoidFunc(sub_583C60, 0x583C60);
VoidFunc(sub_429070, 0x429070);
VoidFunc(sub_4293B0, 0x4293B0);
VoidFunc(sub_4292E0, 0x4292E0);
VoidFunc(DoSomethingBlendingMode, 0x0429170);
FunctionPointer(void, __cdecl GXSetViewport, (float arg0, float a2, float a3, float a4, float MinZ, float MaxZ), 0x00420210);

IDirect3DDevice9* device;
ID3DXFont* font = nullptr;
IDirect3DVertexDeclaration9* declaration;
IDirect3DVertexDeclaration9* njDrawPolygonDeclaration;
IDirect3DTexture9* texture;
D3DVIEWPORT9 viewport;
struct  Renderer
{
	void* vtable;
	char field_4[52];
	IDirect3DDevice9* pointerToDevice;
	char field_3C[70];

};
DataPointer(Renderer*, dword_1A557C0, 0x1A557C0);
IDirect3DVertexShader9* uiShader;
D3DVERTEXELEMENT9 testDeclaration[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,   0},
	{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,      0},
	{0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0},
	D3DDECL_END()
};

D3DVERTEXELEMENT9 drawPolygonDeclaration[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,   0},
	{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,      0},
	D3DDECL_END()
};

struct FVFStruct_J
{
	float x;
	float y;
	float z;
	Uint32 diffuse;
};

struct FVFStruct_K
{
	FVFStruct_J base;
	float u;
	float v;
};

FVFStruct_K GiantVertexBuffer_ptr[4];
struct __declspec(align(4)) VertexDeclarationInfo
{
	float gap0;
	int field_4;
	float field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	IDirect3DVertexDeclaration9* D3DVertexDeclaration;
	int field_20;
	int StrideSize;
	int field_28;
	int field_2C;
	int field_30;
};
DataPointer(VertexDeclarationInfo*, VertexDeclarationInfoInstance, 0x0174F7E8);

int RenderColor;
void __cdecl Direct3D_DrawQuad(NJS_QUAD_TEXTURE_EX* quad)
{
	NJS_QUAD_TEXTURE_EX* q; // esi
	float base_z; // ST10_4
	float v3; // ST1C_4
	double zValue; // st7
	double v5; // st6
	double v6; // st5
	FVFStruct_K* v7; // eax
	NJS_COLOR v8; // ecx
	double v9; // st4
	float v10; // ST1C_4
	float v11; // ST20_4
	double v12; // st6
	double v13; // st5
	float x_halfoffset; // [esp+14h] [ebp+4h]
	float pointsb; // [esp+14h] [ebp+4h]

	q = quad;
	base_z = quad->z;
	x_halfoffset = quad->x + 0.5;
	v3 = q->y + 0.5;
	zValue = -1.0f;//DoWeirdProjectionThings(base_z);
	v5 = q->u;
	v6 = q->v;
	v7 = (FVFStruct_K*)GiantVertexBuffer_ptr;
	v8.color = RenderColor;

	v7->base.x = x_halfoffset;
	v7->base.y = v3;
	v7->base.z = zValue;
	v7->u = v5;
	v7->v = v6;
	v7->base.diffuse = v8.color;

	v7[1].base.x = (x_halfoffset + q->vx1);
	v7[1].base.y = v3 + q->vy1;
	v7[1].base.z = zValue;
	v7[1].u = v5 + q->vu1;
	v7[1].v = v6 + q->vv1;
	v7[1].base.diffuse = v8.color;

	v7[2].base.x = x_halfoffset + q->vx2;
	v7[2].base.y = v3 + q->vy2;
	v7[2].base.z = zValue;
	v7[2].u = v5 + q->vu2;
	v7[2].v = v6 + q->vv2;
	v7[2].base.diffuse = v8.color;

	v7[3].base.x = (v7[2].base.x + q->vx1);
	v7[3].base.y = v7[2].base.y + q->vy1;
	v7[3].base.z = zValue;
	v7[3].u = v7[2].u + q->vu1;
	v7[3].v = v7[2].v + q->vv1;
	v7[3].base.diffuse = v8.color;

	for (int i = 0; i < 4; i++)
	{
		v7[i].base.x *= zValue; //???????
		v7[i].base.y *= zValue;
	}


	dword_1A557C0->pointerToDevice->SetVertexDeclaration(declaration);
	dword_1A557C0->pointerToDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &GiantVertexBuffer_ptr[0], 24);

	dword_1A557C0->pointerToDevice->SetStreamSource(0, 0, 0, 0);

}
void __cdecl DrawRectPoints(NJS_POINT2* points, float scale)
{
	double widthmaybe; // st7
	Float base_x; // ecx
	Float v4; // edx
	double v5; // st7
	Float v6; // ecx
	double v7; // st7
	double v8; // st7
	NJS_QUAD_TEXTURE_EX _points; // [esp+0h] [ebp-40h]

	widthmaybe = points[1].x - points->x;
	base_x = points->x;
	_points.y = points->y;
	v4 = points[2].x;
	_points.vx1 = widthmaybe;
	_points.x = base_x;
	v5 = points[1].y - points->y;
	_points.u = v4;
	_points.z = scale;
	v6 = points[2].y;
	_points.vy2 = v5;
	v7 = points[3].x - points[2].x;
	_points.vy1 = 0.0;
	_points.vx2 = 0.0;
	_points.v = v6;
	_points.vu1 = v7;
	_points.vv1 = 0.0;
	v8 = points[3].y;
	_points.vu2 = 0.0;
	_points.vv2 = v8 - points[2].y;
	Direct3D_DrawQuad(&_points);
}
void sub_793B20(unsigned __int8 a1, __int16 a2, __int16 a3, __int16 a4)
{
	double v4; // st7
	double v5; // st6
	double v6; // st5
	double v7; // st4
	double v8; // st3
	NJS_POINT2 idk[4]; // [esp+4h] [ebp-20h]

	v4 = (double)(a1 & 0xF) * 0.0625;
	v5 = (double)(a1 >> 4) * 0.0625;
	v6 = (double)a2;
	idk[0].x = v6;
	v7 = (double)a3;
	idk[0].y = v7;
	v8 = (double)a4;
	idk[1].x = v8 + v6;
	idk[1].y = v8 + v7;
	idk[2].x = v4;
	idk[2].y = v5;
	idk[3].x = v4 + 0.0625;
	idk[3].y = v5 + 0.0625;
	DrawRectPoints(idk, 1.0);
}

void __cdecl njDrawPolygon(NJS_POLYGON_VTX* polygon, Int count, Int trans)
{
	Int primitives; // ebx
	FVFStruct_J* v4; // esi
	NJS_POLYGON_VTX* v5; // edi
	double v6; // st7
	double v7; // st6
	double v8; // st6

	//backup last shader
	int prebackup = *(int*)0x01A5579C;

	SetShaders(0);
	dword_1A557C0->pointerToDevice->SetVertexShader(uiShader);

	//init 2d stuff
	sub_429070();

	//transparent blending
	sub_4293B0();

	primitives = count;
	if (count > 2)
	{
		v4 = (FVFStruct_J*)GiantVertexBuffer_ptr;
		if (count)
		{
			v5 = polygon;
			do
			{
				v6 = -1;// DoWeirdProjectionThings(v5->z);
				v7 = v5->x + 0.5;
				++v5;
				++v4;
				--primitives;
				v4[-1].x = v7;
				v8 = v5[-1].y;
				v4[-1].y = v8 + 0.5;
				v4[-1].z = v6;
				v4[-1].diffuse = v5[-1].col;

				v4[-1].x *= v6;
				v4[-1].y *= v6;
				
			} while (primitives);
			primitives = count;
		}
		
		dword_1A557C0->pointerToDevice->SetTexture(0, 0);
		dword_1A557C0->pointerToDevice->SetVertexDeclaration(njDrawPolygonDeclaration);
		dword_1A557C0->pointerToDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, primitives - 2, &GiantVertexBuffer_ptr[0], 16);

		dword_1A557C0->pointerToDevice->SetStreamSource(0, 0, 0, 0);
	}
	//Direct3D_TextureFilterLinear();
	SetShaders(prebackup);
}

struct DebugStringInfo
{
	__int16 column;
	__int16 row;
	int fontsize;
	int color;
	const char* text;
};

DebugStringInfo* DebugMessages;
int DebugMessageMax;
int DebugTextBufferMax;
int DebugMessageCount = 1;
float DebugFontSize = 12;
int DebugFontColor;
char* DebugTextBuffer;
int DebugTextBufferLength;

extern "C"
{
	__declspec(dllexport) void SetDebugFontSize(float size)
	{
		DebugFontSize = size;
	}

	__declspec(dllexport) void SetDebugFontColor(int color)
	{
		DebugFontColor = color;
	}
}

void QueueDebugMessage(DebugStringInfo* a1)
{
	char* v1; // eax
	unsigned int v2; // eax
	unsigned int v3; // ebx
	int v4; // esi
	int v5; // edi
	DebugStringInfo* v6; // eax
	char* v7; // edx

	if (a1)
	{
		v1 = (char*)a1->text;
		if (v1)
		{
			v2 = strlen(v1);
			v4 = DebugMessageCount;
			v3 = v2;
			if (DebugMessageCount < (unsigned int)DebugMessageMax)
			{
				v5 = DebugTextBufferLength;
				if (v2 < DebugTextBufferMax - DebugTextBufferLength)
				{
					v6 = &DebugMessages[DebugMessageCount];
					v6->column = a1->column;
					v6->row = a1->row;
					v6->fontsize = a1->fontsize;
					v6->color = a1->color;
					v6->text = &DebugTextBuffer[v5];
					strcpy((char*)v6->text, a1->text);
					DebugMessageCount++;
					DebugTextBufferLength = v5 + v3 + 1;
				}
			}
		}
	}
}

extern "C" {
	__declspec(dllexport) void DisplayDebugString(Int loc, const char* s)
	{
		signed __int16* locshort = (__int16*)& loc;
		DebugStringInfo info;
		info.column = (locshort[1] * DebugFontSize);
		info.row = (locshort[0] * DebugFontSize);
		info.fontsize = DebugFontSize;
		info.text = s;
		info.color = DebugFontColor;
		QueueDebugMessage(&info);
	}

	__declspec(dllexport) void PrintDebugNumber(signed int position, int value, signed int numdigits)
	{
		char v3; // al
		DebugStringInfo v4; // [esp+4h] [ebp-9Ch]
		char Format[4]; // [esp+14h] [ebp-8Ch]
		char Dest[128]; // [esp+1Ch] [ebp-84h]

		strcpy(Format, "%00d");
		v3 = numdigits;
		if (numdigits > 9)
		{
			v3 = 9;
		}
		Format[2] += v3;
		sprintf(Dest, Format, value);
		signed __int16* locshort = (__int16*)& position;
		v4.column = (locshort[1] * DebugFontSize);
		v4.row = (locshort[0] * DebugFontSize);
		v4.fontsize = DebugFontSize;
		v4.color = DebugFontColor;
		v4.text = Dest;
		QueueDebugMessage(&v4);
	}

	__declspec(dllexport) void DisplayDebugStringFormatted(Int loc, const char* Format, ...)
	{
		va_list ap;
		va_start(ap, Format);
		int result = vsnprintf(nullptr, 0, Format, ap) + 1;
		va_end(ap);
		char* buf = new char[result + 1];
		va_start(ap, Format);
		result = vsnprintf(buf, result + 1, Format, ap);
		va_end(ap);

		DisplayDebugString(loc, buf);
	}

	__declspec(dllexport) void sub_759AA0(int a1, int a2, int a3, int a4, int a5)
	{
		int v5; // esi
		double v6; // st7
		double v7; // st6
		double v8; // st5
		double v9; // st5
		NJS_POLYGON_VTX a2a[4]; // [esp+4h] [ebp-40h]
		Float v11; // [esp+48h] [ebp+4h]
		int v12; // [esp+50h] [ebp+Ch]
		Float v13; // [esp+50h] [ebp+Ch]

		v5 = a3;
		if (!a3)
		{
			v5 = 1;
		}
		v6 = (double)(12 * a1);
		a2a[0].x = v6;
		v7 = (double)(12 * a2 + 1);
		a2a[0].y = v7;
		a2a[1].x = v6;
		a2a[0].z = 2.0;
		v8 = (double)(12 * a2 + 11);
		v12 = v5 + 12 * a1;
		a2a[0].col = a4;
		a2a[1].z = 2.0;
		v11 = v8;
		a2a[1].col = a4;
		a2a[1].y = v8;
		v9 = (double)(unsigned int)v12;
		v13 = v9;
		a2a[2].x = v9;
		a2a[2].y = v7;
		a2a[2].col = a5;
		a2a[3].col = a5;
		a2a[2].z = 2.0;
		a2a[3].x = v13;
		a2a[3].y = v11;
		a2a[3].z = 2.0;
		njDrawPolygon(a2a, 4, 0);
	}
}
void njDrawDebugInfo(DebugStringInfo* info)
{
	IDirect3DBaseTexture9* backupTexture;

	dword_1A557C0->pointerToDevice->GetTexture(0, &backupTexture);

	//MAINMEMORY: choose if you want this - enable point filtering
	//dword_1A557C0->pointerToDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MINFILTER, D3DTEXF_POINT);
	//dword_1A557C0->pointerToDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	//dword_1A557C0->pointerToDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//backup last shader
	int prebackup = *(int*)0x01A5579C;

	//set to UI shader
	SetShaders(0);
	dword_1A557C0->pointerToDevice->SetVertexShader(uiShader);

	//init 2d stuff
	sub_429070();

	//transparent blending
	sub_4293B0();

	//set texture
	dword_1A557C0->pointerToDevice->SetTexture(0, texture);

	const char* str = info->text;
	int strLength = strlen(info->text);
	for (int i = 0; i < strLength; i++)
	{
		RenderColor = info->color;
		sub_793B20(*str, info->column, info->row, info->fontsize);
		info->column += info->fontsize;
		str++;
	}

	//restore values
	dword_1A557C0->pointerToDevice->SetTexture(0, backupTexture);
	//restore linear filtering
	dword_1A557C0->pointerToDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	dword_1A557C0->pointerToDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	dword_1A557C0->pointerToDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SetShaders(prebackup);
}

void DrawDebugText()
{
	if (DebugMessageCount)
	{
		for (int i = 0; i < DebugMessageCount; i++)
			njDrawDebugInfo(&DebugMessages[i]);
	}
	DebugMessageCount = 0;
	DebugTextBufferLength = 0;
}


struct message
{
	std::string text;
	uint32_t time;
};

static std::deque<message> msgqueue;

void PrintDebug_Screen(const char* buf)
{
	message msg = { { buf }, 0 };
	// Remove trailing newlines if present.
	while (!msg.text.empty() &&
		(msg.text[msg.text.size() - 1] == '\n' ||
			msg.text[msg.text.size() - 1] == '\r'))
	{
		msg.text.resize(msg.text.size() - 1);
	}
	msgqueue.push_back(msg);
}
static const uint32_t fadecolors[] = {
	0xF7FFFFFF, 0xEEFFFFFF, 0xE6FFFFFF, 0xDDFFFFFF,
	0xD5FFFFFF, 0xCCFFFFFF, 0xC4FFFFFF, 0xBBFFFFFF,
	0xB3FFFFFF, 0xAAFFFFFF, 0xA2FFFFFF, 0x99FFFFFF,
	0x91FFFFFF, 0x88FFFFFF, 0x80FFFFFF, 0x77FFFFFF,
	0x6FFFFFFF, 0x66FFFFFF, 0x5EFFFFFF, 0x55FFFFFF,
	0x4DFFFFFF, 0x44FFFFFF, 0x3CFFFFFF, 0x33FFFFFF,
	0x2BFFFFFF, 0x22FFFFFF, 0x1AFFFFFF, 0x11FFFFFF,
	0x09FFFFFF, 0
};
void PrintDebugMessages()
{
	const int numrows = (VerticalResolution / (int)DebugFontSize);
	int pos = (int)msgqueue.size() <= numrows - 1 ? numrows - 1 - (msgqueue.size() - 1) : 0;

	if (msgqueue.empty())
	{
		return;
	}

	for (auto iter = msgqueue.begin();
		iter != msgqueue.end(); ++iter)
	{

		int c = -1;

		if (300 - iter->time < LengthOfArray(fadecolors))
		{
			c = fadecolors[LengthOfArray(fadecolors) - (300 - iter->time) - 1];
		}

		SetDebugFontColor((int)c);
		DisplayDebugString(pos++, (char*)iter->text.c_str());

		if (++iter->time >= 300)
		{
			msgqueue.pop_front();

			if (msgqueue.empty())
			{
				break;
			}

			iter = msgqueue.begin();
		}

		if (pos == numrows)
		{
			break;
		}
	}
}

VoidFunc(sub_44C260, 0x44C260);

D3DVIEWPORT9 backupViewport;
void PreEndSceneHook()
{
	sub_44C260();         //endscene sub
	PrintDebugMessages(); //this queues up the PrintDebug shit
	DrawDebugText();      //this draws it
}

void Debug_LoadShader()
{
	LPD3DXBUFFER pUIShaderBuffer;

	D3DXCompileShaderFromFileA("mods\\DebugTextShader.hlsl", 0, 0, "main", "vs_3_0", 0, &pUIShaderBuffer, 0, 0);
	dword_1A557C0->pointerToDevice->CreateVertexShader((DWORD*)pUIShaderBuffer->GetBufferPointer(), &uiShader);
}

void DebugText_Init()
{
	WriteCall((void*)0x00433FF7, PreEndSceneHook);

	DebugMessages = (DebugStringInfo*)malloc(0x1000);
	DebugTextBuffer = (char*)& DebugMessages[128];
	DebugMessageMax = 128;
	DebugTextBufferMax = 2048;
	DebugMessageCount = 0;
	DebugTextBufferLength = 0;
	SetDebugFontSize(12.0f);

	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinZ = -100.0f;
	viewport.MaxZ = 100.0f;

	dword_1A557C0->pointerToDevice->CreateVertexDeclaration(testDeclaration, &declaration);
	dword_1A557C0->pointerToDevice->CreateVertexDeclaration(drawPolygonDeclaration, &njDrawPolygonDeclaration);

	Debug_LoadShader();
	D3DXCreateTextureFromFileA(dword_1A557C0->pointerToDevice, "mods\\DebugFontTexture.dds", &texture);
}
