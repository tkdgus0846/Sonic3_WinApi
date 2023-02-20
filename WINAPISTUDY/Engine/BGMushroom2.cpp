#include "pch.h"
#include "BGMushroom2.h"

// 16 27

BGMushroom2::BGMushroom2(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 13;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 170;
	m_Frame.repeat = true;
}

void BGMushroom2::Render()
{
	RENDER->Animation_TransparentBlt
	(
		m_FrameKeyArr,
		m_Frame,
		m_Area.left,
		m_Area.top,
		m_Size.x,
		m_Size.y,
		m_RenderSize.x,
		m_RenderSize.y,
		RGB(255, 0, 255),
		0, 0
	);
}

void BGMushroom2::Late_Update()
{
	Move_Frame();
}
