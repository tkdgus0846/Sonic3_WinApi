#include "pch.h"
#include "BGMushroom1.h"

// 72 40

BGMushroom1::BGMushroom1(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	
	m_RandNum = rand() % 2;
}

void BGMushroom1::Render()
{
	RENDER->TransparentImage
	(
		m_FrameKeyArr,
		m_Area.left,
		m_Area.top,
		m_Size.x,
		m_Size.y,
		(m_RandNum * 72.f),
		(m_CoordY * 0.f),
		m_RenderSize.x,
		m_RenderSize.y,
		RGB(255, 0, 255)
	);
}
