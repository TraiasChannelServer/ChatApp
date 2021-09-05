#pragma once

/*
 * 画面部品：ボタン
 * 押された時のコールバック関数は
 * DelegateVoidBaseクラスにして渡す
 */

#include "ScLabel.h"
#include "DelegateVoid.h"
#include <string>
#include <memory>

class ScButton : public ScLabel
{
private:
	static constexpr int FRAME_WIDTH_RELEASE = 2;
	static constexpr int FRAME_WIDTH_PRESS = 3;

public:
	// 背景に色があるボタン、自身の境界は自動設定
	ScButton(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, int x, int y, int FontHandle,
		unsigned int ColorFrame, unsigned int ColorBackHover, unsigned int ColorBackPress, DelegateVoidBase* Callback);

	// 背景に色があるボタン、自身の境界は好きに設定できる
	// @param Text-FontHandle 基底クラス(ScLabel)に同じ
	// @param ColorFrame      境界線の色
	// @param ColorBackHover  マウスを重ねたときの背景色
	// @param ColorBackPress  マウス押下しているときの背景色
	// @param Callback        ボタンを押したときのコールバック関数、nullable
	ScButton(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle,
		unsigned int ColorFrame, unsigned int ColorBackHover, unsigned int ColorBackPress, DelegateVoidBase* Callback);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

protected:
	unsigned int m_ColorFrame;
	unsigned int m_ColorBackHover;
	unsigned int m_ColorBackPress;

	// マウスが重なっているか、クリックされてるかを
	// 感知するための変数
	bool m_Hover;
	int m_PressCount;

	std::unique_ptr<DelegateVoidBase> m_Callback;
};
