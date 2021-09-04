#include "ScToggle.h"

ScToggle::ScToggle(unsigned int ColorFore, unsigned int ColorBack, Bound* bound, int FontHandle,
	DelegateArgBase<bool>* Callback)
	: ScLabel("", ColorFore, ColorBack, bound, TextAlignment::Gravity::CENTER, FontHandle)
	, m_Hover()
	, m_PressCount()
	, m_Padding()
	, m_OffOn()
	, m_Callback(Callback)
{
	RECT r = m_Bound->GetRect();
	int TextXSize = GetDrawStringWidthToHandle("OFF", 4, m_FontHandle);
	m_TextX = r.right - TextXSize;
	m_Padding = (r.right - r.left) / 12;
}

void ScToggle::OnMouseMove(int x, int y)
{
	m_Hover = m_Bound->IsIn(x, y);

	if (m_Hover && m_PressCount > 0)
	{
		m_OffOn = !m_OffOn;
		(*m_Callback)(m_OffOn);
	}

	m_PressCount = 0;
}

void ScToggle::OnMousePress(int x, int y)
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

void ScToggle::Draw() const
{
	RECT r = m_Bound->GetRect();
	DrawBox(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1, m_ColorBack, TRUE);

	int XSizeQuarter = (r.right - r.left) / 4;
	int XSizeHalf = (r.right - r.left) / 2;

	if (m_PressCount)
	{
		DrawBox(r.left + XSizeQuarter, r.top, r.right - XSizeQuarter, r.bottom, m_ColorFore, TRUE);
	}
	else
	{
		if (m_OffOn)
		{
			DrawBox(r.left, r.top, r.right - XSizeHalf, r.bottom, m_ColorFore, TRUE);
			DrawStringToHandle(r.left + m_Padding, m_TextY, "ON", m_ColorBack, m_FontHandle);
		}
		else
		{
			DrawBox(r.left + XSizeHalf, r.top, r.right, r.bottom, m_ColorFore, TRUE);
			DrawStringToHandle(m_TextX - m_Padding, m_TextY, "OFF", m_ColorBack, m_FontHandle);
		}
	}
}
