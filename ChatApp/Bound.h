#pragma once

/*
 * ‹«ŠE‚ð•\‚·Šî’êƒNƒ‰ƒX
 */

#include "DxLib.h"

class Bound
{
public:
	Bound() = default;
	virtual ~Bound() = default;

public:
	virtual RECT GetRect() const = 0;
	virtual bool IsIn(int x, int y) const = 0;
};
