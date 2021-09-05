#include "SceneChoiceHostGuest.h"
#include "ScLabel.h"
#include "ScButton.h"
#include "FontManager.h"
#include "SceneCreator.h"
#include "Common.h"
#include "Logger.h"

SceneChoiceHostGuest::SceneChoiceHostGuest()
	: m_Next()
{
	// コンストラクタでは画面の部品を配置する
	LOG_INFO("ホスト・ゲスト選択画面生成");

	int SmallFont = FontManager::Inst().GetFontHandle(FontManager::Type::SMALL);
	int HugeFont = FontManager::Inst().GetFontHandle(FontManager::Type::HUGE);

	AddBaseComponent(new ScLabel("ホストになるかゲストになるかを選んでください", GetColor(0, 255, 0), 10, 10, SmallFont));

	{
		unsigned int ColorFore = GetColor(0, 0, 0);
		unsigned int ColorBack = GetColor(200, 180, 180);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		unsigned int ColorBackHover = GetColor(220, 200, 200);
		unsigned int ColorBackPress = GetColor(180, 160, 160);
		auto CallbackHost = new DelegateVoid<SceneChoiceHostGuest>(*this, &SceneChoiceHostGuest::BeHost);
		AddBaseComponent(new ScButton("ホスト", ColorFore, ColorBack, 100, 100, HugeFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
	{
		unsigned int ColorFore = GetColor(0, 0, 0);
		unsigned int ColorBack = GetColor(180, 180, 200);
		unsigned int ColorFrame = GetColor(50, 50, 100);
		unsigned int ColorBackHover = GetColor(200, 200, 220);
		unsigned int ColorBackPress = GetColor(160, 160, 200);
		auto CallbackGuest = new DelegateVoid<SceneChoiceHostGuest>(*this, &SceneChoiceHostGuest::BeGuest);
		AddBaseComponent(new ScButton("ゲスト", ColorFore, ColorBack, 100, 200, HugeFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackGuest));
	}
	{
		unsigned int ColorFore = GetColor(0, 0, 0);
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto CallbackHost = new DelegateVoid<SceneChoiceHostGuest>(*this, &SceneChoiceHostGuest::End);
		AddBaseComponent(new ScButton("終了", ColorFore, ColorBack, 100, 500, HugeFont, ColorFrame, ColorBackHover, ColorBackPress, CallbackHost));
	}
}

Scene* SceneChoiceHostGuest::Update()
{
	UpdateBase();
	return m_Next;
}

void SceneChoiceHostGuest::Draw()
{
	ClearDrawScreen();
	DrawBase();
}

void SceneChoiceHostGuest::BeHost()
{
	if (m_Next == nullptr)
		m_Next = SceneCreator::Create(SceneCreator::Name::HOST);
}

void SceneChoiceHostGuest::BeGuest()
{
	if (m_Next == nullptr)
		m_Next = SceneCreator::Create(SceneCreator::Name::GUEST);
}

void SceneChoiceHostGuest::End()
{
	Common::ApplicationExecution = false;
}
