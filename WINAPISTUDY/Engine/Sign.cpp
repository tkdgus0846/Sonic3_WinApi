#include "pch.h"
#include "Sign.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"

Sign::Sign(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode)
	: ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_fPower = 13.f;

	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 8;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 50;
	m_Frame.repeat = true;

	m_ShineFrame.iFrameStart = 0;
	m_ShineFrame.iFrameEnd = 3;
	m_ShineFrame.iMotion = 0;
	m_ShineFrame.dwSpeed = 150;
	m_ShineFrame.repeat = true;

	m_bShineFrameEnabled = false;
	m_ShineTime = 500;

	COLLISION->NewCollider(this, "SignCollider", { 0,0 }, m_Size);

	m_bFalling = true;
	m_bLand = false;
	m_bActEnded = false;
}

Sign::~Sign()
{
	COLLISION->RemoveCollider(GetComponent("SignCollider"));
}

void Sign::Render()
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

	if (m_bShineFrameEnabled)
	{
		for (int i = 0; i < 12; i++)
		{
			RENDER->Animation_TransparentBlt
			(
				L"ShineEffect",
				m_ShineFrame,
				m_Area.left + randPosX[i],
				m_Area.top + randPosY[i],
				16 * 2,
				16 * 2,
				16,
				16,
				RGB(255, 0, 255),
				0, 0
			);
		}
	}
	
}

int Sign::Update()
{
	if (m_ShineStartTime + m_ShineTime <= GetTickCount())
	{
		m_bShineFrameEnabled = false;
	}
	return 0;
}

void Sign::Late_Update()
{
	if (m_bActEnded == true) return;
	Collider* collider = dynamic_cast<Collider*>(GetComponent("SignCollider"));
	if (collider)
	{
		if (m_bFalling == false && collider->GetInCollision() == true && m_bLand == false)
		{
			m_bLand = true;
			m_CollisionTimer = GetTickCount();
		}
		else
		{
			Move_Shine_Frame();

			if (m_bActEnded == false)
				Move_Frame();
		}		
	}

	if (m_bLand == true && m_bActEnded == false)
	{
		if (m_CollisionTimer + 2000 <= GetTickCount())
		{
			m_Frame.iFrameStart = 8;
			STOP_SOUND(CHANNELID::SOUND_BGM);
			STOP_PLAY_SOUND(L"ActClear.wav", CHANNELID::SOUND_EFFECT, 1.0f);
			SCENE->ActClear();
			m_bActEnded = true;
		}
		else
		{
			Move_Frame();
		}
	}
	
	Jump();
	
	Update_Rect();
}

void Sign::Jump()
{
	if (m_bJumping)
	{
		m_fFallingSpeed = m_fPower * m_fJumpTime - (6.8f * m_fJumpTime * m_fJumpTime) * 0.5f;

		if (m_fFallingSpeed < 0.f)
		{
			m_bFalling = true;
			//m_Dir = { 0,-1 };
		}


		if (m_fFallingSpeed <= -5.f) m_fFallingSpeed = -5.f;
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

void Sign::Move_Shine_Frame()
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

void Sign::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (collider == nullptr) return;
	Player* player = dynamic_cast<Player*>(collider->GetOwner());
	if (player == nullptr) return;
	if (m_bLand == true) return;
	
	if (GetCollisionWithCollider() == false)
	{
		Collision_Enter();

		STOP_PLAY_SOUND(L"SignTouch.wav", CHANNELID::SOUND_OBJ, 1.0f);
		player->AddScore(2000);

		SetJumping(true);
		SetJumpTime(0.f);

		m_bShineFrameEnabled = true;

		for (int i = 0; i < 12; i++)
		{
			randPosX[i] = rand() % 100 - 20;
			randPosY[i] = rand() % 100 - 20;
		}

		m_ShineFrame.dwTime = GetTickCount();
		m_ShineStartTime = GetTickCount();
	}
}

void Sign::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Sign::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Sign::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}
