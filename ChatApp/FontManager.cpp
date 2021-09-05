#include "FontManager.h"
#include "Logger.h"


FontManager::FontManager()
	: m_FontHandle()
{
}

void FontManager::Initialize()
{
	Destroy();
	AddFont("‚l‚r ƒSƒVƒbƒN", 13, Type::TINY);
	AddFont("‚l‚r ƒSƒVƒbƒN", 16, Type::SMALL);
	AddFont("‚l‚r ƒSƒVƒbƒN", 20, Type::MIDDLE);
	AddFont("‚l‚r ƒSƒVƒbƒN", 32, Type::BIG);
	AddFont("‚l‚r ƒSƒVƒbƒN", 64, Type::HUGE);
}

void FontManager::Destroy()
{
	for (auto& Pair : m_FontHandle)
	{
		LOG_INFO("ƒtƒHƒ“ƒg”jŠüFtype = %d", static_cast<int>(Pair.first));
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
	LOG_INFO("ƒtƒHƒ“ƒg’Ç‰ÁFtype = %d", static_cast<int>(type));
	m_FontHandle[type] = CreateFontToHandle(FontName, Size, -1);
}
