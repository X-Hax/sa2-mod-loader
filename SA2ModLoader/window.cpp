#include "stdafx.h"
#include <GdiPlus.h>
#include <vector>
#include "IniFile.hpp"
#include "FileSystem.h"
#include "magic.h"
#include "direct3d.h"
#include "UsercallFunctionHandler.h"

// Improves the main window as soon as the loader takes control

VoidFunc(RestoreSounds, 0x435FD0);
VoidFunc(StopSounds, 0x436020);
VoidFunc(StopInputs, 0x439060);

#define IS_FULLSCREEN MainUserConfig->data.Fullscreen

static std::vector<RECT> screenBounds;
static int screenNum = 1;

static RECT   last_rect = {};
static float  last_width = 0.0f;
static float  last_height = 0.0f;
static DWORD  last_style = 0;
static DWORD  last_exStyle = 0;

static bool customWindowSize = false;
static bool windowResize = false;
static bool windowedFullscreen = false;
static bool disableExitPrompt = false;
static bool pauseWhenInactive = true;
static bool maintainAspectRatio = true;
static bool windowWrapper = false;
static double targetAspectRatio = 4 / 3;
static int customWindowWidth = 640;
static int customWindowHeight = 480;

static Gdiplus::Bitmap* backgroundImage = nullptr;
static HWND innerWindow = NULL;
static int innerWidth = 640;
static int innerHeight = 480;

auto PrintExitPrompt = GenerateUsercallWrapper<int(__cdecl*)(HWND hwnd)>(rEAX, 0x4015F0, rEDI);

static void enable_fullscreen_mode(HWND handle)
{
	IS_FULLSCREEN = true;

	// Backup windowed mode information
	last_width = HorizontalResolution;
	last_height = VerticalResolution;

	GetWindowRect(handle, &last_rect);

	last_style = GetWindowLongA(handle, GWL_STYLE);
	last_exStyle = GetWindowLongA(handle, GWL_EXSTYLE);

	// Set to borderless fullscreen
	auto rect = screenBounds[screenNum == 0 ? 0 : screenNum - 1];
	SetWindowPos(handle, nullptr, rect.left, rect.top, rect.right, rect.bottom, SWP_FRAMECHANGED);
	SetWindowLongA(handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowLongA(handle, GWL_EXSTYLE, WS_EX_APPWINDOW);

	while (ShowCursor(FALSE) > 0);
}

static void enable_windowed_mode(HWND handle)
{
	IS_FULLSCREEN = false;

	// Restore windowed mode information
	SetWindowLongA(handle, GWL_STYLE, last_style);
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

	// Restore windowed mode
	SetWindowPos(handle, HWND_NOTOPMOST, last_rect.left, last_rect.top, width, height, SWP_FRAMECHANGED);

	while (ShowCursor(TRUE) < 0);
}

static void update_innerwindow(int w, int h)
{
	if (maintainAspectRatio)
	{
		auto w_ = customWindowSize ? customWindowWidth : w;
		auto h_ = customWindowSize ? customWindowHeight : h;

		if (w > h * targetAspectRatio)
		{
			innerWidth = static_cast<int>(h_ * targetAspectRatio);
			innerHeight = h_;
		}
		else
		{
			innerWidth = w_;
			innerHeight = static_cast<int>(w_ / targetAspectRatio);
		}
	}
	else if (customWindowSize)
	{
		innerWidth = customWindowWidth;
		innerHeight = customWindowHeight;
	}
	else
	{
		innerWidth = w;
		innerHeight = h;
	}

	SetWindowPos(innerWindow, HWND_TOP, (w - innerWidth) / 2, (h - innerHeight) / 2, innerWidth, innerHeight, 0);
}

static void change_resolution(int w, int h, BOOL windowed)
{
	// Update the inner window if it exists
	if (windowWrapper)
	{
		update_innerwindow(w, h);

		// Use the calculated inner window size as the backbuffer size
		w = innerWidth;
		h = innerHeight;
	}

	direct3d::change_resolution(w, h, windowed);
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
		change_resolution(static_cast<int>(last_width), static_cast<int>(last_height), false);
		enable_windowed_mode(handle);
	}
}

static bool Close(HWND handle)
{
	if (disableExitPrompt)
		return true;

	return !(!IS_FULLSCREEN && PrintExitPrompt(handle) == 7);
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
	else if (WindowActive && pauseWhenInactive)
	{
		WindowActive = 0;

		// Fix a potential vanilla crash by checking if the sound system pointer is not empty before using it
		if (*(intptr_t*)0x1A55998) 
			StopSounds();
		StopInputs();
	}
}

static bool DrawBackground(HWND handle, WPARAM wParam)
{
	if (backgroundImage == nullptr)
	{
		return false;
	}

	Gdiplus::Graphics gfx((HDC)wParam);
	gfx.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
	
	RECT rect;
	GetClientRect(handle, &rect);

	auto w = rect.right - rect.left;
	auto h = rect.bottom - rect.top;

	if (w == innerWidth && h == innerHeight)
	{
		return false;
	}

	// Draw border image
	gfx.DrawImage(backgroundImage, 0, 0, w, h);
	return true;
}

