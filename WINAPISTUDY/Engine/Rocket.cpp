#include "pch.h"
#include "Rocket.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"
#include "ScrollManager.h"
#include "../Client/DoomsDay.h"

Rocket::Rocket(int ZOrder, const Vec2& Pos, const Vec2& Dir, bool guided, Creature* target) :
	ImageObject(ZOrder, L"", 0, 0, Pos, { 0,0 }, { 0,0 }, true, 0)
{
	m_bDestroyed = false;
	m_bGuided = guided;
	m_Dir = Dir;
	m_Dir.Normalize();
	m_fSpeed = 5.f;

	m_pTarget = target;
	STOP_PLAY_SOUND(L"RocketEject.wav", CHANNELID::SOUND_EJECT, 1.0f);
	m_FlameKeyIndex = 0;

	m_FlameOffsetArr[0] = { 120,4 };
	m_FlameOffsetArr[1] = { 87,87 };
	m_FlameOffsetArr[2] = { 4,120 };
	m_FlameOffsetArr[3] = { -17,86 };
	m_FlameOffsetArr[4] = { -35,4 };
	m_FlameOffsetArr[5] = { -18,-18 };
	m_FlameOffsetArr[6] = { 4,-32 };
	m_FlameOffsetArr[7] = { 88,-20 };

	m_ColOffset[0] = { { 10, 20 }, { 30,30 } };
	m_ColOffset[1] = { { 15, 15 }, { 30,30 } };
	m_ColOffset[2] = { { 20, 10 }, { 30,30 } };
	m_ColOffset[3] = { { 93, 10 }, { 30,30 } };
	m_ColOffset[4] = { { 110, 20 }, { 30,30 } };
	m_ColOffset[5] = { { 92, 92 }, { 30,30 } };
	m_ColOffset[6] = { { 20, 110 }, { 30,30 } };
	m_ColOffset[7] = { { 15, 90 }, { 30,30 } };

	m_FlameKeyArr[0] = L"RightFlame";
	m_FlameKeyArr[1] = L"RightDownFlame";
	m_FlameKeyArr[2] = L"DownFlame";
	m_FlameKeyArr[3] = L"LeftDownFlame";
	m_FlameKeyArr[4] = L"LeftFlame";
	m_FlameKeyArr[5] = L"LeftUpFlame";
	m_FlameKeyArr[6] = L"UpFlame";
	m_FlameKeyArr[7] = L"RightUpFlame";

	m_ChangeFlameKeyTime = GetTickCount();
	m_bRenderFlame = false;
	COLLISION->NewCollider(this, "RocketCollider", m_ColOffset[CO_L].first, m_ColOffset[CO_D].second);
	/*m_Size = { 128,128 };
	Update_Rect();*/
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 5;
	m_Frame.iMotion = 0;
	m_Frame.dwSpeed = 100;
	m_Frame.repeat = false;
}

Rocket::~Rocket()
{
	COLLISION->RemoveCollider(GetComponent("RocketCollider"));
}

void Rocket::Render()
{
	if (m_bDestroyed)
	{
		Collider* collider = dynamic_cast<Collider*>(GetComponent("RocketCollider"));
		if (collider)
		{
			RENDER->Animation_TransparentBlt
			(L"BoomEffect",
				m_Frame,
				collider->GetPos().x, collider->GetPos().y,
				24 * 2.5, 24 * 2.5,
				24, 24,
				RGB(255,0,255),
				0, 0
			);
		}
			
		return;
	}
	RENDER->TransparentImage
	(
		L"Rocket2",
		m_Area.left, m_Area.top,
		m_RocketInfo.Size.x * 2.5, m_RocketInfo.Size.y * 2.5,
		m_RocketInfo.RenderPos.x, m_RocketInfo.RenderPos.y,
		m_RocketInfo.Size.x, m_RocketInfo.Size.y,
		RGB(255, 0, 255)
	);


	if (m_bRenderFlame)
	{
		RENDER->TransparentImage
		(
			m_FlameKeyArr[m_FlameKeyIndex],
			m_Area.left + m_FlameOffsetArr[m_FlameKeyIndex].x, 
			m_Area.top + m_FlameOffsetArr[m_FlameKeyIndex].y,
			16 * 2, 16 * 2,
			0, 0,
			16, 16,
			RGB(255, 0, 255)
		);
	}	
}

