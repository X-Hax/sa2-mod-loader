#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <deque>
#include "SA2ModLoader.h"
#include "magic.h"
#include "DebugText.h"

VoidFunc(sub_429070, 0x429070);
VoidFunc(sub_4293B0, 0x4293B0);
VoidFunc(sub_44C260, 0x44C260);

namespace debug_text
{
	bool enabled = false;

	struct DebugStringInfo
	{
		__int16 column;
		__int16 row;
		int fontsize;
		int color;
		const char* text;
	};

	struct message
	{
		std::string text;
		uint32_t time;
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

	extern "C" { __declspec(dllexport) IDirect3DVertexShader9* uiShader; }

	static IDirect3DVertexDeclaration9* declaration;
	static IDirect3DVertexDeclaration9* njDrawPolygonDeclaration;
	static IDirect3DTexture9* texture;
	static D3DVIEWPORT9 viewport;
	static FVFStruct_K GiantVertexBuffer_ptr[4];

	static DebugStringInfo* DebugMessages;
	static int DebugMessageMax;
	static int DebugTextBufferMax;
	static int DebugMessageCount = 1;
	static float DebugFontSize = 12.0f;
	static int DebugFontColor = 0xFFBFBFBF;
	static char* DebugTextBuffer;
	static int DebugTextBufferLength;
	static int RenderColor;
	static std::deque<message> msgqueue;

	static const D3DVERTEXELEMENT9 testDeclaration[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,   0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,      0},
		{0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0},
		D3DDECL_END()
	};

