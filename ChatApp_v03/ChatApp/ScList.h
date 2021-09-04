#pragma once

/*
 * 画面部品：リスト
 * 複数の文字列を所持する
 * 文字列の追加・削除を動的に行える
 * 追加・削除する一つ分をItemと呼ぶ
 * 一つのItemに一つのIDを設定する
 */

#include "ScreenComponent.h"
#include "TextAlignment.h"
#include <string>
#include <map>

class ScList : public ScreenComponent
{
public:
	// 描画の度にアライメントを計算していては無駄なので
	// 計算結果(x, y)を保持しておく
	struct Item
	{
		std::string Text = {};
		int x = 0;
		int y = 0;
	};

public:
	// 背景に色があるラベル、自身の境界は好きに設定できる
	// @param ColorFore  文字色
	// @param ColorBack  背景色
	// @param bound      Item一つ分の境界
	// @param gravity    左寄せとか中央揃えとか
	// @param FontHandle フォント
	// @param ColorFrame 境界線の色
	ScList(unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle, unsigned int ColorFrame);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

	void AddItem(int ID, const std::string& Text);
	void RemoveItem(int ID);
	void RemoveAllItem();
	void ChangeText(int ID, const std::string& Text);

	const std::string& GetText(int ID) const;
	const std::map<int, Item>& GetItem() const;

protected:
	// 文字列の表示位置を計算
	void Alignment();

protected:
	unsigned int m_ColorFore;
	unsigned int m_ColorBack;
	unsigned int m_ColorFrame;
	TextAlignment::Gravity m_Gravity;
	std::map<int, Item> m_Item;
	int m_FontHandle;
	int m_ItemYSize;
};
