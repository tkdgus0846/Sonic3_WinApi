#include "pch.h"
#include "Mushroom.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"

// 70 60

Mushroom::Mushroom(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_fJumpPower = MUSHROOM_JUMP_POWER;

	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 11;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 150;
	m_Frame.repeat = true;
}

void Mushroom::Render()
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

int Mushroom::Update()
{
	return 0;
}

void Mushroom::Late_Update()
{
	if (GetCollisionWithCollider() == false)
	{
		m_Frame.iMotion = 0;
		Move_Frame();
	}	
	else
	{
		m_Frame.iFrameStart = 0;
		m_Frame.iMotion = 1;
	}
		
}

void Mushroom::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (GetCollisionWithCollider() == false)
	{
		Collision_Enter();
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		STOP_PLAY_SOUND(L"MushroomJump.wav", CHANNELID::SOUND_OBJ, 1.0f);
		player->SetArtJump(true);
		player->SetJumping(true);
		player->SetJumpPower(PLAYER_JUMP_POWER + m_fJumpPower);
		m_fJumpPower += 5.f;
		player->SetJumpTime(0.f);

	}
}

void Mushroom::Collision_Down(Collider* collider, const int& px, const int& py)
{
}

void Mushroom::Collision_Left(Collider* collider, const int& px, const int& py)
{
}

void Mushroom::Collision_Right(Collider* collider, const int& px, const int& py)
{
}
