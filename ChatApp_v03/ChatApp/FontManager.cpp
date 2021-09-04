#include "FontManager.h"
#include "Logger.h"


FontManager::FontManager()
	: m_FontHandle()
{
}

void FontManager::Initialize()
{
	Destroy();
	AddFont("ＭＳ ゴシック", 16, Type::SMALL);
	AddFont("ＭＳ ゴシック", 20, Type::MIDDLE);
	AddFont("ＭＳ ゴシック", 32, Type::BIG);
	AddFont("ＭＳ ゴシック", 64, Type::HUGE);
}

void FontManager::Destroy()
{
	for (auto& Pair : m_FontHandle)
	{
		Logger::Info("フォント破棄：type = %d", static_cast<int>(Pair.first));
		DeleteGraph(Pair.second);
	}
	m_FontHandle.clear();
}

int FontManager::GetFontHandle(Type type)
{
	return m_FontHandle.at(type);
}

void FontManager::AddFont(const char* FontName, int Size, Type type)
{
	Logger::Info("フォント追加：type = %d", static_cast<int>(type));
	m_FontHandle[type] = CreateFontToHandle(FontName, Size, -1);
}
