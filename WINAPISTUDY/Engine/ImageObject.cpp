#include "pch.h"
#include "ImageObject.h"
#include "ScrollManager.h"
#include "Collider.h"
#include "Creature.h"

ImageObject::ImageObject(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode)
{
	if (collisionOn == false)
	{
		m_ObjID = OBJ_IMAGE;
		m_eObstacleType = OT_END;
	}	
	else
	{
		m_ObjID = OBJ_OBSTACLE;	
	}
		
	m_bCollisionWithCollider = false;
	SetZOrder(ZOrder);

	lstrcpy(m_FrameKeyArr, frameKey);

	/*for (int i = 0; ; i++)
	{
		m_FrameKeyArr[i] = frameKey[i];
		if (m_FrameKeyArr[i] == L'\0') break;
	}*/
	m_CoordX = coordX;
	m_CoordY = coordY;
	m_Pos = Pos;
	m_Size = Size;
	m_RenderSize = RenderSize;
	m_bCollisionOn = collisionOn;
	m_SplitMode = splitMode;

	Update_Rect();
}

void ImageObject::Render()
{
	if (m_SplitMode == 0)
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			(m_CoordX * 128.f),
			(m_CoordY * 128.f),
			m_RenderSize.x,
			m_RenderSize.y,
			RGB(255, 0, 255)
		);
	}
	else if (m_SplitMode == 1)
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			(m_CoordX * 128.f),
			(m_CoordY * 128.f),
			m_RenderSize.x,
			m_RenderSize.y,
			RGB(255, 0, 255)
		);
	}
	else if (m_SplitMode == 2)
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			(m_CoordX * 128.f + m_RenderSize.x),
			(m_CoordY * 128.f),
			m_RenderSize.x,
			m_RenderSize.y,
			RGB(255, 0, 255)
		);
	}
	else if (m_SplitMode == 3)
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			(m_CoordX * 128.f),
			(m_CoordY * 128.f + m_RenderSize.y),
			m_RenderSize.x,
			m_RenderSize.y,
			RGB(255, 0, 255)
		);
	}
	else if (m_SplitMode == 4)
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			(m_CoordX * 128.f + m_RenderSize.x),
			(m_CoordY * 128.f + m_RenderSize.y),
			m_RenderSize.x,
			m_RenderSize.y,
			RGB(255, 0, 255)
		);
	}
	
}

void ImageObject::Init()
{
}

int ImageObject::Update()
{
	if (GetDead()) return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void ImageObject::Late_Update()
{
}

void ImageObject::SelectFrameKey()
{
}

void ImageObject::Collision_Up(Collider* collider, const int& px, const int& py)
{
	

}

void ImageObject::Collision_Down(Collider* collider, const int& px, const int& py)
{
	

}

void ImageObject::Collision_Left(Collider* collider, const int& px, const int& py)
{
	

}

void ImageObject::Collision_Right(Collider* collider, const int& px, const int& py)
{
	

}

