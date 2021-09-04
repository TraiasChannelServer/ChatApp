#include "TextAlignment.h"

RECT TextAlignment::Execute(const std::string& Text, const RECT& Entire, Gravity gravity, int FontHandle, int Padding)
{
	int TextXSize = GetDrawStringWidthToHandle(Text.c_str(), Text.size(), FontHandle);
	int TextYSize = GetFontSizeToHandle(FontHandle);

	int AlignedX = 0;
	int AlignedY = 0;

	if (gravity == Gravity::LEFT || gravity == Gravity::TOP_LEFT || gravity == Gravity::BOTTOM_LEFT)
	{
		AlignedX = Entire.left + Padding;
	}
	else if (gravity == Gravity::RIGHT || gravity == Gravity::TOP_RIGHT || gravity == Gravity::BOTTOM_RIGHT)
	{
		AlignedX = Entire.right - TextXSize - Padding;
	}
	else
	{
		AlignedX = (Entire.left + Entire.right - TextXSize) / 2;
	}

	if (gravity == Gravity::TOP || gravity == Gravity::TOP_LEFT || gravity == Gravity::TOP_RIGHT)
	{
		AlignedY = Entire.top + Padding;
	}
	else if (gravity == Gravity::BOTTOM || gravity == Gravity::BOTTOM_LEFT || gravity == Gravity::BOTTOM_RIGHT)
	{
		AlignedY = Entire.bottom - TextYSize - Padding;
	}
	else
	{
		AlignedY = (Entire.top + Entire.bottom - TextYSize) / 2;
	}

	return {AlignedX, AlignedY, AlignedX + TextXSize, AlignedY + TextYSize};
}
