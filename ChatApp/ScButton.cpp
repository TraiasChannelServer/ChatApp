#include "ScButton.h"
#include "BoundRect.h"
#include "DxLib.h"

ScButton::ScButton(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, int x, int y, int FontHandle,
	unsigned int ColorFrame, unsigned int ColorBackHover, unsigned int ColorBackPress, DelegateVoidBase* Callback)
	: ScLabel(Text, ColorFore, ColorBack, x, y, FontHandle)
	, m_ColorFrame(ColorFrame)
	, m_ColorBackHover(ColorBackHover)
	, m_ColorBackPress(ColorBackPress)
	, m_Hover()
	, m_PressCount()
	, m_Callback(Callback)
{
	RECT r = m_Bound->GetRect();
	m_Bound = std::unique_ptr<BoundRect>(new BoundRect(r.left - FRAME_WIDTH_PRESS, r.top - FRAME_WIDTH_PRESS, r.right - r.left + FRAME_WIDTH_PRESS * 2, r.bottom - r.top + FRAME_WIDTH_PRESS * 2));
}

ScButton::ScButton(const std::string& Text, unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle,
	unsigned int ColorFrame, unsigned int ColorBackHover, unsigned int ColorBackPress, DelegateVoidBase* Callback)
	: ScLabel(Text, ColorFore, ColorBack, bound, gravity, FontHandle)
	, m_ColorFrame(ColorFrame)
	, m_ColorBackHover(ColorBackHover)
	, m_ColorBackPress(ColorBackPress)
	, m_Hover()
	, m_PressCount()
	, m_Callback(Callback)
{
	RECT r = m_Bound->GetRect();
	m_Bound = std::unique_ptr<BoundRect>(new BoundRect(r.left - FRAME_WIDTH_PRESS, r.top - FRAME_WIDTH_PRESS, r.right - r.left + FRAME_WIDTH_PRESS * 2, r.bottom - r.top + FRAME_WIDTH_PRESS * 2));
}

void ScButton::OnMouseMove(int x, int y)
{
	m_Hover = m_Bound->IsIn(x, y);

	if (m_Hover && m_PressCount > 0)
	{
		(*m_Callback)();
	}

	m_PressCount = 0;
}

void ScButton::OnMousePress(int x, int y)
{
	if (m_Bound->IsIn(x, y) == false)
	{
		m_Hover = false;
		m_PressCount = 0;
	}
	if (m_Hover)
	{
		++m_PressCount;
	}
}

void ScButton::Draw() const
{
	RECT r = m_Bound->GetRect();
	DrawBox(r.left, r.top, r.right, r.bottom, m_ColorFrame, TRUE);

	if (m_PressCount)
	{
		DrawBox(r.left + FRAME_WIDTH_PRESS, r.top + FRAME_WIDTH_PRESS, r.right - FRAME_WIDTH_PRESS, r.bottom - FRAME_WIDTH_PRESS, m_ColorBackPress, TRUE);
		DrawStringToHandle(m_TextX + FRAME_WIDTH_PRESS, m_TextY + FRAME_WIDTH_PRESS, m_Text.c_str(), m_ColorFore, m_FontHandle);
	}
	else if (m_Hover)
	{
		DrawBox(r.left + FRAME_WIDTH_RELEASE, r.top + FRAME_WIDTH_RELEASE, r.right - FRAME_WIDTH_RELEASE, r.bottom - FRAME_WIDTH_RELEASE, m_ColorBackHover, TRUE);
		DrawStringToHandle(m_TextX, m_TextY, m_Text.c_str(), m_ColorFore, m_FontHandle);
	}
	else
	{
		DrawBox(r.left + FRAME_WIDTH_RELEASE, r.top + FRAME_WIDTH_RELEASE, r.right - FRAME_WIDTH_RELEASE, r.bottom - FRAME_WIDTH_RELEASE, m_ColorBack, TRUE);
		DrawStringToHandle(m_TextX, m_TextY, m_Text.c_str(), m_ColorFore, m_FontHandle);
	}
}

