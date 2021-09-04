#pragma once

#include "Bound.h"

class BoundRect : public Bound
{
public:
	BoundRect();
	BoundRect(int x, int y, int XSize, int YSize);

public:
	RECT GetRect() const override;
	bool IsIn(int x, int y) const override;

private:
	int m_X;
	int m_Y;
	int m_XSize;
	int m_YSize;
};

inline BoundRect::BoundRect()
	: BoundRect(0, 0, 0, 0)
{
}

inline BoundRect::BoundRect(int x, int y, int XSize, int YSize)
	: m_X(x)
	, m_Y(y)
	, m_XSize(XSize)
	, m_YSize(YSize)
{
}

inline RECT BoundRect::GetRect() const
{
	return {m_X, m_Y, m_X + m_XSize, m_Y + m_YSize};
}

inline bool BoundRect::IsIn(int x, int y) const
{
	return m_X <= x && x < (m_X + m_XSize) && m_Y <= y && y < (m_Y + m_YSize);
}
