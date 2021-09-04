#pragma once

/*
 * 画面部品：トグル(ON-OFFスイッチ)
 * ON-OFFが変更されたときのコールバック関数は
 * DelegateArgBaseクラスにして渡す
 */

#include "ScLabel.h"
#include "DelegateArg.h"
#include <string>
#include <memory>

class ScToggle : public ScLabel
{
public:
	// 背景に色があるボタン、自身の境界は好きに設定できる
	// @param ColorFore-FontHandle 基底クラス(ScLabel)に同じ
	// @param Callback             ON-OFFが変更されたときのコールバック関数
	ScToggle(unsigned int ColorFore, unsigned int ColorBack, Bound* bound, int FontHandle,
		DelegateArgBase<bool>* Callback);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

protected:
	bool m_Hover;
	int m_PressCount;
	int m_Padding;
	bool m_OffOn;
	std::unique_ptr<DelegateArgBase<bool>> m_Callback;
};