int Rocket::Update()
{
	bool dead = ImageObject::Update();
	if (dead == OBJ_DEAD) return OBJ_DEAD;

	RocketOutScreen();
	if (m_Frame.iFrameStart == 6) SetDead();
	if (GetDead() == true) return OBJ_DEAD;

	if (m_bDestroyed == false)
		m_Pos = m_Pos + (m_Dir * m_fSpeed);

	//Update_Rect();
	return OBJ_NOEVENT;
}

void Rocket::Late_Update()
{
	if (m_bDestroyed == false)
	{
		if (m_pTarget != nullptr)
		{
			Change_Direction_ByTarget();
		}
		Change_Flame();
		Change_Info_ByDirection();
		ColliderOffset();
		Update_Rect();
	}
	

	if (m_bDestroyed)
		Move_Explosion_Frame();
}

void Rocket::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (m_bDestroyed == true) return;
	if (GetCollisionWithCollider() == false)
	{
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Collision_Enter();
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr)
		{
			DoomsDay* boss = dynamic_cast<DoomsDay*>(collider->GetOwner());
			if (boss)
			{
				cout << collider->GetPos().x << " " << collider->GetPos().y << endl;
				boss->GetDamaged(1);
				STOP_PLAY_SOUND(L"BossHit.wav", CHANNELID::SOUND_OBJ, 1.0f);
				m_bDestroyed = true;
				//SetDead();
				m_Frame.dwTime = GetTickCount();
				return;
			}
			return;
		}

		bool hit = player->GetDamaged(1);



		if (hit)
		{
			STOP_PLAY_SOUND(L"RocketHit.wav", CHANNELID::SOUND_OBJ, 1.0f);
		}
		m_bDestroyed = true;
		m_Frame.dwTime = GetTickCount();
		//SetDead();
	}
}

void Rocket::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Rocket::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Rocket::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Rocket::Hit_With_SkillRing()
{
	STOP_PLAY_SOUND(L"RocketHit.wav", CHANNELID::SOUND_OBJ, 1.0f);
	m_bDestroyed = true;
	m_Frame.dwTime = GetTickCount();
}

void Rocket::Change_Direction_ByTarget()
{
	if (m_pTarget == nullptr) return;

	Collider* collider = dynamic_cast<Collider*>(GetComponent("RocketCollider"));
	if (collider == nullptr) return;

	Vec2 myPos = collider->GetPos();
	Vec2 Dir = m_pTarget->GetPos() - myPos;

	//m_Dir = Dir;
	float angle = Dir.GetAngle();

	if (Dir.x > 0)
	{
		if (angle < 22.f && angle > -22.f)
		{
			//cout << "+ 오른쪽!" << endl;
			m_Dir = { 1,0 };

		}
		else if (angle < -22.f && angle > -67.f)
		{
			//cout << "+ 오른쪽위!" << endl;
			m_Dir = { 1,-1 };
		}
		else if (angle > 22.f && angle < 67.f)
		{
			//cout << "+ 오른쪽 아래!" << endl;
			m_Dir = { 1,1 };
		}
		else if (angle > 67.f && angle < 90.f)
		{
			//cout << "+ 아래!" << endl;
			m_Dir = { 0,1 };
		}
		else if (angle < -67.f && angle > -90.f)
		{
			m_Dir = { 0,-1 };
		}
	}
	else
	{
		if (angle < 22.f && angle > -22.f)
		{
			//cout << "- 왼쪽!" << endl;
			m_Dir = { -1,0 };
		}
		else if (angle < -22.f && angle > -67.f)
		{
			//cout << "- 왼쪽 아래!" << endl;
			m_Dir = { -1,1 };


		}
		else if (angle > 22.f && angle < 67.f)
		{
			//cout << "- 왼쪽 위!" << endl;
			m_Dir = { -1,-1 };


		}
		else if (angle > 67.f && angle < 90.f)
		{
			//cout << "- 위!" << endl;
			m_Dir = { 0,-1 };


		}
		else if (angle < -67.f && angle > -90.f)
		{
			//cout << "- 아래!" << endl;
			m_Dir = { 0,1 };
		}

	}
	m_Dir.Normalize();
}

