#include "pch.h"
#include "GameObject.h"
#include "ScrollManager.h"

GameObject::GameObject() :
	m_IsInWindow(FALSE),
	m_IsDead(FALSE),
	m_CurLine(nullptr)
{
	Update_Rect();
}

GameObject::~GameObject()
{

}

void GameObject::Update_Rect()
{
	LONG& left = m_Area.left;
	LONG& top = m_Area.top;
	LONG& right = m_Area.right;
	LONG& bottom = m_Area.bottom;

	left = m_Pos.x - m_Size.x * 0.5f;
	top = m_Pos.y - m_Size.y * 0.5f;
	right = m_Pos.x + m_Size.x * 0.5f;
	bottom = m_Pos.y + m_Size.y * 0.5f;
}

void GameObject::Move_Frame()
{
	if (m_Frame.dwTime + m_Frame.dwSpeed < GetTickCount())
	{
		++m_Frame.iFrameStart;

		if (m_Frame.iFrameStart > m_Frame.iFrameEnd)
		{
			if (m_Frame.repeat == true)
			{
				m_Frame.iFrameStart = 0;
			}
			else if (m_Frame.repeat == false)
			{
				m_Frame.iFrameStart = m_Frame.iFrameEnd;
			}
				
		}
			

		m_Frame.dwTime = GetTickCount();
	}
}

bool GameObject::IsObjectInWindow()
{
	RECT rct = GetArea();
	if (rct.left > WINDOW_WIDTH || rct.right < 0 || rct.bottom < 0 || rct.top > WINDOW_HEIGHT)
	{
		return FALSE;
	}
	return TRUE;
}

bool GameObject::IsObjectInWindow_Scroll()
{
	int ScrollX = (int)SCROLLX;
	int ScrollY = (int)SCROLLY;

	RECT rct = GetArea();

	int left = rct.left + ScrollX;
	int top = rct.top + ScrollY;
	int right = rct.right + ScrollX;
	int Bottom = rct.bottom + ScrollY;

	
	if (left > WINDOW_WIDTH + 500 || right < -500 || Bottom < -500 || top > WINDOW_HEIGHT + 500)
	{
		return FALSE;
	}

	return TRUE;
}
