#include "pch.h"
#include "DoomsDay.h"
#include "Rocket.h"
#include "Creature.h"
#include "Scene.h"
#include "Bullet.h"
#include <ScrollManager.h>
#include <SoundManager.h>

DoomsDay::DoomsDay(const Vec2& pos, Scene* curScene, Creature* target)
{
	m_ZOrder = 1;

	m_Pos = pos;
	m_Size.x = 256 * 2;
	m_Size.y = 140 * 2;

	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 0;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 100;
	m_Frame.repeat = true;

	m_BoomEffectFrame.iFrameStart = 0;
	m_BoomEffectFrame.iFrameEnd = 5;
	m_BoomEffectFrame.iMotion = 0;
	m_BoomEffectFrame.dwSpeed = 100;
	m_BoomEffectFrame.repeat = true;

	m_DeathEffectFrame.iFrameStart = 0;
	m_DeathEffectFrame.iFrameEnd = 4;
	m_DeathEffectFrame.iMotion = 0;
	m_DeathEffectFrame.dwSpeed = 100;
	m_DeathEffectFrame.repeat = true;

	m_pTarget = target;
	m_pCurScene = curScene;

	m_MissileStandard = { 285, 130 };

	m_CoordX = 0;
	m_CoordY = 0;
	m_MissileDir = { -1,-1 };

	m_MissileTime1 = GetTickCount();
	m_MissileTime2 = GetTickCount();
	m_MissileTime3 = GetTickCount();

	m_DeathRandomTime = GetTickCount();

	m_EjectMissilePosArr[0] = { -5,30 };
	m_EjectMissilePosArr[1] = { 0,0 };
	m_EjectMissilePosArr[2] = { 20,-5 };
	m_EjectMissilePosArr[3] = { 50,5 };
	m_EjectMissilePosArr[4] = { 60,30 };
	m_EjectMissilePosArr[5] = { 50,50 };
	m_EjectMissilePosArr[6] = { 40,60 };
	m_EjectMissilePosArr[7] = { 10,60 };



	m_AngleIndex = 0;
	m_RocketTime = GetTickCount();
	m_bGenTime = true;
	m_CurRocket = nullptr;

	m_HP = 5;

	m_BlinkTime = 100;
	m_bBlink = false;
	m_bGetDamaged = false;
	m_bDeath = false;

	COLLISION->NewCollider(this, "DoomsDayCollider", { 180.f, -100.f }, { 80.f,40.f });
}
// 285 130

DoomsDay::~DoomsDay()
{
	COLLISION->RemoveCollider(GetComponent("DoomsDayCollider"));
}

// 256 140
// 미사일 발사대 32 32  간격 8
void DoomsDay::Render()
{
	if (m_bBlink == true)
	{
		RENDER->Animation_TransparentBlt
		(
			L"DoomsDay",
			m_Frame,
			m_Area.left, m_Area.top,
			m_Size.x, m_Size.y,
			0, 0,
			RGB(255, 0, 255),
			0, 8
		);
	}
	else
	{
		RENDER->Animation_TransparentBlt
		(
			L"DoomsDay",
			m_Frame,
			m_Area.left, m_Area.top,
			m_Size.x, m_Size.y,
			256, 136,
			RGB(255, 0, 255),
			0, 8
		);

		RENDER->TransparentImage
		(
			L"Missile",
			m_MissileBarrelPosArr[0].x, m_MissileBarrelPosArr[0].y,
			32 * 2.1, 32 * 2.1,
			m_CoordX * (32 + 8), m_CoordY * (32 + 8),
			32, 32,
			RGB(255, 0, 255)
		);

		RENDER->TransparentImage
		(
			L"Missile",
			m_MissileBarrelPosArr[1].x, m_MissileBarrelPosArr[1].y,
			32 * 2.1, 32 * 2.1,
			m_CoordX * (32 + 8), m_CoordY * (32 + 8),
			32, 32,
			RGB(255, 0, 255)
		);

		RENDER->TransparentImage
		(
			L"Missile",
			m_MissileBarrelPosArr[2].x, m_MissileBarrelPosArr[2].y,
			32 * 2.1, 32 * 2.1,
			m_CoordX * (32 + 8), m_CoordY * (32 + 8),
			32, 32,
			RGB(255, 0, 255)
		);
	}

	if (m_bDeath)
	{
		for (int i = 0; i < 12; i++)
		{
			if (m_DeathRandomPos[i].second)
			{
				RENDER->Animation_TransparentBlt
				(L"BoomEffect",
					m_BoomEffectFrame,
					m_Area.left + m_DeathRandomPos[i].first.x, m_Area.top + m_DeathRandomPos[i].first.y,
					24 * 2.5, 24 * 2.5,
					24, 24,
					RGB(255, 0, 255),
					0, 0
				);
			}
			else
			{
				RENDER->Animation_TransparentBlt
				(L"DeathEffect",
					m_BoomEffectFrame,
					m_Area.left + m_DeathRandomPos[i].first.x, m_Area.top + m_DeathRandomPos[i].first.y,
					32 * 2.5, 31 * 2.5,
					32, 31,
					RGB(255, 0, 255),
					0, 0
				);
			}

			
		}
		
	}
	

	

	
}

