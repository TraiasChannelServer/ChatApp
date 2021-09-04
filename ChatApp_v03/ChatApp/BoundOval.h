#pragma once

#include "Bound.h"

class BoundOval : public Bound
{
public:
	BoundOval();
	BoundOval(int x, int y, int XRadius, int YRadius);

public:
	RECT GetRect() const override;
	bool IsIn(int x, int y) const override;

private:
	int m_X;
	int m_Y;
	int m_XRadius;
	int m_YRadius;
};

inline BoundOval::BoundOval()
	: BoundOval(0, 0, 0, 0)
{
}

inline BoundOval::BoundOval(int x, int y, int XRadius, int YRadius)
	: m_X(x)
	, m_Y(y)
	, m_XRadius(XRadius)
	, m_YRadius(YRadius)
{
}

inline RECT BoundOval::GetRect() const
{
	return {m_X - m_XRadius, m_Y - m_YRadius, m_X + m_XRadius, m_Y + m_YRadius};
}

inline bool BoundOval::IsIn(int x, int y) const
{
	int nx = x - m_X;
	int ny = y - m_Y;
	return ((nx * nx) / (m_XRadius * m_XRadius)) + ((ny * ny) / (m_YRadius * m_YRadius)) <= 1;
}
