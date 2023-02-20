#include "pch.h"
#include "Bullet.h"


#include "ScrollManager.h"
#include "SoundManager.h"
#include "../Client/Player.h"
#include "Collider.h"

Bullet::Bullet(int ZOrder, const Vec2& Pos, const Vec2& Dir, bool guided, Creature* target) :
	ImageObject(ZOrder, L"", 0, 0, Pos, { 0,0 }, { 0,0 }, true, 0)
{
	m_Size.x = 16;
	m_Size.y = 16;

	m_Dir = Dir;
	m_Dir.Normalize();
	m_fSpeed = 5.f;

	m_bDestroyed = false;
}

Bullet::~Bullet()
{
}

void Bullet::Render()
{
	RENDER->TransparentImage
	(
		L"Missile",
		m_Area.left, m_Area.top,
		m_Size.x, m_Size.y,
		52, 52,
		8, 8,
		RGB(255, 0, 255)
	);
}

int Bullet::Update()
{
	int dead = ImageObject::Update();
	if (dead == OBJ_DEAD) return OBJ_DEAD;

	MissileOutScreen();
	if (GetDead() == true) return OBJ_DEAD;

	m_Pos = m_Pos + m_Dir * m_fSpeed;

	Update_Rect();
	return OBJ_NOEVENT;
}

void Bullet::Late_Update()
{
}

void Bullet::Collision_Up(Collider* collider, const int& px, const int& py)
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
			STOP_PLAY_SOUND(L"RocketHit.wav", CHANNELID::SOUND_OBJ, 1.0f);
			m_bDestroyed = true;
			SetDead();
		}
	}
}

void Bullet::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Bullet::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Bullet::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void Bullet::MissileOutScreen()
{
	int      ScrollX = (int)SCROLLX;
	int		 ScrollY = (int)SCROLLY;
	//cout << m_Pos.x - ScrollX << endl;
	if (m_Pos.x + ScrollX < -300 || m_Pos.x + ScrollX > 1100)
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