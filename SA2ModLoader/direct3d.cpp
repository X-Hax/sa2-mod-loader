#include "stdafx.h"
#include "magic.h"
#include "Events.h"

#include "DebugText.h"

#include <chrono>
#include <thread>

#include "direct3d.h"

// Direct3D utilties and hooks for window stuff and frame limiter.

using namespace std::chrono;

using FrameRatio = duration<double, std::ratio<1, 60>>;

static bool enable_frame_limit = false;
static auto frame_start = system_clock::now();
static duration<double, std::milli> present_time = {};

static const auto frame_ratio = FrameRatio(1);
static const auto frame_portion_ms = duration_cast<milliseconds>(frame_ratio) - milliseconds(1);

static HWND DrawWindow = NULL;

static void RunFrameLimiter()
{
	if (!enable_frame_limit)
		return;

	if (present_time < frame_ratio)
	{
		auto now = system_clock::now();
		const milliseconds delta = duration_cast<milliseconds>(now - frame_start);

		if (delta < frame_ratio)
		{
			// sleep for a portion of the frame time to free up cpu time
			std::this_thread::sleep_for(frame_portion_ms - delta);

			while ((now = system_clock::now()) - frame_start < frame_ratio)
			{
				// spin for the remainder of the time
			}
		}
	}

	frame_start = system_clock::now();
}

static void __fastcall Present_r(Magic::RenderCore::RenderDevice_DX9* dev)
{
	if (enable_frame_limit)
	{
		// This is done to avoid vsync issues.
		const auto start = std::chrono::system_clock::now();
		dev->m_pD3DDevice->Present(nullptr, nullptr, DrawWindow, nullptr);
		present_time = std::chrono::system_clock::now() - start;
	}
	else
	{
		dev->m_pD3DDevice->Present(nullptr, nullptr, DrawWindow, nullptr);
	}
}

static void __fastcall BeginScene_r(Magic::RenderCore::RenderDevice_DX9* dev)
{
	dev->m_pD3DDevice->BeginScene();
	RunFrameLimiter();
	RaiseEvents(onRenderSceneStart);
}

static void __fastcall EndScene_r(Magic::RenderCore::RenderDevice_DX9* dev)
{
	RaiseEvents(onRenderSceneEnd);
	dev->m_pD3DDevice->EndScene();
}

void direct3d::reset_device()
{
	RaiseEvents(modRenderDeviceLost);
	g_pRenderDevice->__vftable->ResetRenderDeviceInitInfo(g_pRenderDevice, &g_pRenderDevice->m_InitInfo, &DeviceLostFunc, &DeviceResetFunc);
	RaiseEvents(modRenderDeviceReset);
}

void direct3d::change_resolution(int w, int h)
{
	change_resolution(w, h, g_pRenderDevice->m_pDeviceCreator->m_D3DPP.Windowed);
}

void direct3d::change_resolution(int w, int h, bool windowed)
{
	// Make sure at least one parameter is different before resetting the device
	auto& pp = g_pRenderDevice->m_pDeviceCreator->m_D3DPP;
	if (pp.BackBufferWidth != w || pp.BackBufferHeight != h || pp.Windowed != (BOOL)windowed)
	{
		HorizontalResolution = static_cast<float>(w);
		VerticalResolution = static_cast<float>(h);
		g_pRenderDevice->m_InitInfo.m_BackBufferWidth = w;
		g_pRenderDevice->m_InitInfo.m_BackBufferHeight = h;
		pp.BackBufferWidth = static_cast<UINT>(w);
		pp.BackBufferHeight = static_cast<UINT>(h);
		reset_device();
	}
}

void direct3d::change_dest_window(HWND hwnd)
{
	DrawWindow = hwnd;
}

void direct3d::enable_frame_limiter()
{
	enable_frame_limit = true;
}

void direct3d::init()
{
	WriteJump(Magic::RenderCore::Present, Present_r);
	WriteJump(Magic::RenderCore::BeginScene, BeginScene_r);
	WriteJump(Magic::RenderCore::EndScene, EndScene_r);
}