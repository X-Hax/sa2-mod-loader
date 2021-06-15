#pragma once

namespace debug_text
{
	void Initialize();
	void DisplayGameDebug(const char* buf);
	void SetFontSize(float size);
	void SetFontColor(int color);
	void DisplayString(int loc, const char* str);
	void DisplayNumber(int loc, int value, signed int numdigits);
	void DisplayStringFormatted(int loc, const char* Format, ...);
	void sub_759AA0(int a1, int a2, int a3, int a4, int a5);
}