#include "pch.h"
#include "Collider.h"
#include "ScrollManager.h"

Collider::Collider() :
	m_Color(RGB(0x80, 0xff, 0)),
	m_InCollision(FALSE)
{
}

Collider::Collider(string _Name, const Vec2& offsetPos, const Vec2& size) :
	m_Color(RGB(0x80, 0xff, 0)),
	m_InCollision(FALSE),
	m_OffsetPos(offsetPos),
	m_Size(size)
{
	SetName(_Name);
	
}

void Collider::Update()
{
	Update_Rect();
	
	/*if (GetOwner())
		RECT rct = GetOwner()->GetArea();*/
		
}

void Collider::Render()
{
	bool IsActive = GetActive();
	float      ScrollX = (int)SCROLL->GetScrollX();
	float      ScrollY = (int)SCROLL->GetScrollY();

	if (IsActive == false)
	{
		RENDER->SetPen(RGB(0, 255, 0), PENTYPE::Solid, 2);
		RENDER->SetBrush(RGB(1, 2, 3), BRUSHTYPE::Null);
		RENDER->DrawRect
		(
			m_Area.left + ScrollX,
			m_Area.top + ScrollY,
			m_Area.right + ScrollX,
			m_Area.bottom + ScrollY
		);
	}	
}


void Collider::OnCollisionEnter(Collider* Other)
{
	/*if (Other)
	{
		m_Color = RGB(255, 0, 0);
		m_InCollision = TRUE;
	}
	else
	{
		m_Color = RGB(0x80, 0xff, 0);
		m_InCollision = FALSE;
	}*/
}

void Collider::OnCollisionExit()
{
}

void Collider::Update_Rect()
{
	Vec2 OwnerPos;
	if (GetOwner())
	{
		OwnerPos = GetOwner()->GetPos();

		LONG& left = m_Area.left;
		LONG& top = m_Area.top;
		LONG& right = m_Area.right;
		LONG& bottom = m_Area.bottom;

		left = OwnerPos.x - m_Size.x * 0.5f + m_OffsetPos.x;
		top = OwnerPos.y - m_Size.y * 0.5f + m_OffsetPos.y;
		right = OwnerPos.x + m_Size.x * 0.5f + m_OffsetPos.x;
		bottom = OwnerPos.y + m_Size.y * 0.5f + m_OffsetPos.y;

		m_Pos.x = left + m_Size.x * 0.5f;
		m_Pos.y = top + m_Size.y * 0.5f;
	}

	
}