static LRESULT CALLBACK WndProc_Resizable(HWND handle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		exit(0);
		break;
	case WM_SIZE:
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
	case WM_ERASEBKGND:
		if (DrawBackground(handle, wParam))
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

	return DefWindowProcW(handle, Msg, wParam, lParam);
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
	case WM_ERASEBKGND:
		if (DrawBackground(handle, wParam))
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

	return DefWindowProcW(handle, Msg, wParam, lParam);
}

static BOOL CALLBACK GetMonitorSize(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	screenBounds.push_back(*lprcMonitor);
	return TRUE;
}

enum screenmodes { window_mode, borderless_mode, custom_mode, fullscreen_mode };

void PatchWindow(const LoaderSettings& settings, std::wstring& borderimg)
{
	if (IS_FULLSCREEN)
	{
		return;
	}

	maintainAspectRatio = !settings.KeepAspectWhenResizing;	// When this toggle is enabled, maintaining the aspect should be disabled.
	windowResize = settings.ResizableWindow;
	screenNum = settings.ScreenNum;
	disableExitPrompt = settings.DisableExitPrompt;
	pauseWhenInactive = settings.PauseWhenInactive;

	switch (settings.ScreenMode)
	{
	case window_mode:
		windowedFullscreen = false;
		customWindowSize = false;
		break;
	case borderless_mode:
		windowedFullscreen = true;
		customWindowSize = false;
		break;
	case custom_mode:
		windowedFullscreen = false;
		customWindowSize = true;
		customWindowWidth = settings.WindowWidth;
		customWindowHeight = settings.WindowHeight;
		break;
	default:
		return;
	}

	windowWrapper = maintainAspectRatio;

	// Replace the default window procedure
	WriteJump(reinterpret_cast<void*>(0x00401810), WndProc_Hook);

	// Override the current window procedure
	SetWindowLongW(MainWindowHandle, GWL_WNDPROC, (LONG)(windowResize ? WndProc_Resizable : WndProc_Hook));

	// Force the window to the foreground to prevent the console from setting the window inactive
	SetForegroundWindow(MainWindowHandle);

	// Set missing background brush to prevent ghost screen while resizing.
	SetClassLongW(MainWindowHandle, GCL_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));

	RECT windowRect;
	windowRect.top = 0;
	windowRect.left = 0;

	if (customWindowSize)
	{
		windowRect.right = settings.WindowWidth;
		windowRect.bottom = settings.WindowHeight;
	}
	else
	{
		windowRect.right = MainUserConfig->data.Width;
		windowRect.bottom = MainUserConfig->data.Height;
	}

	// Get all of the display monitors virtual positions
	EnumDisplayMonitors(nullptr, nullptr, GetMonitorSize, 0);

	int screenX, screenY, screenW, screenH, wsX, wsY, wsW, wsH;
	if (screenNum > 0)
	{
		if (screenBounds.size() < static_cast<size_t>(screenNum))
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
	
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	
	if (windowResize)
	{
		dwStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
	}

	if (windowWrapper)
	{
		// Calculate aspect ratio for the MaintainAspectRatio option
		targetAspectRatio = HorizontalResolution / VerticalResolution;

		// If a mod changed the border image path to something invalid, restore normal path
		if (!FileExists(borderimg))
		{
			borderimg = L"mods\\Border_Default.png";
		}

		// Try to load the border image
		if (FileExists(borderimg))
		{
			Gdiplus::GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
			backgroundImage = Gdiplus::Bitmap::FromFile(borderimg.c_str());
		}
	}

	// Let Windows calculate any extra space required by the style
	AdjustWindowRectEx(&windowRect, dwStyle, false, 0);

	int w = windowRect.right - windowRect.left;
	int h = windowRect.bottom - windowRect.top;
	int x = wsX + ((wsW - w) / 2);
	int y = wsY + ((wsH - h) / 2);

	// Update the main window
	if (windowedFullscreen)
	{
		IS_FULLSCREEN = TRUE;

		// If the user can swap to windowed mode, set return window information
		if (windowResize)
		{
			last_width = HorizontalResolution;
			last_height = VerticalResolution;
			last_style = dwStyle;
			last_rect = { x, y, x + w, y + h };
		}

		// Apply fake fullscreen
		SetWindowLongW(MainWindowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowLongW(MainWindowHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
		SetWindowPos(MainWindowHandle, nullptr, screenX, screenY, screenW, screenH, SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowLongW(MainWindowHandle, GWL_STYLE, dwStyle);
		SetWindowPos(MainWindowHandle, nullptr, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

	// Create an inner window to wrap the game
	if (windowWrapper)
	{
		LPCWSTR const lpszClassName = L"SONIC ADVENTURE 2";

		innerWindow = CreateWindowExW(0,
			lpszClassName,
			lpszClassName,
			WS_CHILD | WS_VISIBLE,
			0, 0, static_cast<int>(HorizontalResolution), static_cast<int>(VerticalResolution),
			MainWindowHandle, nullptr, GetModuleHandle(NULL), nullptr);

		if (innerWindow == NULL)
		{
			windowWrapper = false;
			return;
		}
		
		direct3d::change_dest_window(innerWindow);

		// Position the inner window properly
		if (windowedFullscreen)
		{
			update_innerwindow(screenW, screenH);
		}
		else
		{
			GetClientRect(MainWindowHandle, &windowRect);
			update_innerwindow(windowRect.right, windowRect.bottom);
		}
	}
}