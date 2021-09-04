#pragma once

/*
 * 画面部品の親クラス
*/

#include "Bound.h"
#include <memory>

class ScreenComponent
{
public:
	ScreenComponent();
	ScreenComponent(Bound* bound);
	virtual ~ScreenComponent() = default;

public:
	// マウス操作に反応するための関数
	virtual void OnMouseMove(int x, int y) = 0;
	virtual void OnMousePress(int x, int y) = 0;

	// 自身を描画するための関数
	virtual void Draw() const = 0;

protected:
	// 自身の大きさを表す境界
	std::unique_ptr<Bound> m_Bound;
};

inline ScreenComponent::ScreenComponent()
	: m_Bound()
{
}
inline ScreenComponent::ScreenComponent(Bound* bound)
	: m_Bound(bound)
{
}
