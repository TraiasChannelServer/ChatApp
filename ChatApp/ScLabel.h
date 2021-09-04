#pragma once

/*
 * 画面部品：ラベル
 * テキストを表示するのみ
 * マウス操作には反応しない
 */

#include "ScreenComponent.h"
#include "TextAlignment.h"
#include <string>

class ScLabel : public ScreenComponent
{
public:
	// 背景が透明のラベル、自身の境界は自動設定
	ScLabel(const std::string& Text, unsigned int ColorFore, int x, int y, int FontHandle);

	// 背景に色があるラベル、自身の境界は自動設定
	ScLabel(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, int x, int y, int FontHandle);

	// 背景に色があるラベル、自身の境界は好きに設定できる
	// @param Text       文字列
	// @param ColorFore  文字色
	// @param ColorBack  背景色
	// @param bound      境界
	// @param gravity    左寄せとか中央揃えとか
	// @param FontHandle フォント
	ScLabel(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle);

	virtual ~ScLabel() = default;

public:
	virtual void OnMouseMove(int x, int y) override;
	virtual void OnMousePress(int x, int y) override;
	virtual void Draw() const override;

	// 文字列を変更する
	// m_FitBoundMode によって境界は調整される
	void ChangeText(const std::string& Text);

	const std::string& GetText() const;

protected:
	// 自身の境界を文字列にフィットさせる
	void FitBound();

protected:
	std::string m_Text;
	unsigned int m_ColorFore;
	unsigned int m_ColorBack;
	bool m_BkColorTransparent;
	TextAlignment::Gravity m_Gravity;
	int m_TextX;
	int m_TextY;
	int m_FontHandle;

	// 自身の境界を文字列にフィットさせるのか否か、呼ばれたコンストラクタによって固定
	bool m_FitBoundMode;
};

inline const std::string& ScLabel::GetText() const
{
	return m_Text;
}