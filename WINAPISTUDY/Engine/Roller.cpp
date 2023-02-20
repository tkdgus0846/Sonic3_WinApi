#include "pch.h"
#include "Roller.h"
#include "Creature.h"
#include "Collider.h"
#include "SoundManager.h"

// 66 13
Roller::Roller(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) : 
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos,Size, RenderSize, collisionOn, splitMode)
{
	m_eObstacleType = OT_ROLLER;
	m_RollerAddSpeed = 20.f;
}

void Roller::Render()
{
	RENDER->TransparentImage
	(
		m_FrameKeyArr,
		m_Area.left,
		m_Area.top,
		m_Size.x,
		m_Size.y,
		0.f,
		0.f,
		m_RenderSize.x,
		m_RenderSize.y,
		RGB(255, 0, 255)
	);
}

void Roller::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (GetCollisionWithCollider() == false)
	{
		Collision_Enter();
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
		if (player == nullptr) return;
		STOP_PLAY_SOUND(L"Rolling.wav", CHANNELID::SOUND_OBJ, 1.f);
		player->AddSpeed(m_RollerAddSpeed);	
	}
}

void Roller::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Roller::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Roller::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}
