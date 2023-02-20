#include "pch.h"
#include "Meteor.h"
#include "ScrollManager.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"

Meteor::Meteor(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	int randNum = rand() % 100;
	if (randNum >= 50) m_RandomIndex = 0;
	else if (randNum >= 20) m_RandomIndex = 1;
	else m_RandomIndex = 2;

	m_FrameKeyArr[0] = L"BigMeteor";
	m_FrameKeyArr[1] = L"NormalMeteor";
	m_FrameKeyArr[2] = L"SmallMeteor";
	
	m_RenderSizeArr[0] = { 80,80 };
	m_RenderSizeArr[1] = { 48,48 };
	m_RenderSizeArr[2] = { 32,32 };

	m_Size = m_RenderSizeArr[m_RandomIndex] * 2;

	int speed = rand() % 9 + 5;
	m_fSpeed = speed;

	m_bDestroyed = false;
}

void Meteor::Render()
{
	if (!m_bDestroyed)
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr[m_RandomIndex],
			m_Area.left, m_Area.top,
			m_Size.x, m_Size.y,
			0, 0,
			m_RenderSizeArr[m_RandomIndex].x, m_RenderSizeArr[m_RandomIndex].y,
			RGB(255, 0, 255)
		);
	}
	else
	{
		
		switch (m_RandomIndex)
		{
		case 0:
			RenderBigMeteorDestroyed();
			break;
		case 1:
			RenderNormalMeteorDestroyed();
			break;
		case 2:
			RenderSmallMeteorDestroyed();
			break;
		}
		
		//L"MeteorFragment2"
	}
	
}

int Meteor::Update()
{
	bool dead = ImageObject::Update();
	if (dead == OBJ_DEAD) return OBJ_DEAD;

	MeteorOutScreen();
	if (GetDead() == true) return OBJ_DEAD;

	m_Pos.x = m_Pos.x - m_fSpeed;
	
	Update_Rect();
	return OBJ_NOEVENT;
}

void Meteor::Late_Update()
{
}

void Meteor::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (m_bDestroyed == true) return;
	if (GetCollisionWithCollider() == false)
	{
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Collision_Enter();
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		bool hit = player->GetDamaged(1);

		if (hit)
		{
			STOP_PLAY_SOUND(L"TouchStone2.wav", CHANNELID::SOUND_SUSTAIN, 0.6f);
			m_bDestroyed = true;
			m_fSpeed += 10.f;
		}
		
		
	}
}

void Meteor::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Meteor::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Meteor::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Meteor::Hit_With_SkillRing()
{
	if (IS_PLAYING(CHANNELID::SOUND_EFFECT) == FALSE)
		STOP_PLAY_SOUND(L"DestroyStone.wav", CHANNELID::SOUND_EFFECT, 0.6f);
	m_bDestroyed = true;
	m_fSpeed += 10.f;
}

void Meteor::MeteorOutScreen()
{
	int      ScrollX = (int)SCROLLX;

	//cout << m_Pos.x - ScrollX << endl;
	if (m_Pos.x + ScrollX < -100)
	{
		SetDead();
		return;
	}
	
}

void Meteor::RenderBigMeteorDestroyed()
{
	RENDER->TransparentImage
	(
		L"MeteorFragment1",
		m_Area.left, m_Area.top,
		48, 48,
		0, 0,
		24, 24,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"MeteorFragment1",
		m_Area.left + 100, m_Area.top,
		48, 48,
		32, 0,
		24, 24,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"MeteorFragment1",
		m_Area.left + 100, m_Area.top + 100,
		48, 48,
		32, 0,
		24, 24,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"MeteorFragment1",
		m_Area.left, m_Area.top + 100,
		48, 48,
		0, 0,
		24, 24,
		RGB(255, 0, 255)
	);
	RENDER->TransparentImage
	(
		L"MeteorFragment2",
		m_Area.left + 50, m_Area.top + 50,
		14, 26,
		0, 0,
		7, 8,
		RGB(255, 0, 255)
	);
}

void Meteor::RenderNormalMeteorDestroyed()
{
	RENDER->TransparentImage
	(
		L"MeteorFragment1",
		m_Area.left, m_Area.top,
		48, 48,
		32, 0,
		24, 24,
		RGB(255, 0, 255)
	);
	RENDER->TransparentImage
	(
		L"MeteorFragment2",
		m_Area.left, m_Area.top + 100,
		14, 26,
		0, 0,
		7, 8,
		RGB(255, 0, 255)
	);

}

void Meteor::RenderSmallMeteorDestroyed()
{
	RENDER->TransparentImage
	(
		L"MeteorFragment2",
		m_Area.left, m_Area.top + 100,
		14, 26,
		0, 0,
		7, 8,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"MeteorFragment2",
		m_Area.left + 70, m_Area.top + 100,
		14, 26,
		16, 0,
		7, 8,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"MeteorFragment2",
		m_Area.left + 20, m_Area.top + 30,
		14, 26,
		16, 0,
		7, 8,
		RGB(255, 0, 255)
	);
}