	static const D3DVERTEXELEMENT9 drawPolygonDeclaration[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,   0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,      0},
		D3DDECL_END()
	};

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

	void __cdecl Direct3D_DrawQuad(NJS_QUAD_TEXTURE_EX* quad)
	{
		NJS_QUAD_TEXTURE_EX* q; // esi
		float v3; // ST1C_4
		float zValue; // st7 (originally double)
		float v5; // st6 (originally double)
		float v6; // st5 (originally double)
		FVFStruct_K* v7; // eax
		NJS_COLOR v8; // ecx
		float x_halfoffset; // [esp+14h] [ebp+4h]

		q = quad;
		x_halfoffset = quad->x + 0.5f;
		v3 = q->y + 0.5f;
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

		g_pRenderDevice->m_pD3DDevice->SetVertexDeclaration(declaration);
		g_pRenderDevice->m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &GiantVertexBuffer_ptr[0], 24);

		g_pRenderDevice->m_pD3DDevice->SetStreamSource(0, nullptr, 0, 0);
	}

	void __cdecl DrawRectPoints(NJS_POINT2* points, float scale)
	{
		float widthmaybe; // st7 (originally double)
		Float base_x; // ecx
		Float v4; // edx
		float v5; // st7 (originally double)
		Float v6; // ecx
		float v7; // st7 (originally double)
		float v8; // st7 (originally double)
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

	void DrawDebugLetter(unsigned __int8 c, __int16 x, __int16 y, __int16 size)
	{
		NJS_POINT2 rect[4]; // [esp+4h] [ebp-20h]

		const float char_size = 0.0625f;

		float u = (float)(c & 0xF) * char_size + 0.001f;
		float v = (float)(c >> 4) * char_size + 0.001f;

		rect[0].x = x;
		rect[0].y = y;
		rect[1].x = (float)(size + x);
		rect[1].y = (float)(size + y);
		rect[2].x = u;
		rect[2].y = v;
		rect[3].x = u + char_size;
		rect[3].y = v + char_size;

		DrawRectPoints(rect, 1.0f);
	}

	void SetFontSize(float size)
	{
		DebugFontSize = size;
	}

	void SetFontColor(int color)
	{
		DebugFontColor = color;
	}

	void QueueDebugMessage(DebugStringInfo* a1)
	{
		char* v1; // eax
		unsigned int v2; // eax
		unsigned int v3; // ebx
		int v5; // edi
		DebugStringInfo* v6; // eax

		if (a1)
		{
			v1 = (char*)a1->text;
			if (v1)
			{
				v2 = strlen(v1);
				v3 = v2;
				if (DebugMessageCount < DebugMessageMax)
				{
					v5 = DebugTextBufferLength;
					if (v2 < (unsigned int)(DebugTextBufferMax - DebugTextBufferLength))
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

	void DisplayString(int loc, const char* str)
	{
		signed __int16* locshort = (__int16*)&loc;
		DebugStringInfo info;
		info.column = (short)(locshort[1] * DebugFontSize);
		info.row = (short)(locshort[0] * DebugFontSize);
		info.fontsize = (int)DebugFontSize;
		info.text = str;
		info.color = DebugFontColor;
		QueueDebugMessage(&info);
	}

	void DisplayNumber(int loc, int value, int numdigits)
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
		signed __int16* locshort = (__int16*)&loc;
		v4.column = (short)(locshort[1] * DebugFontSize);
		v4.row = (short)(locshort[0] * DebugFontSize);
		v4.fontsize = (int)DebugFontSize;
		v4.color = DebugFontColor;
		v4.text = Dest;
		QueueDebugMessage(&v4);
	}

	void DisplayStringFormatted(int loc, const char* Format, ...)
	{
		va_list ap;
		va_start(ap, Format);
		int result = vsnprintf(nullptr, 0, Format, ap) + 1;
		va_end(ap);
		char* buf = new char[result + 1];
		va_start(ap, Format);
		result = vsnprintf(buf, result + 1, Format, ap);
		va_end(ap);

		DisplayString(loc, buf);
	}

	void njDrawDebugInfo(DebugStringInfo* info)
	{
		// Backup important states
		IDirect3DBaseTexture9* backupTexture;
		IDirect3DVertexShader9* backupVertShader;
		IDirect3DPixelShader9* backupPixelShader;
		IDirect3DVertexDeclaration9* backupDeclaration;
		float backupTev[4] = { 0,0,0,0 };
		g_pRenderDevice->m_pD3DDevice->GetTexture(0, &backupTexture);
		g_pRenderDevice->m_pD3DDevice->GetVertexShader(&backupVertShader);
		g_pRenderDevice->m_pD3DDevice->GetPixelShader(&backupPixelShader);
		g_pRenderDevice->m_pD3DDevice->GetPixelShaderConstantF(0, backupTev, 1);
		g_pRenderDevice->m_pD3DDevice->GetVertexDeclaration(&backupDeclaration);

		// Enable point filtering
		g_pRenderDevice->m_pD3DDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MINFILTER, D3DTEXF_POINT);
		g_pRenderDevice->m_pD3DDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		g_pRenderDevice->m_pD3DDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		// Backup last shader ID
		int prebackup = CurrentShaderID;

		// Init 2D stuff, sets UI shader
		sub_429070();
		// We overwrite the UI vertex shader with ours, to scale the text correctly
		g_pRenderDevice->m_pD3DDevice->SetVertexShader(uiShader);

		// Transparent blending
		sub_4293B0();

		// Set texture
		g_pRenderDevice->m_pD3DDevice->SetTexture(0, texture);

		// Set TevMode for UI pixel shader, if its not 0 the text doesn't display correctly
		float tevmode = 0;
		g_pRenderDevice->m_pD3DDevice->SetPixelShaderConstantF(0, &tevmode, 1);

		const char* str = info->text;
		int strLength = strlen(info->text);
		for (int i = 0; i < strLength; i++)
		{
			RenderColor = info->color;
			DrawDebugLetter(*str, info->column, info->row, info->fontsize);
			info->column += info->fontsize;
			str++;
		}

		// Restore texture
		g_pRenderDevice->m_pD3DDevice->SetTexture(0, backupTexture);

		// Restore linear filtering
		g_pRenderDevice->m_pD3DDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pRenderDevice->m_pD3DDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_pRenderDevice->m_pD3DDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		// Restore shader, tevmode, and vertex declaration
		SetShaders(prebackup); // We still run this, so that the CurrentShaderID gets reset back to the last thing, even if the actual shader gets overwritten
		g_pRenderDevice->m_pD3DDevice->SetVertexShader(backupVertShader);
		g_pRenderDevice->m_pD3DDevice->SetPixelShader(backupPixelShader);
		g_pRenderDevice->m_pD3DDevice->SetPixelShaderConstantF(0, backupTev, 1);
		g_pRenderDevice->m_pD3DDevice->SetVertexDeclaration(backupDeclaration);
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

	void DisplayGameDebug(const char* buf)
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

	void PrintDebugMessages()
	{
		const int numrows = (int)(VerticalResolution / (int)DebugFontSize);
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

			SetFontColor((int)c);
			DisplayString(pos++, (char*)iter->text.c_str());

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

	void __cdecl njDrawPolygon(NJS_POLYGON_VTX* polygon, Int count, Int trans)
	{
		Int primitives; // ebx
		FVFStruct_J* v4; // esi
		NJS_POLYGON_VTX* v5; // edi
		float v6; // st7 (originally double)
		float v7; // st6 (originally double)
		float v8; // st6 (originally double)

		//backup last shader
		int prebackup = *(int*)0x01A5579C;

		SetShaders(0);
		g_pRenderDevice->m_pD3DDevice->SetVertexShader(uiShader);

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
					v7 = v5->x + 0.5f;
					++v5;
					++v4;
					--primitives;
					v4[-1].x = v7;
					v8 = v5[-1].y;
					v4[-1].y = v8 + 0.5f;
					v4[-1].z = v6;
					v4[-1].diffuse = v5[-1].col;

					v4[-1].x *= v6;
					v4[-1].y *= v6;

				} while (primitives);
				primitives = count;
			}

			g_pRenderDevice->m_pD3DDevice->SetTexture(0, nullptr);
			g_pRenderDevice->m_pD3DDevice->SetVertexDeclaration(njDrawPolygonDeclaration);
			g_pRenderDevice->m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, primitives - 2, &GiantVertexBuffer_ptr[0], 16);

			g_pRenderDevice->m_pD3DDevice->SetStreamSource(0, nullptr, 0, 0);
		}

		//Direct3D_TextureFilterLinear();
		SetShaders(prebackup);
	}

	void sub_759AA0(int a1, int a2, int a3, int a4, int a5)
	{
		int v5; // esi
		float v6; // st7 (originally double)
		float v7; // st6 (originally double)
		float v8; // st5 (originally double)
		float v9; // st5 (originally double)
		NJS_POLYGON_VTX a2a[4]; // [esp+4h] [ebp-40h]
		Float v11; // [esp+48h] [ebp+4h]
		int v12; // [esp+50h] [ebp+Ch]
		Float v13; // [esp+50h] [ebp+Ch]

		v5 = a3;
		if (!a3)
		{
			v5 = 1;
		}
		v6 = (float)(12 * a1);
		a2a[0].x = v6;
		v7 = (float)(12 * a2 + 1);
		a2a[0].y = v7;
		a2a[1].x = v6;
		a2a[0].z = 2.0;
		v8 = (float)(12 * a2 + 11);
		v12 = v5 + 12 * a1;
		a2a[0].col = a4;
		a2a[1].z = 2.0;
		v11 = v8;
		a2a[1].col = a4;
		a2a[1].y = v8;
		v9 = (float)(unsigned int)v12;
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

	void PreEndSceneHook()
	{
		sub_44C260();         //endscene sub
		PrintDebugMessages();
		DrawDebugText();
	}

	void Initialize()
	{
		DebugMessages = (DebugStringInfo*)malloc(0x1000);
		DebugTextBuffer = (char*)&DebugMessages[128];
		DebugMessageMax = 128;
		DebugTextBufferMax = 2048;
		DebugMessageCount = 0;
		DebugTextBufferLength = 0;
		SetFontColor(0xFFBFBFBF);
		SetFontSize(12.0f);

		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = 800;
		viewport.Height = 600;
		viewport.MinZ = -100.0f;
		viewport.MaxZ = 100.0f;

		g_pRenderDevice->m_pD3DDevice->CreateVertexDeclaration(testDeclaration, &declaration);
		g_pRenderDevice->m_pD3DDevice->CreateVertexDeclaration(drawPolygonDeclaration, &njDrawPolygonDeclaration);

		LPD3DXBUFFER pUIShaderBuffer;

		if (FAILED(D3DXCompileShaderFromFileA("mods\\DebugTextShader.hlsl", nullptr, nullptr, "main", "vs_3_0", 0, &pUIShaderBuffer, nullptr, nullptr)))
		{
			return;
		}

		if (FAILED(g_pRenderDevice->m_pD3DDevice->CreateVertexShader((DWORD*)pUIShaderBuffer->GetBufferPointer(), &uiShader)))
		{
			return;
		}

		if (FAILED(D3DXCreateTextureFromFileA(g_pRenderDevice->m_pD3DDevice, "mods\\DebugFontTexture.dds", &texture)))
		{
			return;
		}

		WriteCall((void*)0x00433FF7, PreEndSceneHook);
	}
}