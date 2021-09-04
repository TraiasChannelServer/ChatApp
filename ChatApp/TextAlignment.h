#pragma once

/*
 * テキストの左寄せ・中央揃えを計算するクラス
 */

#include <string>
#include "DxLib.h"

class TextAlignment
{
public:
	enum class Gravity
	{
		CENTER,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};

public:
	static RECT Execute(const std::string& Text, const RECT& Entire, Gravity gravity, int FontHandle, int Padding = 0);
	
public:
	TextAlignment() = delete;
};