#include "stdafx.h"
#include <vector>
#include "IniFile.hpp"
#include "magic.h"
#include "UsercallFunctionHandler.h"

VoidFunc(RestoreSounds, 0x435FD0);
VoidFunc(StopSounds, 0x436020);
VoidFunc(StopInputs, 0x439060);

#define IS_FULLSCREEN MainUserConfig->data.Fullscreen

static std::vector<RECT> screenBounds;
static int screenNum = 1;

static RECT   last_rect = {};
static Uint32 last_width = 0;
static Uint32 last_height = 0;
static DWORD  last_style = 0;
static DWORD  last_exStyle = 0;

static bool customWindowSize = false;
static bool windowResize = false;
static bool borderlessWindow = false;
static bool vsync = false;
static bool disableCloseMessage = false;

auto PrintCloseMessage = GenerateUsercallWrapper<int(__cdecl*)(HWND hwnd)>(rEAX, 0x4015F0, rEDI);

static void enable_fullscreen_mode(HWND handle)
{
	IS_FULLSCREEN = true;
	last_width = HorizontalResolution;
	last_height = VerticalResolution;

	GetWindowRect(handle, &last_rect);

	last_style = GetWindowLongA(handle, GWL_STYLE);
	last_exStyle = GetWindowLongA(handle, GWL_EXSTYLE);

	if (borderlessWindow)
	{
		auto rect = screenBounds[max(0, screenNum - 1)];
		SetWindowPos(handle, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_FRAMECHANGED);
		SetWindowLongA(handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowLongA(handle, GWL_EXSTYLE, WS_EX_APPWINDOW);
	}
	else
	{
		SetWindowLongA(handle, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
		ShowWindow(handle, SW_SHOWMAXIMIZED);
	}

	while (ShowCursor(FALSE) > 0);
}

static void enable_windowed_mode(HWND handle)
{
	IS_FULLSCREEN = false;

	SetWindowLongA(handle, GWL_STYLE, last_style | WS_SYSMENU);
	SetWindowLongA(handle, GWL_EXSTYLE, last_exStyle);

	auto width = last_rect.right - last_rect.left;
	auto height = last_rect.bottom - last_rect.top;

	if (width <= 0 || height <= 0)
	{
		last_rect = {};

		last_rect.right = 640;
		last_rect.bottom = 480;

		AdjustWindowRectEx(&last_rect, last_style, false, last_exStyle);

		width = last_rect.right - last_rect.left;
		height = last_rect.bottom - last_rect.top;
	}

	SetWindowPos(handle, HWND_NOTOPMOST, last_rect.left, last_rect.top, width, height, 0);

	if (!borderlessWindow)
		ShowWindow(handle, SW_SHOWNORMAL);

	while (ShowCursor(TRUE) < 0);
}

static void setup_vsync()
{
	auto& p = g_pRenderDevice->m_pDeviceCreator->m_D3DPP;

	if (vsync)
	{
		p.SwapEffect = D3DSWAPEFFECT_COPY;
		p.PresentationInterval = IS_FULLSCREEN ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_DEFAULT;
	}
	else
	{
		p.SwapEffect = D3DSWAPEFFECT_DISCARD;
		p.PresentationInterval = IS_FULLSCREEN ? D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_DEFAULT;
	}
}

static void reset_device()
{
	setup_vsync();
	*(BYTE*)&g_pSOCRender->field_644 = 1; // Reload shaders
	g_pRenderDevice->__vftable->ResetRenderDeviceInitInfo(g_pRenderDevice, &g_pRenderDevice->m_InitInfo, &DeviceLostFunc, &DeviceResetFunc);
}

static void change_resolution(int w, int h, bool windowed)
{
	Magic::RenderCore::RenderDeviceInitInfo_t info;
	g_pRenderDevice->__vftable->GetRenderDeviceInitInfo(g_pRenderDevice, &info);

	info.m_BackBufferWidth = HorizontalResolution;
	info.m_BackBufferHeight = VerticalResolution;
	info.m_IsWindow = windowed;

	HorizontalResolution = w;
	VerticalResolution = h;

	g_pRenderDevice->m_pDeviceCreator->m_D3DPP.BackBufferWidth = w;
	g_pRenderDevice->m_pDeviceCreator->m_D3DPP.BackBufferHeight = h;
	g_pRenderDevice->m_pDeviceCreator->m_D3DPP.Windowed = windowed;

	g_pVideoDevice.m_VideoMode.Width = w;
	g_pVideoDevice.m_VideoMode.Height = h;

	reset_device();
}

static void swap_window_mode(HWND handle)
{
	if (!IS_FULLSCREEN && g_pRenderDevice->m_pDeviceCreator->m_D3DPP.Windowed)
	{
		enable_fullscreen_mode(handle);

		const auto& rect = screenBounds[screenNum == 0 ? 0 : screenNum - 1];

		const auto w = rect.right - rect.left;
		const auto h = rect.bottom - rect.top;

		change_resolution(w, h, true);
	}
	else
	{
		change_resolution(last_width, last_height, false);
		enable_windowed_mode(handle);
	}
}

static bool Close(HWND handle)
{
	return !(!disableCloseMessage && !IS_FULLSCREEN && PrintCloseMessage(handle) == 7);
}

static void Activate(bool activating)
{
	if (activating)
	{
		if (!WindowActive)
		{
			WindowActive = 1;
			RestoreSounds();
		}
	}
	else if (WindowActive)
	{
		WindowActive = 0;

		if (*(intptr_t*)0x1A55998)
			StopSounds();
		StopInputs();
	}
}

static LRESULT CALLBACK WndProc_Resizable(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		exit(0);
		break;
	case WM_SIZE:
		if (customWindowSize)
		{
			break;
		}

		if (IS_FULLSCREEN || g_pRenderDevice == nullptr)
		{
			return 0;
		}
		else
		{
			int w = LOWORD(lParam);
			int h = HIWORD(lParam);

			if (!w || !h)
			{
				break;
			}

			change_resolution(w, h, false);
		}

		break;
	case WM_ACTIVATE:
		Activate(wParam != FALSE);
		break;
	case WM_CLOSE:
		if (!Close(handle))
			return 0;
		break;
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN)
		{
			swap_window_mode(handle);
			return 0;
		}

		if (wParam != VK_F4 && wParam != VK_F2) return 0;
		break;
	case WM_SYSKEYUP:
		if (wParam != VK_F4 && wParam != VK_F2 && wParam != VK_RETURN) return 0;
		break;
	case WM_MENUCHAR:
		return MNC_CLOSE << 16;
	}

	return DefWindowProcA(handle, Msg, wParam, lParam);
}