void DoomsDay::Init()
{
}

int DoomsDay::Update()
{
	DoomsDayOutScreen();
	if (GetDead())
	{
		SCENE->ActClear();
		return OBJ_DEAD;
	}

	if (m_bDeath == true)
	{
		m_Pos.y += 1.f;
		Update_Rect();
		return OBJ_NOEVENT;
	}

	if (m_bGenTime == false)
		m_Pos.x += 1.f;
	else
	{
		int scrollX = (int)SCROLLX;
		Vec2 TargetPos = { (float)(600 - scrollX), 300.f};
		Vec2 Dir = TargetPos - m_Pos;
		Dir.Normalize();
		m_Pos = m_Pos + Dir * 1.f;

		if (m_Pos.x >= 595 && m_Pos.x <= 605 && m_Pos.y >= 295 && m_Pos.y <= 305)
		{
			m_bGenTime = false;
		}
	}

	if (m_bGetDamaged == true)
	{
		if (m_BlinkStartTime + m_BlinkTime <= GetTickCount())
		{

			m_BlinkTime -= 5;
			m_BlinkStartTime = GetTickCount();
			if (m_bBlink == true) m_bBlink = false;
			else m_bBlink = true;

			//if (m_bSuperMode == true) m_Pos.x -= 10.f;

			if (m_BlinkTime < 0)
			{
				m_bBlink = false;
				m_BlinkTime = 100;
				m_bGetDamaged = false;
			}
		}
	}

	
	Update_Rect();
	return OBJ_NOEVENT;
}

void DoomsDay::Late_Update()
{
	if (m_bDeath == true)
	{
		Move_BoomEffect_Frame();
		Move_DeathEffect_Frame();
		RandomPos_Effect();
		Setting_Missile_Info();
		return;
	}
	Move_DoomsDay_Frame();
	Missile_Barrel_Select();
	Setting_Missile_Info();
	Eject_Missile();
	Eject_Rocket();
}

void DoomsDay::SelectFrameKey()
{
}

void DoomsDay::Eject_Missile()
{
	if (m_pCurScene && m_pTarget)
	{
		if (m_MissileTime1 + 2000 <= GetTickCount())
		{
			Vec2 bulletPos = m_MissileBarrelPosArr[0] + m_EjectMissilePosArr[m_AngleIndex];
			m_pCurScene->AddGameObject(OBJ_OBSTACLE, new Bullet(1, bulletPos, m_MissileDir, true, m_pTarget));
			m_MissileTime1 = GetTickCount();
		}
		if (m_MissileTime2 + 1500 <= GetTickCount())
		{
			Vec2 bulletPos = m_MissileBarrelPosArr[1] + m_EjectMissilePosArr[m_AngleIndex];
			m_pCurScene->AddGameObject(OBJ_OBSTACLE, new Bullet(1, bulletPos, m_MissileDir, true, m_pTarget));
			m_MissileTime2 = GetTickCount();
		}
		if (m_MissileTime3 + 1800 <= GetTickCount())
		{
			Vec2 bulletPos = m_MissileBarrelPosArr[2] + m_EjectMissilePosArr[m_AngleIndex];
			m_pCurScene->AddGameObject(OBJ_OBSTACLE, new Bullet(1, bulletPos, m_MissileDir, true, m_pTarget));
			m_MissileTime3 = GetTickCount();
		}
		
	}
}