void Rocket::Change_Info_ByDirection()
{
	// 왼쪽
	if (m_Dir.x < 0 && m_Dir.y == 0)
	{
		m_RocketInfo.RenderPos = { 0,56 };
		m_RocketInfo.Size = { 48, 16 };
		m_FlameKeyIndex = 0;
	}
	// 왼위
	else if (m_Dir.x < 0 && m_Dir.y < 0)
	{
		m_RocketInfo.RenderPos = { 8,8 };
		m_RocketInfo.Size = { 40, 40 };
		m_FlameKeyIndex = 1;
	}
	// 위
	else if (m_Dir.x == 0 && m_Dir.y < 0)
	{
		m_RocketInfo.RenderPos = { 56,0 };
		m_RocketInfo.Size = { 16, 48 };
		m_FlameKeyIndex = 2;
	}
	// 우위
	else if (m_Dir.x > 0 && m_Dir.y < 0)
	{
		m_RocketInfo.RenderPos = { 80,8 };
		m_RocketInfo.Size = { 40, 40 };
		m_FlameKeyIndex = 3;
	}
	// 우
	else if (m_Dir.x > 0 && m_Dir.y == 0)
	{
		m_RocketInfo.RenderPos = { 80,56 };
		m_RocketInfo.Size = { 48, 16 };
		m_FlameKeyIndex = 4;
	}
	// 우하
	else if (m_Dir.x > 0 && m_Dir.y > 0)
	{
		m_RocketInfo.RenderPos = { 80,80 };
		m_RocketInfo.Size = { 40, 40 };
		m_FlameKeyIndex = 5;
	}
	// 하
	else if (m_Dir.x == 0 && m_Dir.y > 0)
	{
		m_RocketInfo.RenderPos = { 56,80 };
		m_RocketInfo.Size = { 16, 48 };
		m_FlameKeyIndex = 6;
	}
	// 왼하
	else if (m_Dir.x < 0 && m_Dir.y > 0)
	{
		m_RocketInfo.RenderPos = { 8,80 };
		m_RocketInfo.Size = { 40, 40 };
		m_FlameKeyIndex = 7;
	}
	
}

void Rocket::Change_Flame()
{
	if (m_ChangeFlameKeyTime + 50 <= GetTickCount())
	{
		if (m_bRenderFlame)
			m_bRenderFlame = false;
		else
			m_bRenderFlame = true;
		
		m_ChangeFlameKeyTime = GetTickCount();
	}
}

void Rocket::ColliderOffset()
{
	Collider* collider = dynamic_cast<Collider*>(GetComponent("RocketCollider"));
	if (collider)
	{
		collider->SetOffset(m_ColOffset[m_FlameKeyIndex]);
	}
}

void Rocket::Move_Explosion_Frame()
{
	if (m_Frame.dwTime + m_Frame.dwSpeed < GetTickCount())
	{
		++m_Frame.iFrameStart;

		if (m_Frame.iFrameStart == m_Frame.iFrameEnd + 1)
		{
			return;
		}
		m_Frame.dwTime = GetTickCount();
	}
}

void Rocket::RocketOutScreen()
{
	int      ScrollX = (int)SCROLLX;
	int		 ScrollY = (int)SCROLLY;
	//cout << m_Pos.x - ScrollX << endl;
	if (m_Pos.x + ScrollX < -300)
	{
		SetDead();
		return;
	}

	if (m_Pos.y + ScrollY < -300 || m_Pos.y + ScrollY > 1100)
	{
		SetDead();
		return;
	}
}
