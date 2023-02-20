#include "pch.h"
#include "MarioTile.h"

MarioTile::MarioTile(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
}

void MarioTile::Render()
{
	RENDER->TransparentImage
	(
		m_FrameKeyArr,
		m_Area.left,
		m_Area.top,
		m_Size.x,
		m_Size.y,
		(m_CoordX * 31.f),
		(m_CoordY * 0.f),
		m_RenderSize.x,
		m_RenderSize.y,
		RGB(255, 0, 255)
	);
}

void MarioTile::Collision_Up(Collider* collider, const int& px, const int& py)
{
}

void MarioTile::Collision_Down(Collider* collider, const int& px, const int& py)
{
}

void MarioTile::Collision_Left(Collider* collider, const int& px, const int& py)
{
}

void MarioTile::Collision_Right(Collider* collider, const int& px, const int& py)
{
}