void DoomsDay::Eject_Rocket()
{
	if (m_pCurScene && m_pTarget)
	{
		
		if (m_RocketTime + 5000 <= GetTickCount())
		{
			m_CurRocket = new Rocket(1, { (float)(m_Area.left + 450) , (float)(m_Area.top + 200) }, { 1,0 }, true, m_pTarget);
			m_pCurScene->AddGameObject(OBJ_OBSTACLE, m_CurRocket);
			m_RocketTime = GetTickCount();
		}
		
	}
}

void DoomsDay::Move_DoomsDay_Frame()
{
	if (m_Frame.dwTime + m_Frame.dwSpeed < GetTickCount())
	{
		++m_Frame.iMotion;

		if (m_Frame.iMotion == 2)
		{
			m_Frame.iMotion = 0;
		}

		m_Frame.dwTime = GetTickCount();
	}
}

void DoomsDay::Missile_Barrel_Select()
{
	if (m_pTarget == nullptr) return;

	float standardX = m_Area.left + m_MissileStandard.x;
	float standardY = m_Area.top + m_MissileStandard.y;
	Vec2 myPos = { standardX, standardY };

	Vec2 Dir = m_pTarget->GetPos() - myPos;
	

	float angle = Dir.GetAngle();
	
	if (Dir.x > 0)
	{
		if (angle < 22.f && angle > -22.f)
		{
			//cout << "+ 오른쪽!" << endl;
			m_CoordX = 2;
			m_CoordY = 1;
			m_MissileDir = { 1,0 };
			m_AngleIndex = 4;
			
		}
		else if (angle < -22.f && angle > -67.f)
		{
			//cout << "+ 오른쪽위!" << endl;
			m_CoordX = 2;
			m_CoordY = 0;
			m_MissileDir = { 1,-1 };
			m_AngleIndex = 3;
		}
		else if (angle > 22.f && angle < 67.f)
		{
			//cout << "+ 오른쪽 아래!" << endl;
			m_CoordX = 2;
			m_CoordY = 2;
			m_MissileDir = { 1,1 };
			m_AngleIndex = 5;
		}
		else if (angle > 67.f && angle < 90.f)
		{
			//cout << "+ 아래!" << endl;
			m_CoordX = 1;
			m_CoordY = 2;
			m_MissileDir = { 0,1 };
			m_AngleIndex = 6;
		}
		else if (angle < -67.f && angle > -90.f)
		{
			//cout << "+ 위!" << endl;
			m_CoordX = 1;
			m_CoordY = 0;
			m_MissileDir = { 0,-1 };
			m_AngleIndex = 2;
		}
	}
	else
	{
		if (angle < 22.f && angle > -22.f)
		{
			//cout << "- 왼쪽!" << endl;
			m_CoordX = 0;
			m_CoordY = 1;
			m_MissileDir = { -1,0 };
			m_AngleIndex = 0;
		}
		else if (angle < -22.f && angle > -67.f)
		{
			//cout << "- 왼쪽 아래!" << endl;
			m_CoordX = 0;
			m_CoordY = 2;
			m_MissileDir = { -1,1 };
			m_AngleIndex = 7;

		}
		else if (angle > 22.f && angle < 67.f)
		{
			//cout << "- 왼쪽 위!" << endl;
			m_CoordX = 0;
			m_CoordY = 0;
			m_MissileDir = { -1,-1 };
			m_AngleIndex = 1;

		}
		else if (angle > 67.f && angle < 90.f)
		{
			//cout << "- 위!" << endl;
			m_CoordX = 1;
			m_CoordY = 0;
			m_MissileDir = { 0,-1 };
			m_AngleIndex = 2;

		}
		else if (angle < -67.f && angle > -90.f)
		{
			//cout << "- 아래!" << endl;
			m_CoordX = 1;
			m_CoordY = 2;
			m_MissileDir = { 0,1 };
			m_AngleIndex = 6;

		}

	}

	

	// 방향의 범위를 짜는것이 문제이다.

	
}

