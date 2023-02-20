#include "pch.h"
#include "Tile.h"
#include "ScrollManager.h"

Tile::Tile(const Vec2& pos, const Vec2& size) :
	m_Pos(pos),
	m_Size(size)
{
	m_Area.left = m_Pos.x - m_Size.x * 0.5f;
	m_Area.top = m_Pos.y - m_Size.y * 0.5f;
	m_Area.right = m_Pos.x + m_Size.x * 0.5f;
	m_Area.bottom = m_Pos.y + m_Size.y * 0.5f;
}

Tile::Tile(const RECT& rect, const Vec2& size)
{
	m_Area = rect;
	m_Size = size;
	m_Pos.x = m_Area.left + (size.x * 0.5f);
	m_Pos.y = m_Area.top + (size.y * 0.5f);
}

Tile::~Tile()
{
}

void Tile::Render()
{
	int		scrollX = (int)SCROLL->GetScrollX();
	int		ScrollY = (int)SCROLL->GetScrollY();
	RENDER->SetPen(RGB(255, 0, 0));
	RENDER->SetBrush(RGB(255, 255, 255));
	RENDER->DrawRect
	(
		m_Area.left + scrollX, 
		m_Area.top + ScrollY,
		m_Area.right + scrollX, 
		m_Area.bottom + ScrollY
	);
}

void Tile::Update()
{
}