static LRESULT CALLBACK WndProc_Hook(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		exit(0);
		break;
	case WM_CLOSE:
		if (!Close(handle))
			return 0;
		break;
	case WM_ACTIVATE:
		Activate(wParam != FALSE);
		break;
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		if (wParam != VK_F4 && wParam != VK_F2 && wParam != VK_RETURN) return 0;
		break;
	}

	return DefWindowProcA(handle, Msg, wParam, lParam);
}

static BOOL CALLBACK GetMonitorSize(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	screenBounds.push_back(*lprcMonitor);
	return TRUE;
}

void PatchWindow(const IniGroup* settings)
{
	customWindowSize = settings->getBool("CustomWindowSize");
	windowResize = settings->getBool("ResizableWindow") && !customWindowSize;
	borderlessWindow = settings->getBool("BorderlessWindow");
	screenNum = settings->getInt("ScreenNum");
	vsync = settings->getBool("EnableVsync", true);
	disableCloseMessage = settings->getBool("DisableCloseMessage");

	// Hook default return of SA2's window procedure to force it to return DefWindowProc
	WriteJump(reinterpret_cast<void*>(0x00401810), WndProc_Hook);

	SetWindowLongW(MainWindowHandle, GWL_WNDPROC, (LONG)(windowResize ? WndProc_Resizable : WndProc_Hook));

	SetForegroundWindow(MainWindowHandle);

	RECT windowRect;
	windowRect.top = 0;
	windowRect.left = 0;

	if (customWindowSize)
	{
		windowRect.right = settings->getInt("WindowWidth", 640);
		windowRect.bottom = settings->getInt("WindowHeight", 480);
	}
	else
	{
		windowRect.right = MainUserConfig->data.Width;
		windowRect.bottom = MainUserConfig->data.Height;
	}

	EnumDisplayMonitors(nullptr, nullptr, GetMonitorSize, 0);

	int screenX, screenY, screenW, screenH, wsX, wsY, wsW, wsH;
	if (screenNum > 0)
	{
		if (screenBounds.size() < screenNum)
		{
			screenNum = 1;
		}

		RECT screenSize = screenBounds[screenNum - 1];

		wsX = screenX = screenSize.left;
		wsY = screenY = screenSize.top;
		wsW = screenW = screenSize.right - screenSize.left;
		wsH = screenH = screenSize.bottom - screenSize.top;
	}
	else
	{
		screenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
		screenY = GetSystemMetrics(SM_YVIRTUALSCREEN);
		screenW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		screenH = GetSystemMetrics(SM_CYVIRTUALSCREEN);

		wsX = 0;
		wsY = 0;
		wsW = GetSystemMetrics(SM_CXSCREEN);
		wsH = GetSystemMetrics(SM_CYSCREEN);
	}
	
	if (borderlessWindow)
	{
		IS_FULLSCREEN = TRUE;
	}

	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	
	if (windowResize)
	{
		dwStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
	}

	AdjustWindowRectEx(&windowRect, dwStyle, false, 0);

	int w = windowRect.right - windowRect.left;
	int h = windowRect.bottom - windowRect.top;
	int x = wsX + ((wsW - w) / 2);
	int y = wsY + ((wsH - h) / 2);

	SetWindowLong(MainWindowHandle, GWL_STYLE, dwStyle);
	SetWindowPos(MainWindowHandle, nullptr, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);

	if (IS_FULLSCREEN)
	{
		enable_fullscreen_mode(MainWindowHandle);
	}
}