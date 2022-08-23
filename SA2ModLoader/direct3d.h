#pragma once

namespace direct3d
{
	void reset_device();
	void change_resolution(int w, int h);
	void change_resolution(int w, int h, bool windowed);
	void change_dest_window(HWND hwnd);
	void enable_frame_limiter();
	void init();
}