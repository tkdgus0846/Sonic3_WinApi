#include "pch.h"
#include "Line.h"
#include "ScrollManager.h"

Line::Line(const Vec2& p1, const Vec2& p2, LINELAYER layer, int LineNum)
{
	m_eLayer = layer;
	m_LineNum = LineNum;

	if (p1.x < p2.x)
	{
		point[LINE_LEFT] = p1;
		point[LINE_RIGHT] = p2;
	}
	else
	{
		point[LINE_LEFT] = p2;
		point[LINE_RIGHT] = p1;
	}

	if (p1.y < p2.y)
	{
		point[LINE_TOP] = p1;
		point[LINE_BOTTOM] = p2;
	}
	else
	{
		point[LINE_TOP] = p2;
		point[LINE_BOTTOM] = p1;
	}

	float DeltaX = (GetPoint(LINE_RIGHT).x - GetPoint(LINE_LEFT).x);
	float DeltaY = (GetPoint(LINE_RIGHT).y - GetPoint(LINE_LEFT).y);


	m_fIncline = DeltaY / DeltaX;
	m_fRadian = atanf(m_fIncline);
	m_fAngle = m_fRadian * 180.f / PI;
	m_Direction = GetPoint(LINE_RIGHT) - GetPoint(LINE_LEFT);
	m_Direction.Normalize();
}

void Line::Render()
{
	int scrollX = (int)SCROLL->GetScrollX();
	int scrollY = (int)SCROLL->GetScrollY();

	if (m_eLayer == LL_FORWARD)
		RENDER->SetPen(RGB(255, 255, 255));
	else if (m_eLayer == LL_REVERSE)
		RENDER->SetPen(RGB(255, 0, 0));

	RENDER->DrawLine
	(
		point[LINE_LEFT].x + scrollX,
		point[LINE_LEFT].y + scrollY,
		point[LINE_RIGHT].x + scrollX,
		point[LINE_RIGHT].y + scrollY
	);

	RENDER->Text
	(
		point[LINE_LEFT].x + (point[LINE_RIGHT].x - point[LINE_LEFT].x)*0.5f + scrollX,
		point[LINE_TOP].y + (point[LINE_BOTTOM].y - point[LINE_TOP].y)*0.5f + scrollY,
		to_wstring(m_LineNum)
	);
}

void Line::Update()
{
}

float Line::GetLineIncline()
{
	return m_fIncline;
}

float Line::GetRadian()
{
	return m_fRadian;
}

float Line::GetAngle()
{
	return m_fAngle;
}

Vec2 Line::GetDirection()
{
	return m_Direction;
}
