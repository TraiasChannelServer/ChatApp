#include "ScLabel.h"
#include "BoundRect.h"
#include "DxLib.h"
#include "Logger.h"

ScLabel::ScLabel(const std::string& Text, unsigned int ColorFore, int x, int y, int FontHandle)
	: m_Text(Text)
	, m_ColorFore(ColorFore)
	, m_ColorBack()
	, m_BkColorTransparent(true)
	, m_Gravity()
	, m_TextX(x)
	, m_TextY(y)
	, m_FontHandle(FontHandle)
	, m_FitBoundMode(true)
{
	FitBound();
}

ScLabel::ScLabel(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, int x, int y, int FontHandle)
	: m_Text(Text)
	, m_ColorFore(ColorFore)
	, m_ColorBack(ColorBack)
	, m_BkColorTransparent(false)
	, m_Gravity()
	, m_TextX(x)
	, m_TextY(y)
	, m_FontHandle(FontHandle)
	, m_FitBoundMode(true)
{
	FitBound();
}

ScLabel::ScLabel(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle)
	: ScreenComponent(bound)
	, m_Text(Text)
	, m_ColorFore(ColorFore)
	, m_ColorBack(ColorBack)
	, m_BkColorTransparent(false)
	, m_Gravity(gravity)
	, m_TextX()
	, m_TextY()
	, m_FontHandle(FontHandle)
	, m_FitBoundMode(false)
{
	RECT r = TextAlignment::Execute(m_Text, m_Bound->GetRect(), m_Gravity, m_FontHandle, 8);
	m_TextX = r.left;
	m_TextY = r.top;
}

void ScLabel::OnMouseMove(int x, int y)
{
	// Do nothing
}

void ScLabel::OnMousePress(int x, int y)
{
	// Do nothing
}

void ScLabel::Draw() const
{
	if (m_BkColorTransparent == false)
	{
		RECT r = m_Bound->GetRect();
		DrawBox(r.left, r.top, r.right, r.bottom, m_ColorBack, TRUE);
	}

	DrawStringToHandle(m_TextX, m_TextY, m_Text.c_str(), m_ColorFore, m_FontHandle);
}

void ScLabel::ChangeText(const std::string& Text)
{
	LOG_INFO("ÉâÉxÉãñºÇïœçXÅFText = %s Å® %s", m_Text.c_str(), Text.c_str());

	m_Text = Text;
	if (m_FitBoundMode)
		FitBound();
}

void ScLabel::FitBound()
{
	int TextXSize = GetDrawStringWidthToHandle(m_Text.c_str(), m_Text.size(), m_FontHandle);
	int TextYSize = GetFontSizeToHandle(m_FontHandle);
	m_Bound = std::unique_ptr<BoundRect>(new BoundRect(m_TextX, m_TextY, TextXSize, TextYSize));
}
