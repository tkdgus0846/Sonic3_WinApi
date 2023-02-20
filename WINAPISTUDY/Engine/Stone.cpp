#include "pch.h"
#include "Stone.h"
#include "Collider.h"
#include "../Client/Player.h"
#include "SoundManager.h"

Stone::Stone(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 3;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 50;
	m_Frame.repeat = true;

	m_RandSpeed = rand() % 40;
	int randX = rand() % 20 - 20;
	int randY = rand() % 5 - 2;

	m_Dir.x = randX;
	m_Dir.y = randY;
	m_Dir.Normalize();
	m_fPower = 12.f;
	m_bTouched = false;

	Update_Rect();
}

void Stone::Render()
{
	if (m_bTouched == false)
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
	else
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
}

int Stone::Update()
{
	if (m_bTouched)
	{
		
		m_Pos = m_Pos + (m_Dir * m_RandSpeed);
		if (IsObjectInWindow_Scroll() == false)
		{
			SetDead();
		}
	}

	bool dead = ImageObject::Update();
	if (dead) return OBJ_DEAD;

	

	Update_Rect();
}

void Stone::Jump()
{
	if (m_bJumping)
	{
		m_fFallingSpeed = m_fPower * m_fJumpTime - (6.8f * m_fJumpTime * m_fJumpTime) * 0.5f;

		if (m_fFallingSpeed < 0.f)
		{
			m_bFalling = true;
			//m_Dir = { 0,-1 };
		}


		if (m_fFallingSpeed <= -15.f) m_fFallingSpeed = -15.f;
		m_Pos.y -= m_fFallingSpeed;
		//m_Pos = m_Pos + m_Dir * m_fFallingSpeed;
		m_fJumpTime += 0.2f;
	}
	else
	{
		m_fFallingSpeed = -10.f;
		m_Pos.y -= m_fFallingSpeed;

		m_bFalling = true;
	}
}

void Stone::Late_Update()
{
	if (m_bTouched == true)
	{
		Move_Frame();
		Jump();
	}
	
}

void Stone::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (m_bTouched == true) return;
	if (GetCollisionWithCollider() == false)
	{
		Collision_Enter();
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		if (player->GetCurState() == PS_JUMP)
		{
			m_bTouched = true;

			if (IS_PLAYING(CHANNELID::SOUND_OBJ) == false)
				STOP_PLAY_SOUND(L"DestroyStone.wav", CHANNELID::SOUND_OBJ, 1.2f);

			m_fJumpTime = 0.f;
			m_bJumping = true;
		}
		else
		{
			Vec2 playerPos = player->GetPos();
			player->SetCanPush(true);
			player->SetAttachToSide(true);
			player->SetSpeed(0);
			player->SetPos(Vec2(playerPos.x + px + 8.f, playerPos.y));
		}

		
		
	}
}

void Stone::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Stone::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Stone::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}
