#include "pch.h"
#include "Ring.h"
#include "Creature.h"
#include "Collider.h"
#include "../Client/Player.h"
#include "SoundManager.h"

// 16,16

Ring::Ring(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode, bool autoScroll) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 15;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 20;
	m_Frame.repeat = true;
	m_bAutoScroll = autoScroll;

	// 16 16
	m_ShineFrame.iFrameStart = 0;
	m_ShineFrame.iFrameEnd = 3;
	m_ShineFrame.iMotion = 0;
	m_ShineFrame.dwSpeed = 150;
	m_ShineFrame.repeat = true;

	m_bShineFrameEnabled = false;
	m_ShineTime = 500;
}

void Ring::Render()
{
	if (m_bShineFrameEnabled == false)
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
	else
	{
		RENDER->Animation_TransparentBlt
		(
			L"ShineEffect",
			m_ShineFrame,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			16,
			16,
			RGB(255, 0, 255),
			0, 0
		);
	}
	
}

int Ring::Update()
{
	if (m_bShineFrameEnabled == true)
	{
		if (m_ShineStartTime + m_ShineTime <= GetTickCount())
		{
			SetDead();
		}
	}
	bool dead = ImageObject::Update();
	if (dead) return OBJ_DEAD;

	if (m_bAutoScroll)
	{
		m_Pos.x -= 5.f;
		if (IsObjectInWindow_Scroll() == false) SetDead();
	}
	if (GetDead()) return OBJ_DEAD;

	
	Update_Rect();
	return OBJ_NOEVENT;
}

void Ring::Late_Update()
{
	if (m_bShineFrameEnabled == false)
		Move_Frame();
	else
		Move_Shine_Frame();


}

void Ring::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (m_bShineFrameEnabled == true) return;
	if (GetCollisionWithCollider() == false)
	{
		
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		if (player->GetMarioMode() == false)
		{
			STOP_PLAY_SOUND(L"GainRing.wav", CHANNELID::SOUND_OBJ, 0.6f);
		}
		else
		{
			STOP_PLAY_SOUND(L"MarioCoin.wav", CHANNELID::SOUND_OBJ, 1.0f);
		}
			
		player->AddRings(1);
		player->AddScore(100);
		
		m_bShineFrameEnabled = true;

		m_ShineFrame.dwTime = GetTickCount();
		m_ShineStartTime = GetTickCount();
	}
}

void Ring::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Ring::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Ring::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Ring::Move_Shine_Frame()
{
	if (m_ShineFrame.dwTime + m_ShineFrame.dwSpeed < GetTickCount())
	{
		++m_ShineFrame.iFrameStart;

		if (m_ShineFrame.iFrameStart > m_ShineFrame.iFrameEnd)
		{
			if (m_ShineFrame.repeat == true)
			{
				m_ShineFrame.iFrameStart = 0;
			}
			else if (m_ShineFrame.repeat == false)
			{
				m_ShineFrame.iFrameStart = m_Frame.iFrameEnd;
			}

		}


		m_ShineFrame.dwTime = GetTickCount();
	}
}
