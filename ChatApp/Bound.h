#pragma once

/*
 * 境界を表す基底クラス
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
