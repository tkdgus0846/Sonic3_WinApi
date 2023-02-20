#include "pch.h"
#include "Spring.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"

Spring::Spring(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int Diagonal) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, Diagonal)
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 1;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 200;
	m_Frame.repeat = false;

	m_bActivated = false;
	m_fTimeAccel = 1.5f;
	m_fTimeSpeed = 0.8f;

	m_Diagonal = Diagonal;

	if (Diagonal == 0)
	{
		if (coordY == 0)
		{
			m_Dir = { 0,-1 };
		}
		else if (coordY == 1)
		{
			m_Dir = { -1,0 };
		}
		else if (coordY == 2)
		{
			m_Dir = { 0,1 };
		}
		else if (coordY == 3)
		{
			m_Dir = { 1,0 };
		}
	}
	else
	{
		if (coordY == 0)
		{
			m_Dir = { -1,-1 };
		}
		else if (coordY == 1)
		{
			m_Dir = { 1,-1 };
		}
		else if (coordY == 2)
		{
			m_Dir = { -1,1 };
		}
		else if (coordY == 3)
		{
			m_Dir = { 1,1 };
		}
	}
	m_Dir.Normalize();

	
	// coordX와 coordY 값으로 SetDir()을 해줘야한다.
}

void Spring::Render()
{
	RENDER->TransparentImage
	(
		m_FrameKeyArr,
		m_Area.left,
		m_Area.top,
		m_Size.x,
		m_Size.y,
		(m_CoordX * m_RenderSize.x) + (m_Frame.iFrameStart * m_RenderSize.x),
		m_CoordY * m_RenderSize.y,
		m_RenderSize.x, m_RenderSize.y,
		RGB(255,0,255)
	);
}

void Spring::Late_Update()
{
	
	Move_Spring();
}

void Spring::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (GetCollisionWithCollider() == false)
	{
		Collision_Enter();
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		STOP_PLAY_SOUND(L"Spring.wav", CHANNELID::SOUND_OBJ, 1.f);
		// 스프링이 생긴 방향대로 방향을 설정해줘야함.
		player->SetDir(m_Dir);
		// 속도는 보고 결정해야한다.
		m_bActivated = true;

		if (m_Diagonal == 0)
		{
			// 위
			if (m_CoordY == 0) 			
			{
				player->SetArtJump(true);
				player->SetJumping(true);
				player->SetJumpPower(PLAYER_JUMP_POWER + 15.f);
				player->SetJumpTime(0.f);
			}
			// 왼
			else if (m_CoordY == 1)
			{

			}
			// 아래
			else if (m_CoordY == 2)
			{
				player->SetArtJump(false);
				player->SetJumping(true);
				player->SetJumpPower(0);
				player->SetJumpTime(0.f);
				player->SetFallingSpeed(-10.f);
			}
			// 오른쪽
			else if (m_CoordY == 3)
			{

			}
		}
		else
		{
			// 왼위
			if (m_CoordY == 0)
			{
				player->SetArtJump(true);
				player->SetJumping(true);
				player->SetJumpPower(PLAYER_JUMP_POWER + 15.f);
				player->SetJumpTime(0.f);
			}
			// 우위
			else if (m_CoordY == 1)
			{
				player->SetArtJump(true);
				player->SetJumping(true);
				player->SetJumpPower(PLAYER_JUMP_POWER + 15.f);
				player->SetJumpTime(0.f);
			}
			// 왼아래
			else if (m_CoordY == 2)
			{
				player->SetArtJump(false);
				player->SetJumping(true);
				player->SetJumpPower(0);
				player->SetJumpTime(0.f);
				player->SetFallingSpeed(-10.f);
			}
			// 우아래
			else if (m_CoordY == 3)
			{
				player->SetArtJump(false);
				player->SetJumping(true);
				player->SetJumpPower(0);
				player->SetJumpTime(0.f);
				player->SetFallingSpeed(-10.f);
			}
		}


		if (m_Dir.x > 0)
		{
			player->SetObjectDir(OD_RIGHT);
			player->SetSpeed(20.f);
		}
		else if (m_Dir.x < 0)
		{
			player->SetObjectDir(OD_LEFT);
			player->SetSpeed(20.f);
		}
			
	}
}

void Spring::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Spring::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Spring::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Spring::Move_Spring()
{
	if (GetCollisionWithCollider() == true)
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
	else
	{
		
		if (m_Frame.dwTime  + 100.f < GetTickCount())
		{
			m_Frame.iFrameStart = 0;
		}
		
	}
	

	
}
