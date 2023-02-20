#include "pch.h"
#include "DropRing.h"
#include "../Client/Player.h"
#include "SoundManager.h"
#include "Collider.h"

DropRing::DropRing(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, float jumpPower, float speed, const Vec2& randDir, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 15;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 1;
	m_Frame.repeat = true;

	// 16 16
	m_ShineFrame.iFrameStart = 0;
	m_ShineFrame.iFrameEnd = 3;
	m_ShineFrame.iMotion = 0;
	m_ShineFrame.dwSpeed = 150;
	m_ShineFrame.repeat = true;

	m_bShineFrameEnabled = false;
	m_ShineTime = 500;

	m_fPower = jumpPower;
	m_Dir = randDir;

	m_fJumpTime = 0.f;
	m_bJumping = true;

	m_fSpeed = speed;

	COLLISION->NewCollider(this, "DropRingCollider", { 0,0 }, m_Size);
	m_BlinkTime = 120;
	m_BlinkStartTime = GetTickCount();
	m_bBlink = false;
	
}

DropRing::~DropRing()
{
	COLLISION->RemoveCollider(GetComponent("DropRingCollider"));
}

void DropRing::Render()
{
	//cout << m_bBlink << " "<<m_BlinkStartTime<< " "<<m_BlinkTime << endl;
	if (m_bShineFrameEnabled == false)
	{
		if (m_bBlink == false)
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
				m_FrameKeyArr,
				m_Frame,
				m_Area.left,
				m_Area.top,
				0,
				0,
				m_RenderSize.x,
				m_RenderSize.x,
				RGB(255, 0, 255),
				0, 0
			);
		}
		
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

int DropRing::Update()
{
	if (m_bShineFrameEnabled == true)
	{
		if (m_ShineStartTime + m_ShineTime <= GetTickCount())
		{
			SetDead();
		}
	}
	else
	{
		if (m_BlinkStartTime + m_BlinkTime <= GetTickCount())
		{

			m_BlinkTime -= 2;
			m_BlinkStartTime = GetTickCount();
			if (m_bBlink == true) m_bBlink = false;
			else m_bBlink = true;

			if (m_BlinkTime < 0) SetDead();
		}
	}
	bool dead = ImageObject::Update();
	if (dead) return OBJ_DEAD;

	/*cout << m_Pos.x << " " << m_Pos.y << " " << m_fFallingSpeed << " " << m_bFalling << endl;*/
	m_Pos = m_Pos + (m_Dir * m_fSpeed);

	Update_Rect();

	return OBJ_NOEVENT;
}

void DropRing::Late_Update()
{
	if (m_bShineFrameEnabled == false)
	{
		Move_Frame();
	}	
	else
		Move_Shine_Frame();

	Jump();
	
}

void DropRing::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (m_bShineFrameEnabled == true) return;
	if (GetCollisionWithCollider() == false)
	{
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		STOP_PLAY_SOUND(L"GainRing.wav", CHANNELID::SOUND_OBJ, 0.6f);
		player->AddRings(1);
		player->AddScore(100);

		m_bShineFrameEnabled = true;

		m_ShineFrame.dwTime = GetTickCount();
		m_ShineStartTime = GetTickCount();
		
	}
}

void DropRing::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void DropRing::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void DropRing::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void DropRing::Jump()
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

void DropRing::Move_Shine_Frame()
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
