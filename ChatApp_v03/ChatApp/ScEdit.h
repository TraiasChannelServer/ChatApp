#pragma once

/*
 * 画面部品：エディットテキスト
 * テキストが変更されたときのコールバック関数は
 * DelegateArgBaseクラスにして渡す
 */

#include "ScLabel.h"
#include "DelegateArg.h"
#include <string>
#include <memory>

class ScEdit : public ScLabel
{
private:
	static constexpr int FRAME_WIDTH = 2;

public:
	// 背景に色があるボタン、自身の境界は好きに設定できる
	// @param Text-FontHandle 基底クラス(ScLabel)に同じ
	// @param ColorFrame      境界線の色
	// @param TextSize        入力できる最大文字数
	// @param Callback        テキストが変更されたときのコールバック関数
	ScEdit(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, int FontHandle,
		unsigned int ColorFrame, size_t TextSize, DelegateArgBase<std::string>* Callback);
	~ScEdit();

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

protected:
	unsigned int m_ColorFrame;
	bool m_Hover;
	int m_PressCount;
	size_t m_TextSize;
	std::unique_ptr<DelegateArgBase<std::string>> m_Callback;
};