void DoomsDay::Setting_Missile_Info()
{
	m_MissileBarrelPosArr[0] = { (float)(m_Area.left + 255), (float)(m_Area.top + 115) };
	m_MissileBarrelPosArr[1] = { (float)(m_Area.left + 320), (float)(m_Area.top + 99) };
	m_MissileBarrelPosArr[2] = { (float)(m_Area.left + 303), (float)(m_Area.top + 165) };

}

void DoomsDay::Move_BoomEffect_Frame()
{
	if (m_BoomEffectFrame.dwTime + m_BoomEffectFrame.dwSpeed < GetTickCount())
	{
		++m_BoomEffectFrame.iFrameStart;

		if (m_BoomEffectFrame.iFrameStart > m_BoomEffectFrame.iFrameEnd)
		{
			if (m_BoomEffectFrame.repeat == true)
			{
				m_BoomEffectFrame.iFrameStart = 0;
			}
			else if (m_BoomEffectFrame.repeat == false)
			{
				m_BoomEffectFrame.iFrameStart = m_BoomEffectFrame.iFrameEnd;
			}
		}

		m_BoomEffectFrame.dwTime = GetTickCount();
	}
}

void DoomsDay::Move_DeathEffect_Frame()
{
	if (m_DeathEffectFrame.dwTime + m_DeathEffectFrame.dwSpeed < GetTickCount())
	{
		++m_DeathEffectFrame.iFrameStart;

		if (m_DeathEffectFrame.iFrameStart > m_DeathEffectFrame.iFrameEnd)
		{
			if (m_DeathEffectFrame.repeat == true)
			{
				m_DeathEffectFrame.iFrameStart = 0;
			}
			else if (m_DeathEffectFrame.repeat == false)
			{
				m_DeathEffectFrame.iFrameStart = m_DeathEffectFrame.iFrameEnd;
			}

		}
		m_DeathEffectFrame.dwTime = GetTickCount();
	}
}

void DoomsDay::RandomPos_Effect()
{
	if (m_DeathRandomTime + 800 <= GetTickCount())
	{
		STOP_PLAY_SOUND(L"Destroyed.wav", CHANNELID::SOUND_EFFECT, 0.8f);
		for (int i = 0; i < 12; i++)
		{
			bool randBool = rand() % 2;
			int randPosX = rand() % 400;
			int randPosY = rand() % 200;

			Vec2 randPos = { (float)randPosX, (float)randPosY };
			m_DeathRandomPos[i] = { randPos, randBool };
		}
		m_DeathRandomTime = GetTickCount();
	}
	
}

void DoomsDay::DoomsDayOutScreen()
{
	int      ScrollX = (int)SCROLLX;
	int		 ScrollY = (int)SCROLLY;
	//cout << m_Pos.x - ScrollX << endl;
	if (m_Pos.x + ScrollX < -300)
	{
		SetDead();
	}

}

bool DoomsDay::GetDamaged(int dmg)
{
	SubHP(1);
	if (GetHP() == 0)
	{
		m_bDeath = true;
		m_DeathEffectFrame.dwTime = GetTickCount();
		m_BoomEffectFrame.dwTime = GetTickCount();
		STOP_SOUND(CHANNELID::SOUND_BGM);
		
		return true;
	}
	m_BlinkStartTime = GetTickCount();
	m_bBlink = true;
	m_bGetDamaged = true;
	
	return true;
}

void DoomsDay::DropRings()
{
}
